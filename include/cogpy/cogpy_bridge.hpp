/*++
 * cogpy_bridge.hpp — Bridge between cogpy-hpp (cog::) and ATenSpace (at::atomspace)
 *
 * Part of WinKoGNN Unified AGI-OS Build
 *
 * This header provides the integration layer that maps the header-only
 * cogpy-hpp modules to the ATenSpace tensor-backed AtomSpace and the
 * NT kernel substrate.
 *
 * The bridge follows the optimal-cognitive-grip composition:
 *   A( S( snn[ NSE(GNE(TC)) ⊕ PLA ] ⊗ WC( FC[nn→Lnn] ⊗ ⊕⊗ ))) ≅ S∞
 *--*/

#pragma once

/* ------------------------------------------------------------------ */
/* Include cogpy-hpp modules                                           */
/* ------------------------------------------------------------------ */
#include <cog/core/core.hpp>
#include <cog/prime/prime.hpp>
#include <cog/gml/gml.hpp>
#include <cog/pilot/pilot.hpp>
#include <cog/grip/grip.hpp>
#include <cog/mach/mach.hpp>
#include <cog/plan9/plan9.hpp>
#include <cog/glow/glow.hpp>
#include <cog/webvm/webvm.hpp>

#include <string>
#include <vector>
#include <cstring>

namespace winkognn {

/* ================================================================== */
/* Unified AtomSpace Adapter                                           */
/* ================================================================== */
class UnifiedAtomSpace {
public:
    explicit UnifiedAtomSpace(size_t initial_capacity = 65536) {
        (void)initial_capacity;
    }

    cog::Handle add_node(cog::AtomType type, const std::string& name,
                         const cog::TruthValue& tv = cog::TruthValue(1.0f, 1.0f)) {
        return core_space_.add_node(type, name, tv);
    }

    cog::Handle add_link(cog::AtomType type,
                         const std::vector<cog::Handle>& outgoing,
                         const cog::TruthValue& tv = cog::TruthValue(1.0f, 1.0f)) {
        return core_space_.add_link(type, outgoing, tv);
    }

    const cog::Atom* get_atom(cog::Handle h) const {
        return core_space_.get_atom(h);
    }

    size_t size() const { return core_space_.size(); }

    cog::AtomSpace& core() { return core_space_; }
    const cog::AtomSpace& core() const { return core_space_; }

private:
    cog::AtomSpace core_space_;
};

/* ================================================================== */
/* Unified Cognitive Engine                                            */
/* ================================================================== */
class UnifiedCognitiveEngine {
public:
    enum class Mode {
        REACTIVE,
        PROACTIVE,
        GOAL_DIRECTED,
        EXPLORATORY,
        BALANCED
    };

    explicit UnifiedCognitiveEngine(
        UnifiedAtomSpace& space,
        Mode mode = Mode::BALANCED,
        size_t reservoir_size = 64)
        : space_(space)
        , mode_(mode)
        , grip_(reservoir_size)
        , cycle_count_(0)
    {
        grip_.set_composability(0.5f);
        grip_.update_differentiability(0.5f, 0.5f);

        grip_.workflow().add(cog::grip::Verb::PERCEIVE, "environment");
        grip_.workflow().add(cog::grip::Verb::ATTEND, "salience");
        grip_.workflow().add(cog::grip::Verb::REMEMBER, "episodic");
        grip_.workflow().add(cog::grip::Verb::REASON, "inference");
        grip_.workflow().add(cog::grip::Verb::PLAN, "goals");
        grip_.workflow().add(cog::grip::Verb::DECIDE, "action_selection");
        grip_.workflow().add(cog::grip::Verb::ACT, "execute");
        grip_.workflow().add(cog::grip::Verb::LEARN, "experience");
        grip_.workflow().add(cog::grip::Verb::REFLECT, "meta_cognition");
        grip_.workflow().add(cog::grip::Verb::EVOLVE, "self_improvement");
    }

    void step() {
        grip_.workflow().advance();
        grip_.evolution_step();
        cycle_count_++;
    }

    void run(size_t n_cycles) {
        for (size_t i = 0; i < n_cycles; ++i) step();
    }

    cog::grip::GripDimensions compute_grip() {
        return grip_.compute_grip();
    }

    size_t cycle_count() const { return cycle_count_; }
    Mode mode() const { return mode_; }
    void set_mode(Mode m) { mode_ = m; }
    cog::grip::CognitiveGrip& grip() { return grip_; }

private:
    UnifiedAtomSpace& space_;
    Mode mode_;
    cog::grip::CognitiveGrip grip_;
    size_t cycle_count_;
};

/* ================================================================== */
/* NT Kernel Bridge                                                    */
/* ================================================================== */
class NTKernelBridge {
public:
    struct MemoryRegion {
        void*   base;
        size_t  size;
        bool    committed;
    };

    struct ProcessContext {
        uint32_t    pid;
        std::string name;
        bool        active;
    };

    struct IPCPort {
        std::string name;
        bool        connected;
    };

    MemoryRegion allocate_memory(size_t size) {
        MemoryRegion region;
        region.base = malloc(size);
        region.size = size;
        region.committed = (region.base != nullptr);
        if (region.committed) memset(region.base, 0, size);
        regions_.push_back(region);
        return region;
    }

    void free_memory(MemoryRegion& region) {
        if (region.base) { free(region.base); region.base = nullptr; region.committed = false; }
    }

    ProcessContext create_process(const std::string& name) {
        ProcessContext ctx;
        ctx.pid = next_pid_++;
        ctx.name = name;
        ctx.active = true;
        processes_.push_back(ctx);
        return ctx;
    }

    IPCPort create_port(const std::string& name) {
        IPCPort port; port.name = name; port.connected = true;
        ports_.push_back(port);
        return port;
    }

    size_t active_processes() const { return processes_.size(); }
    size_t allocated_regions() const { return regions_.size(); }
    size_t active_ports() const { return ports_.size(); }

    ~NTKernelBridge() {
        for (auto& r : regions_) { if (r.base) free(r.base); }
    }

private:
    std::vector<MemoryRegion> regions_;
    std::vector<ProcessContext> processes_;
    std::vector<IPCPort> ports_;
    uint32_t next_pid_ = 1000;
};

/* ================================================================== */
/* AGI-OS Unified System                                               */
/* ================================================================== */
class AGIOSSystem {
public:
    explicit AGIOSSystem(
        UnifiedCognitiveEngine::Mode mode = UnifiedCognitiveEngine::Mode::BALANCED,
        size_t reservoir_size = 64)
        : space_()
        , engine_(space_, mode, reservoir_size)
        , kernel_()
    {}

    void initialize() {
        kernel_.create_process("cogkernel");
        kernel_.create_port("\\CognitivePort");
        kernel_.allocate_memory(1024 * 1024);

        auto self = space_.add_node(cog::AtomType::CONCEPT_NODE, "SELF");
        auto world = space_.add_node(cog::AtomType::CONCEPT_NODE, "WORLD");
        auto perceive = space_.add_node(cog::AtomType::PREDICATE_NODE, "perceives");
        space_.add_link(cog::AtomType::EVALUATION_LINK, {perceive, self, world});
    }

    void cognitive_step() { engine_.step(); }
    void run(size_t n) { engine_.run(n); }

    struct SystemStatus {
        size_t atoms;
        size_t cycles;
        size_t processes;
        size_t memory_regions;
        size_t ipc_ports;
        float  grip_product;
    };

    SystemStatus status() {
        auto grip = engine_.compute_grip();
        return {
            space_.size(), engine_.cycle_count(),
            kernel_.active_processes(), kernel_.allocated_regions(),
            kernel_.active_ports(), grip.product()
        };
    }

    UnifiedAtomSpace& space() { return space_; }
    UnifiedCognitiveEngine& engine() { return engine_; }
    NTKernelBridge& kernel() { return kernel_; }

private:
    UnifiedAtomSpace space_;
    UnifiedCognitiveEngine engine_;
    NTKernelBridge kernel_;
};

} // namespace winkognn
