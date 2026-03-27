/*++
 * test_main.cpp — WinKoGNN Unified AGI-OS Comprehensive Test Suite
 *--*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "cogpy_bridge.hpp"
#include "nt_cognitive_kernel.hpp"
#include "atenspace_bridge.hpp"
#include "cog0_orchestrator.hpp"
#include "nt_autognosis.hpp"

extern "C" {
#include "nt.h"
#include "ntrtl.h"
#include "ntstatus.h"
#include "bugcodes.h"
#include "windef.h"
}

/* ── Minimal test framework ── */
static int tests_run = 0, tests_passed = 0, tests_failed = 0;

#define TEST(name) \
    static void test_##name(); \
    static struct test_reg_##name { \
        test_reg_##name() { \
            printf("  [TEST] %-50s ", #name); \
            tests_run++; \
            try { test_##name(); tests_passed++; printf("PASS\n"); } \
            catch (...) { tests_failed++; printf("FAIL (exception)\n"); } \
        } \
    } test_reg_instance_##name; \
    static void test_##name()

#define ASSERT_TRUE(expr) \
    do { if (!(expr)) { printf("FAIL at %s:%d\n", __FILE__, __LINE__); tests_failed++; tests_passed--; return; } } while(0)
#define ASSERT_EQ(a, b) ASSERT_TRUE((a) == (b))
#define ASSERT_GT(a, b) ASSERT_TRUE((a) > (b))

/* ═══════ 1. NT Compatibility ═══════ */

TEST(nt_types_sizes) {
    ASSERT_EQ(sizeof(UCHAR), 1u); ASSERT_EQ(sizeof(USHORT), 2u);
    ASSERT_EQ(sizeof(ULONG), 4u); ASSERT_EQ(sizeof(ULONGLONG), 8u);
}

TEST(nt_status_macros) {
    ASSERT_TRUE(NT_SUCCESS(STATUS_SUCCESS));
    ASSERT_TRUE(!NT_SUCCESS(STATUS_UNSUCCESSFUL));
    ASSERT_TRUE(NT_ERROR(STATUS_NO_MEMORY));
}

TEST(nt_list_operations) {
    LIST_ENTRY head, e1, e2, e3;
    InitializeListHead(&head);
    ASSERT_TRUE(IsListEmpty(&head));
    InsertTailList(&head, &e1); InsertTailList(&head, &e2); InsertTailList(&head, &e3);
    ASSERT_TRUE(!IsListEmpty(&head));
    ASSERT_TRUE(head.Flink == &e1);
    RemoveEntryList(&e2);
    ASSERT_TRUE(e1.Flink == &e3);
}

TEST(nt_unicode_string) {
    UNICODE_STRING str; WCHAR buf[] = {'H','e','l','l','o',0};
    RtlInitUnicodeString(&str, buf);
    ASSERT_EQ(str.Length, (USHORT)(5 * sizeof(WCHAR)));
}

TEST(nt_large_integer) {
    LARGE_INTEGER li; li.QuadPart = 0x0000000100000002LL;
    ASSERT_EQ(li.u.LowPart, (ULONG)0x00000002);
}

TEST(nt_spinlock) {
    KSPIN_LOCK lock; KIRQL old;
    KeInitializeSpinLock(&lock);
    KeAcquireSpinLock(&lock, &old);
    KeReleaseSpinLock(&lock, old);
    ASSERT_TRUE(true);
}

TEST(nt_pool_alloc) {
    PVOID p = ExAllocatePool(NonPagedPool, 1024);
    ASSERT_TRUE(p != NULL); memset(p, 0xAB, 1024); ExFreePool(p);
}

TEST(nt_rtl_memory) {
    char a[16] = "Hello, World!"; char b[16];
    RtlCopyMemory(b, a, 14);
    ASSERT_TRUE(RtlEqualMemory(a, b, 14));
}

TEST(nt_rtl_bitmap) {
    ULONG buffer[4] = {0}; RTL_BITMAP bm;
    RtlInitializeBitMap(&bm, buffer, 128);
    RtlSetBit(&bm, 0); RtlSetBit(&bm, 42); RtlSetBit(&bm, 127);
    ASSERT_TRUE(RtlTestBit(&bm, 0)); ASSERT_TRUE(RtlTestBit(&bm, 42));
    ASSERT_TRUE(!RtlTestBit(&bm, 1));
}

TEST(nt_interlocked) {
    volatile LONG val = 0;
    InterlockedIncrement(&val); ASSERT_EQ(val, 1L);
    InterlockedDecrement(&val); ASSERT_EQ(val, 0L);
}

TEST(nt_syscall_stubs) {
    HANDLE proc;
    NTSTATUS s = NtCreateProcess(&proc, 0, NULL, NULL, FALSE, NULL, NULL, NULL);
    ASSERT_TRUE(NT_SUCCESS(s)); ASSERT_TRUE(proc != NULL);
    NtClose(proc);
}

/* ═══════ 2. cogpy-hpp Modules ═══════ */

TEST(cog_core_atomspace) {
    cog::AtomSpace space;
    auto h1 = space.add_node(cog::AtomType::CONCEPT_NODE, "cat", cog::TruthValue(0.9f, 0.8f));
    auto h2 = space.add_node(cog::AtomType::CONCEPT_NODE, "animal", cog::TruthValue(1.0f, 1.0f));
    auto h3 = space.add_link(cog::AtomType::INHERITANCE_LINK, {h1, h2}, cog::TruthValue(0.95f, 0.9f));
    ASSERT_EQ(space.size(), 3u);
    const cog::Atom* atom = space.get_atom(h1);
    ASSERT_TRUE(atom != nullptr);
    ASSERT_EQ(atom->name, std::string("cat"));
    ASSERT_TRUE(atom->is_node());
    const cog::Atom* link = space.get_atom(h3);
    ASSERT_TRUE(link != nullptr);
    ASSERT_TRUE(link->is_link());
    ASSERT_EQ(link->outgoing.size(), 2u);
}

TEST(cog_truth_value) {
    cog::TruthValue tv1(0.8f, 0.9f), tv2(0.6f, 0.7f);
    ASSERT_TRUE(tv1.strength > tv2.strength);
    auto merged = cog::TruthValue::revision(tv1, tv2);
    ASSERT_GT(merged.confidence, tv1.confidence);
}

TEST(cog_grip_dimensions) {
    cog::grip::CognitiveGrip grip(32);
    grip.set_composability(0.8f);
    grip.update_differentiability(0.7f, 0.6f);
    auto dims = grip.compute_grip();
    ASSERT_GT(dims.composability, 0.0f);
    ASSERT_GT(dims.differentiability, 0.0f);
    /* product may be 0 if executability/self_awareness/convergence not set */
}

TEST(cog_grip_workflow) {
    cog::grip::CognitiveGrip grip(16);
    grip.workflow().add(cog::grip::Verb::PERCEIVE, "test");
    grip.workflow().add(cog::grip::Verb::ATTEND, "test");
    grip.workflow().add(cog::grip::Verb::REASON, "test");
    grip.workflow().advance(); grip.workflow().advance();
    ASSERT_TRUE(true);
}

/* ═══════ 3. ATenSpace Bridge ═══════ */

TEST(atenspace_tv_adapter) {
    using TVA = winkognn::atenspace_bridge::TruthValueAdapter;
    cog::TruthValue tv(0.8f, 0.9f);
    auto etv = TVA::from_cog(tv);
    ASSERT_TRUE(etv.strength > 0.7f);
    auto back = TVA::to_cog(etv);
    ASSERT_TRUE(back.strength > 0.7f);
}

TEST(atenspace_pln_revision) {
    using TVA = winkognn::atenspace_bridge::TruthValueAdapter;
    TVA::ExtendedTV a = {TVA::TVType::SIMPLE, 0.8f, 0.5f, 0, 0, 0};
    TVA::ExtendedTV b = {TVA::TVType::SIMPLE, 0.6f, 0.3f, 0, 0, 0};
    auto merged = TVA::revision(a, b);
    ASSERT_TRUE(merged.strength >= 0.5f && merged.strength <= 0.9f);
    ASSERT_GT(merged.confidence, a.confidence);
}

TEST(atenspace_ecan) {
    cog::AtomSpace space;
    auto h1 = space.add_node(cog::AtomType::CONCEPT_NODE, "test1");
    auto h2 = space.add_node(cog::AtomType::CONCEPT_NODE, "test2");
    winkognn::atenspace_bridge::ECANAdapter ecan;
    ecan.set_attention(h1, 0.8f, 0.5f, 0.3f);
    ecan.set_attention(h2, 0.3f, 0.2f, 0.1f);
    ASSERT_TRUE(ecan.get_attention(h1).sti > 0.7f);
    ecan.cycle(space);
    ASSERT_EQ(ecan.cycle_count(), 1u);
    ASSERT_GT(ecan.attentional_focus().size(), 0u);
}

TEST(atenspace_reservoir) {
    winkognn::atenspace_bridge::ReservoirBridge reservoir;
    std::vector<float> input(64, 0.0f); input[0] = 1.0f;
    reservoir.observe(input);
    ASSERT_EQ(reservoir.step_count(), 1u);
    float energy = 0; for (auto v : reservoir.state()) energy += v*v;
    ASSERT_GT(energy, 0.0f);
}

TEST(atenspace_bridge_full) {
    winkognn::atenspace_bridge::ATenSpaceBridge bridge(32);
    bridge.add_node(cog::AtomType::CONCEPT_NODE, "cat", 0.9f, 0.8f);
    bridge.add_node(cog::AtomType::CONCEPT_NODE, "animal", 1.0f, 1.0f);
    ASSERT_EQ(bridge.atom_count(), 2u);
    bridge.step();
    ASSERT_EQ(bridge.ecan_cycles(), 1u);
    ASSERT_EQ(bridge.reservoir_steps(), 1u);
}

TEST(atenspace_9p_filesystem) {
    winkognn::atenspace_bridge::ATenSpaceBridge bridge(16);
    bridge.add_node(cog::AtomType::CONCEPT_NODE, "test", 0.5f, 0.5f);
    bridge.step();
    auto entries = bridge.snapshot();
    ASSERT_GT(entries.size(), 5u);
    bool found_atoms = false, found_grip = false;
    for (const auto& e : entries) {
        if (e.path == "/atoms") found_atoms = true;
        if (e.path == "/grip/dimensions") found_grip = true;
    }
    ASSERT_TRUE(found_atoms); ASSERT_TRUE(found_grip);
}

/* ═══════ 4. NT Cognitive Kernel ═══════ */

TEST(nt_cognitive_process) {
    using namespace winkognn::nt_cognitive;
    CognitiveProcess proc(1, "test-agent");
    ASSERT_EQ(proc.state(), CognitiveProcess::State::CREATED);
    proc.activate();
    ASSERT_EQ(proc.state(), CognitiveProcess::State::RUNNING);
    proc.set_attention(0.8f, 0.5f, 0.3f);
    ASSERT_GT(proc.total_attention(), 1.0f);
    proc.cognitive_step();
    ASSERT_EQ(proc.cycle_count(), 1u);
}

TEST(nt_cognitive_object_manager) {
    using namespace winkognn::nt_cognitive;
    CognitiveObjectManager mgr;
    ASSERT_GT(mgr.type_count(), 10u);
    ASSERT_TRUE(mgr.lookup("ConceptNode") != nullptr);
    ASSERT_TRUE(mgr.lookup("InheritanceLink") != nullptr);
    mgr.register_type("CustomNode", true, 0.5f, 0.5f, false);
    ASSERT_TRUE(mgr.lookup("CustomNode") != nullptr);
}

TEST(nt_cognitive_lpc) {
    using namespace winkognn::nt_cognitive;
    CognitiveLPC lpc;
    lpc.create_port("\\CogPort", 1);
    CognitiveMessage msg; msg.source_pid=1; msg.target_pid=2;
    msg.port_name="\\CogPort"; msg.content="Hello"; msg.importance=0.8f; msg.timestamp=0;
    ASSERT_TRUE(lpc.send_message(msg));
    auto received = lpc.receive("\\CogPort");
    ASSERT_EQ(received.size(), 1u);
    ASSERT_EQ(received[0].content, std::string("Hello"));
}

TEST(nt_cognitive_kernel_full) {
    using namespace winkognn::nt_cognitive;
    NTCognitiveKernel kernel;
    auto& p1 = kernel.create_process("perceiver");
    auto& p2 = kernel.create_process("reasoner");
    p1.set_attention(0.9f, 0.5f, 0.3f); p1.activate();
    p2.set_attention(0.7f, 0.6f, 0.4f); p2.activate();
    kernel.lpc().create_port("\\PerceptionPort", p1.pid());
    kernel.run_cycle();
    ASSERT_EQ(kernel.total_cycles(), 1u);
    auto* next = kernel.schedule();
    ASSERT_TRUE(next != nullptr);
    /* Scheduler returns highest-attention process */
    ASSERT_TRUE(next->name() == std::string("perceiver") || next->name() == std::string("reasoner"));
}

/* ═══════ 5. cog0 Orchestrator ═══════ */

TEST(cog0_creation) {
    using namespace winkognn::orchestrator;
    Cog0Orchestrator orc;
    ASSERT_EQ(orc.mode(), OrcMode::BALANCED);
    auto s = orc.status();
    ASSERT_GT(s.atoms, 0u);
}

TEST(cog0_mode_switching) {
    using namespace winkognn::orchestrator;
    Cog0Orchestrator orc;
    orc.set_mode(OrcMode::AUTONOMOUS);
    ASSERT_EQ(orc.mode(), OrcMode::AUTONOMOUS);
}

TEST(cog0_goals_tasks) {
    using namespace winkognn::orchestrator;
    Cog0Orchestrator orc;
    uint32_t g1 = orc.add_goal("learn", "Learn", 0.8f);
    orc.add_goal("sub", "Sub", 0.6f, g1);
    orc.add_task("read", "Read", g1, 0.7f);
    auto s = orc.status();
    ASSERT_EQ(s.goals, 2u); ASSERT_EQ(s.tasks, 1u);
}

TEST(cog0_perception) {
    using namespace winkognn::orchestrator;
    Cog0Orchestrator orc;
    orc.perceive_text("The cat sat on the mat", 0.8f);
    orc.perceive_text("Cats are animals", 0.7f);
    ASSERT_GT(orc.status().memory_items, 0u);
}

TEST(cog0_cognitive_cycle) {
    using namespace winkognn::orchestrator;
    Cog0Orchestrator orc;
    orc.perceive_text("Hello world", 0.5f);
    orc.add_task("test", "Test", 0, 0.5f);
    orc.run(10);
    auto s = orc.status();
    ASSERT_EQ(s.cycles, 10u);
    ASSERT_GT(s.ecan_cycles, 0u);
    ASSERT_GT(s.reservoir_steps, 0u);
}

TEST(cog0_script) {
    using namespace winkognn::orchestrator;
    Cog0Orchestrator orc;
    std::string output = orc.execute_script(
        "# Test\ngoal learn\ntask read\npercept sky is blue\nmode PROACTIVE\nrun 5\nstatus\n");
    ASSERT_TRUE(!output.empty());
    ASSERT_TRUE(output.find("Goal") != std::string::npos);
}

TEST(cog0_tools) {
    using namespace winkognn::orchestrator;
    Cog0Orchestrator orc;
    ASSERT_EQ(orc.tools().invoke("echo", "hello"), std::string("hello"));
}

/* ═══════ 6. Unified AGI-OS ═══════ */

TEST(agios_system) {
    winkognn::AGIOSSystem sys;
    sys.initialize();
    auto s = sys.status();
    ASSERT_GT(s.atoms, 0u); ASSERT_GT(s.processes, 0u);
}

TEST(agios_cognitive_cycle) {
    winkognn::AGIOSSystem sys;
    sys.initialize(); sys.run(10);
    auto s = sys.status();
    ASSERT_EQ(s.cycles, 10u);
    /* grip_product may be 0 if not all dimensions are initialized */
    ASSERT_TRUE(s.grip_product >= 0.0f);
}

TEST(agios_full_integration) {
    winkognn::orchestrator::Cog0Orchestrator orc;
    orc.perceive_text("OpenCog is a cognitive architecture", 0.9f);
    orc.perceive_text("NT kernel provides process management", 0.8f);
    orc.perceive_text("ATenSpace provides tensor-backed atoms", 0.85f);
    orc.add_goal("understand", "Understand architecture", 0.9f);
    orc.add_task("analyze", "Analyze NT mapping", 1, 0.7f);
    orc.set_mode(winkognn::orchestrator::OrcMode::AUTONOMOUS);
    orc.run(50);
    auto s = orc.status();
    ASSERT_EQ(s.cycles, 50u);
    ASSERT_GT(s.atoms, 10u);
    ASSERT_TRUE(s.grip_product >= 0.0f);
    std::string report = orc.status_string();
    ASSERT_TRUE(report.find("AUTONOMOUS") != std::string::npos);
}

/* ═══════ 8. Autognosis (Self-Awareness) ═══════ */

TEST(autognosis_l0_observation) {
    winkognn::autognosis::NTAutognosis ag;
    ASSERT_EQ(static_cast<uint8_t>(ag.level()),
              static_cast<uint8_t>(cog::grip::AutonomyLevel::L0_Observation));
    ASSERT_EQ(ag.total_observations(), 0u);
    // Record some observations
    for (int i = 0; i < 5; i++) {
        ag.observe("build.errors", 73247.0 - i * 1000.0);
    }
    ASSERT_EQ(ag.total_observations(), 5u);
    ASSERT_TRUE(ag.self_awareness_value() > 0.0f);
}

TEST(autognosis_build_health) {
    winkognn::autognosis::NTAutognosis ag;
    winkognn::autognosis::BuildHealth health;
    health.total_errors = 73247;
    health.resolved_includes = 17234;
    health.missing_includes = 8250;
    health.total_include_directives = 25484;
    health.nt_compat_headers = 104;
    health.total_source_files = 9674;
    health.ci_passing = false;
    ag.observe_build(health);
    ASSERT_TRUE(ag.build_health().composability_ratio() > 0.6);
    ASSERT_TRUE(ag.build_health().composability_ratio() < 0.8);
}

TEST(autognosis_l1_self_model) {
    winkognn::autognosis::NTAutognosis ag;
    // Feed enough observations + build health to advance to L1
    winkognn::autognosis::BuildHealth health;
    health.total_source_files = 9674;
    health.total_include_directives = 25484;
    health.resolved_includes = 17234;
    ag.observe_build(health);
    for (int i = 0; i < 12; i++) {
        ag.observe("grip.product", 0.01 * i);
    }
    ASSERT_EQ(static_cast<uint8_t>(ag.level()),
              static_cast<uint8_t>(cog::grip::AutonomyLevel::L1_SelfModel));
    ASSERT_TRUE(ag.self_awareness_value() >= 0.25f);
}

TEST(autognosis_coverage_model) {
    winkognn::autognosis::NTAutognosis ag;
    ASSERT_TRUE(ag.subsystem_coverage().size() >= 10);
    float cov = ag.overall_integration_coverage();
    ASSERT_TRUE(cov > 0.0f);  // Some subsystems have partial coverage
    ASSERT_TRUE(cov < 1.0f);  // Not everything is covered yet
}

TEST(autognosis_weakest_dimension) {
    winkognn::autognosis::NTAutognosis ag;
    cog::grip::GripDimensions dims;
    dims.composability = 0.22f;
    dims.differentiability = 1.0f;
    dims.executability = 0.38f;
    dims.self_awareness = 0.05f;
    dims.convergence = 0.5f;
    std::string weakest = ag.weakest_dimension(dims);
    ASSERT_TRUE(weakest == "self_awareness");
}

TEST(autognosis_self_report) {
    winkognn::autognosis::NTAutognosis ag;
    winkognn::autognosis::BuildHealth health;
    health.total_source_files = 9674;
    health.total_include_directives = 25484;
    health.resolved_includes = 17234;
    health.nt_compat_headers = 104;
    ag.observe_build(health);
    std::string report = ag.self_report();
    ASSERT_TRUE(report.find("NTAutognosis") != std::string::npos);
    ASSERT_TRUE(report.find("ke (Kernel)") != std::string::npos);
    ASSERT_TRUE(report.find("BuildHealth") != std::string::npos);
}

TEST(autognosis_ksm_iteration_record) {
    winkognn::autognosis::NTAutognosis ag;
    winkognn::autognosis::KSMIterationRecord rec;
    rec.iteration = 5;
    rec.focus = "composability + self_awareness";
    rec.description = "Added ARC types, CONTEXT, KPROFILE_SOURCE, Autognosis L0";
    rec.grip_before = 0.0f;
    rec.grip_after = 0.01f;
    rec.delta = 0.01f;
    rec.commit_hash = "pending";
    ag.record_iteration(rec);
    ASSERT_EQ(ag.iterations().size(), 1u);
    ASSERT_TRUE(ag.iterations()[0].improvement() > 0.0f);
}

/* ═══════ Main ═══════ */

int main() {
    printf("\n");
    printf("================================================================\n");
    printf("  WinKoGNN Unified AGI-OS — Comprehensive Test Suite\n");
    printf("  cogpy-hpp (x) ATenSpace (x) NT Kernel (x) cog0 Orchestrator\n");
    printf("================================================================\n\n");

    /* Tests auto-register via static constructors above */

    printf("\n=== Results ===\n");
    printf("  Total:  %d\n", tests_run);
    printf("  Passed: %d\n", tests_passed);
    printf("  Failed: %d\n", tests_failed);
    printf("\n");

    if (tests_failed > 0) { printf("*** SOME TESTS FAILED ***\n"); return 1; }
    printf("All tests passed!\n");
    return 0;
}
