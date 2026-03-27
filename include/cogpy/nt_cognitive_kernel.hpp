/*++
 * nt_cognitive_kernel.hpp — NT Kernel ↔ OpenCog Cognitive Integration
 *
 * Part of WinKoGNN Unified AGI-OS Build
 *
 * Deep integration layer that maps NT kernel subsystems (ke, mm, io,
 * ps, ob, se, ex, lpc) to OpenCog cognitive primitives.  This is the
 * key innovation: treating the NT microkernel architecture as a
 * cognitive substrate where:
 *
 *   NT Process Manager (ps)  → Agent lifecycle management
 *   NT Memory Manager (mm)   → AtomSpace memory regions
 *   NT Object Manager (ob)   → Atom type hierarchy
 *   NT I/O Manager (io)      → Perception/Action pipeline
 *   NT LPC (lpc)             → Inter-agent communication
 *   NT Security (se)         → Trust/truth value propagation
 *   NT Executive (ex)        → Cognitive cycle scheduler
 *   NT Kernel (ke)           → Attention allocation (ECAN)
 *
 * This maps directly to the b9/p9/j9 naming convention:
 *   b9: binary/base files → NT kernel objects (ke, mm)
 *   p9: membrane files    → process/object scopes (ps, ob, se)
 *   j9: distributed files → IPC/IO distribution (lpc, io, ex)
 *--*/

#pragma once

#include <cog/core/core.hpp>
#include <cog/prime/prime.hpp>
#include <cog/mach/mach.hpp>
#include <cog/grip/grip.hpp>

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <cstdint>

namespace winkognn {
namespace nt_cognitive {

/* ================================================================== */
/* NT Subsystem → Cognitive Module Mapping                             */
/* ================================================================== */

/**
 * CognitiveSubsystem — Enumerates the NT kernel subsystems and their
 * cognitive role in the AGI-OS.
 */
enum class Subsystem : uint8_t {
    KE_KERNEL       = 0,  // Attention allocation (ECAN scheduler)
    MM_MEMORY       = 1,  // AtomSpace memory regions
    PS_PROCESS      = 2,  // Agent lifecycle
    OB_OBJECT       = 3,  // Atom type hierarchy
    IO_MANAGER      = 4,  // Perception/Action I/O
    SE_SECURITY     = 5,  // Trust propagation
    EX_EXECUTIVE    = 6,  // Cognitive cycle executive
    LPC_COMMS       = 7,  // Inter-agent messaging
    CACHE_MGR       = 8,  // Working memory cache
    CONFIG_MGR      = 9,  // Knowledge base configuration
    NUM_SUBSYSTEMS  = 10
};

/**
 * SubsystemDescriptor — Metadata for each NT→Cognitive mapping.
 */
struct SubsystemDescriptor {
    Subsystem       id;
    const char*     nt_name;        // NT subsystem name
    const char*     nt_path;        // Path in private/ntos/
    const char*     cog_role;       // Cognitive role
    const char*     cog_module;     // cogpy-hpp module
    size_t          source_files;   // Number of C/H files
};

/**
 * Get the subsystem registry — maps all NT subsystems to cognitive roles.
 */
inline const SubsystemDescriptor* get_subsystem_registry() {
    static const SubsystemDescriptor registry[] = {
        { Subsystem::KE_KERNEL,   "ke",     "private/ntos/ke",     "ECAN Attention Scheduler",   "cog::prime",  0 },
        { Subsystem::MM_MEMORY,   "mm",     "private/ntos/mm",     "AtomSpace Memory Regions",   "cog::core",   0 },
        { Subsystem::PS_PROCESS,  "ps",     "private/ntos/ps",     "Agent Lifecycle Manager",    "cog::prime",  0 },
        { Subsystem::OB_OBJECT,   "ob",     "private/ntos/ob",     "Atom Type Hierarchy",        "cog::core",   0 },
        { Subsystem::IO_MANAGER,  "io",     "private/ntos/io",     "Perception/Action Pipeline", "cog::pilot",  0 },
        { Subsystem::SE_SECURITY, "se",     "private/ntos/se",     "Trust Value Propagation",    "cog::prime",  0 },
        { Subsystem::EX_EXECUTIVE,"ex",     "private/ntos/ex",     "Cognitive Cycle Executive",  "cog::grip",   0 },
        { Subsystem::LPC_COMMS,   "lpc",    "private/ntos/lpc",    "Inter-Agent Communication",  "cog::mach",   0 },
        { Subsystem::CACHE_MGR,   "cache",  "private/ntos/cache",  "Working Memory Cache",       "cog::gml",    0 },
        { Subsystem::CONFIG_MGR,  "config", "private/ntos/config", "Knowledge Base Config",      "cog::webvm",  0 },
    };
    return registry;
}

/* ================================================================== */
/* Cognitive Process — NT EPROCESS analogue for cognitive agents       */
/* ================================================================== */

/**
 * CognitiveProcess — Represents a cognitive agent as an NT-style
 * process with its own AtomSpace partition, attention budget, and
 * communication ports.
 */
class CognitiveProcess {
public:
    enum class State : uint8_t {
        CREATED,
        READY,
        RUNNING,
        WAITING,
        SUSPENDED,
        TERMINATED
    };

    struct AttentionBudget {
        float sti;      // Short-Term Importance (maps to NT priority)
        float lti;      // Long-Term Importance (maps to NT base priority)
        float vlti;     // Very Long-Term Importance (maps to NT affinity)
    };

    CognitiveProcess(uint32_t pid, const std::string& name)
        : pid_(pid)
        , name_(name)
        , state_(State::CREATED)
        , budget_{0.5f, 0.5f, 0.5f}
        , atom_count_(0)
        , cycle_count_(0)
    {}

    /* State transitions (mirrors NT process state machine) */
    void activate() {
        if (state_ == State::CREATED || state_ == State::READY)
            state_ = State::RUNNING;
    }
    void suspend() { state_ = State::SUSPENDED; }
    void resume() { state_ = State::READY; }
    void terminate() { state_ = State::TERMINATED; }
    void wait() { state_ = State::WAITING; }
    void ready() { state_ = State::READY; }

    /* Attention management (ECAN integration) */
    void set_attention(float sti, float lti, float vlti) {
        budget_ = {sti, lti, vlti};
    }
    float total_attention() const {
        return budget_.sti + budget_.lti + budget_.vlti;
    }

    /* Cognitive cycle */
    void cognitive_step() {
        if (state_ == State::RUNNING) {
            cycle_count_++;
            /* Decay attention over time (ECAN rent) */
            budget_.sti *= 0.99f;
        }
    }

    /* Accessors */
    uint32_t pid() const { return pid_; }
    const std::string& name() const { return name_; }
    State state() const { return state_; }
    const AttentionBudget& budget() const { return budget_; }
    size_t atom_count() const { return atom_count_; }
    size_t cycle_count() const { return cycle_count_; }

    void add_atoms(size_t n) { atom_count_ += n; }

private:
    uint32_t pid_;
    std::string name_;
    State state_;
    AttentionBudget budget_;
    size_t atom_count_;
    size_t cycle_count_;
};

/* ================================================================== */
/* Cognitive Object Manager — NT Object Manager for Atom types         */
/* ================================================================== */

/**
 * CognitiveObjectType — NT OBJECT_TYPE analogue for Atom types.
 * Each atom type has a type object that defines its behavior.
 */
struct CognitiveObjectType {
    std::string name;
    uint32_t    type_index;
    bool        is_node;        // true = Node, false = Link
    size_t      instance_count;

    /* Cognitive properties */
    float       default_sti;
    float       default_lti;
    bool        persistent;     // survives attention decay
};

class CognitiveObjectManager {
public:
    CognitiveObjectManager() : next_type_index_(0) {
        /* Register fundamental OpenCog atom types */
        register_type("ConceptNode", true, 0.5f, 0.5f, false);
        register_type("PredicateNode", true, 0.6f, 0.6f, false);
        register_type("VariableNode", true, 0.3f, 0.1f, false);
        register_type("NumberNode", true, 0.4f, 0.4f, false);
        register_type("SchemaNode", true, 0.7f, 0.7f, true);
        register_type("GroundedSchemaNode", true, 0.8f, 0.8f, true);
        register_type("InheritanceLink", false, 0.5f, 0.5f, false);
        register_type("EvaluationLink", false, 0.5f, 0.5f, false);
        register_type("ListLink", false, 0.3f, 0.3f, false);
        register_type("ImplicationLink", false, 0.6f, 0.6f, false);
        register_type("BindLink", false, 0.7f, 0.7f, true);
        register_type("ExecutionOutputLink", false, 0.8f, 0.8f, true);
        register_type("MemberLink", false, 0.5f, 0.5f, false);
        register_type("SimilarityLink", false, 0.5f, 0.5f, false);
        register_type("ContextLink", false, 0.4f, 0.4f, false);
        register_type("AtTimeLink", false, 0.4f, 0.4f, false);
    }

    void register_type(const std::string& name, bool is_node,
                       float default_sti, float default_lti, bool persistent) {
        CognitiveObjectType type;
        type.name = name;
        type.type_index = next_type_index_++;
        type.is_node = is_node;
        type.instance_count = 0;
        type.default_sti = default_sti;
        type.default_lti = default_lti;
        type.persistent = persistent;
        types_[name] = type;
    }

    CognitiveObjectType* lookup(const std::string& name) {
        auto it = types_.find(name);
        return (it != types_.end()) ? &it->second : nullptr;
    }

    size_t type_count() const { return types_.size(); }

    const std::map<std::string, CognitiveObjectType>& types() const {
        return types_;
    }

private:
    std::map<std::string, CognitiveObjectType> types_;
    uint32_t next_type_index_;
};

/* ================================================================== */
/* Cognitive LPC — Inter-Agent Communication (NT LPC analogue)         */
/* ================================================================== */

struct CognitiveMessage {
    uint32_t    source_pid;
    uint32_t    target_pid;
    std::string port_name;
    std::string content;
    float       importance;     // Message priority (ECAN-derived)
    uint64_t    timestamp;
};

class CognitiveLPC {
public:
    struct Port {
        std::string name;
        uint32_t    owner_pid;
        bool        active;
        std::vector<CognitiveMessage> queue;
    };

    Port* create_port(const std::string& name, uint32_t owner_pid) {
        Port port;
        port.name = name;
        port.owner_pid = owner_pid;
        port.active = true;
        ports_[name] = port;
        return &ports_[name];
    }

    bool send_message(const CognitiveMessage& msg) {
        auto it = ports_.find(msg.port_name);
        if (it == ports_.end() || !it->second.active) return false;
        it->second.queue.push_back(msg);
        return true;
    }

    std::vector<CognitiveMessage> receive(const std::string& port_name) {
        auto it = ports_.find(port_name);
        if (it == ports_.end()) return {};
        auto msgs = std::move(it->second.queue);
        it->second.queue.clear();
        return msgs;
    }

    size_t port_count() const { return ports_.size(); }

private:
    std::map<std::string, Port> ports_;
};

/* ================================================================== */
/* Cognitive Executive — NT Executive for cognitive cycle scheduling    */
/* ================================================================== */

class CognitiveExecutive {
public:
    CognitiveExecutive()
        : total_cycles_(0)
        , active_processes_(0)
    {}

    /**
     * Schedule and run one cognitive cycle across all active processes.
     * This is the NT KeDispatcherReady / KiQuantumEnd analogue.
     */
    void dispatch_cycle(std::vector<CognitiveProcess>& processes) {
        active_processes_ = 0;
        for (auto& proc : processes) {
            if (proc.state() == CognitiveProcess::State::RUNNING) {
                proc.cognitive_step();
                active_processes_++;
            }
        }
        total_cycles_++;
    }

    /**
     * Select the highest-attention process to run next.
     * This is the NT priority scheduler analogue, but uses
     * ECAN attention values instead of fixed priorities.
     */
    CognitiveProcess* select_next(std::vector<CognitiveProcess>& processes) {
        CognitiveProcess* best = nullptr;
        float best_attention = -1.0f;
        for (auto& proc : processes) {
            if (proc.state() == CognitiveProcess::State::READY ||
                proc.state() == CognitiveProcess::State::RUNNING) {
                float attn = proc.total_attention();
                if (attn > best_attention) {
                    best_attention = attn;
                    best = &proc;
                }
            }
        }
        return best;
    }

    size_t total_cycles() const { return total_cycles_; }
    size_t active_processes() const { return active_processes_; }

private:
    size_t total_cycles_;
    size_t active_processes_;
};

/* ================================================================== */
/* NTCognitiveKernel — Unified NT + OpenCog kernel                     */
/* ================================================================== */

class NTCognitiveKernel {
public:
    NTCognitiveKernel()
        : next_pid_(1)
    {}

    /* --- Process management (ps subsystem) --- */
    CognitiveProcess& create_process(const std::string& name) {
        uint32_t pid = next_pid_++;
        processes_.emplace_back(pid, name);
        return processes_.back();
    }

    CognitiveProcess* find_process(uint32_t pid) {
        for (auto& p : processes_) {
            if (p.pid() == pid) return &p;
        }
        return nullptr;
    }

    /* --- Object management (ob subsystem) --- */
    CognitiveObjectManager& object_manager() { return obj_mgr_; }

    /* --- Communication (lpc subsystem) --- */
    CognitiveLPC& lpc() { return lpc_; }

    /* --- Executive (ex subsystem) --- */
    void run_cycle() {
        executive_.dispatch_cycle(processes_);
    }

    CognitiveProcess* schedule() {
        return executive_.select_next(processes_);
    }

    /* --- Status --- */
    size_t process_count() const { return processes_.size(); }
    size_t type_count() const { return obj_mgr_.type_count(); }
    size_t port_count() const { return lpc_.port_count(); }
    size_t total_cycles() const { return executive_.total_cycles(); }

    const std::vector<CognitiveProcess>& processes() const { return processes_; }

private:
    std::vector<CognitiveProcess> processes_;
    CognitiveObjectManager obj_mgr_;
    CognitiveLPC lpc_;
    CognitiveExecutive executive_;
    uint32_t next_pid_;
};

} // namespace nt_cognitive
} // namespace winkognn
