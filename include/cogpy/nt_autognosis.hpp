/*++
 * nt_autognosis.hpp — NT Kernel Self-Awareness Module (Autognosis)
 *
 * Part of WinKoGNN Unified AGI-OS Build — NetCog KSM Iteration 5
 *
 * This module implements the Self-Awareness dimension of the 5D cognitive
 * grip, mapping NT kernel self-monitoring mechanisms to the Autognosis
 * hierarchy:
 *
 *   L0: Observation     — Performance counters, build metrics
 *   L1: Self-Model      — Model of own capabilities and coverage
 *   L2: Meta-Cognition  — Reasoning about own reasoning (grip analysis)
 *   L3: Self-Modification — Hot-patching, runtime code replacement
 *   L4: Recursive Self-Improve — Fixed-point: T(NetCog) = NetCog
 *
 * The key insight: the NT kernel already has self-monitoring primitives
 * (perf counters, ETW, bugcheck, hot-patching, kernel debugger). We
 * map these to the Autognosis levels to create a self-aware kernel.
 *
 * This module provides L0 (Observation) and L1 (Self-Model) to bootstrap
 * the self_awareness dimension above 0, unblocking the multiplicative
 * grip product.
 *--*/
#pragma once

#include <cog/grip/grip.hpp>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cstdint>
#include <algorithm>
#include <chrono>

namespace winkognn {
namespace autognosis {

/* ================================================================== */
/* NT Performance Counter Analogue                                     */
/* ================================================================== */

/**
 * PerfCounter — Maps to NT kernel performance counters (KeQueryPerformanceCounter).
 * L0 Observation: raw metric collection about the system's own state.
 */
struct PerfCounter {
    std::string name;
    double      value;
    double      min_value;
    double      max_value;
    uint64_t    sample_count;
    std::chrono::steady_clock::time_point last_updated;

    PerfCounter()
        : value(0), min_value(1e18), max_value(-1e18), sample_count(0)
        , last_updated(std::chrono::steady_clock::now()) {}

    explicit PerfCounter(const std::string& n)
        : name(n), value(0), min_value(1e18), max_value(-1e18), sample_count(0)
        , last_updated(std::chrono::steady_clock::now()) {}

    void record(double v) {
        value = v;
        min_value = std::min(min_value, v);
        max_value = std::max(max_value, v);
        sample_count++;
        last_updated = std::chrono::steady_clock::now();
    }
};

/* ================================================================== */
/* Build Health Monitor                                                */
/* ================================================================== */

/**
 * BuildHealth — Tracks the CI/CD build state of the unified AGI-OS.
 * This is the kernel's awareness of its own compilation health.
 */
struct BuildHealth {
    uint64_t total_errors;
    uint64_t total_warnings;
    uint64_t resolved_includes;
    uint64_t missing_includes;
    uint64_t nt_compat_headers;
    uint64_t total_source_files;
    uint64_t total_include_directives;
    bool     ci_passing;

    BuildHealth()
        : total_errors(0), total_warnings(0)
        , resolved_includes(0), missing_includes(0)
        , nt_compat_headers(0), total_source_files(0)
        , total_include_directives(0), ci_passing(false) {}

    double composability_ratio() const {
        if (total_include_directives == 0) return 0.0;
        return static_cast<double>(resolved_includes) / total_include_directives;
    }

    std::string to_string() const {
        std::ostringstream oss;
        oss << "BuildHealth["
            << "errors=" << total_errors
            << " resolved=" << resolved_includes
            << "/" << total_include_directives
            << " (" << (composability_ratio() * 100.0) << "%)"
            << " nt_compat=" << nt_compat_headers
            << " ci=" << (ci_passing ? "PASS" : "FAIL")
            << "]";
        return oss.str();
    }
};

/* ================================================================== */
/* Integration Coverage Model                                          */
/* ================================================================== */

/**
 * SubsystemCoverage — L1 Self-Model: the kernel's model of which
 * NT subsystems have been cognitively integrated.
 */
struct SubsystemCoverage {
    std::string subsystem;
    std::string bridge_header;
    float       type_coverage;      // 0..1 — fraction of types mapped
    float       function_coverage;  // 0..1 — fraction of functions bridged
    bool        has_tests;

    SubsystemCoverage()
        : type_coverage(0), function_coverage(0), has_tests(false) {}

    SubsystemCoverage(const std::string& sub, const std::string& bridge,
                      float tc, float fc, bool tests)
        : subsystem(sub), bridge_header(bridge)
        , type_coverage(tc), function_coverage(fc), has_tests(tests) {}

    float combined_coverage() const {
        return (type_coverage + function_coverage) / 2.0f;
    }
};

/* ================================================================== */
/* KSM Iteration Record                                                */
/* ================================================================== */

/**
 * KSMIterationRecord — Records the state at each KSM iteration.
 * This is the kernel's episodic memory of its own evolution.
 */
struct KSMIterationRecord {
    uint32_t    iteration;
    std::string focus;          // Which dimension was targeted
    std::string description;
    float       grip_before;
    float       grip_after;
    float       delta;
    std::string commit_hash;

    float improvement() const { return grip_after - grip_before; }
};

/* ================================================================== */
/* NTAutognosis — The Self-Awareness Engine                            */
/* ================================================================== */

/**
 * NTAutognosis — Implements the Autognosis hierarchy for the NT kernel.
 *
 * Maps NT self-monitoring to cognitive self-awareness:
 *   NT perf counters  → L0 Observation
 *   Coverage model    → L1 Self-Model
 *   Grip analysis     → L2 Meta-Cognition
 *   Hot-patching      → L3 Self-Modification (future)
 *   Kernel debugger   → L4 Recursive Self-Improve (future)
 */
class NTAutognosis {
public:
    NTAutognosis()
        : current_level_(cog::grip::AutonomyLevel::L0_Observation)
        , total_observations_(0)
    {
        // Initialize default performance counters
        register_counter("build.errors");
        register_counter("build.composability");
        register_counter("grip.product");
        register_counter("grip.composability");
        register_counter("grip.differentiability");
        register_counter("grip.executability");
        register_counter("grip.self_awareness");
        register_counter("grip.convergence");
        register_counter("ksm.iteration");
        register_counter("ksm.delta");

        // Initialize default subsystem coverage (L1 Self-Model)
        init_coverage_model();
    }

    /* --- L0: Observation --- */

    void register_counter(const std::string& name) {
        counters_[name] = PerfCounter(name);
    }

    void observe(const std::string& counter_name, double value) {
        auto it = counters_.find(counter_name);
        if (it != counters_.end()) {
            it->second.record(value);
        }
        total_observations_++;
        check_level_advancement();
    }

    void observe_build(const BuildHealth& health) {
        build_health_ = health;
        observe("build.errors", static_cast<double>(health.total_errors));
        observe("build.composability", health.composability_ratio());
    }

    void observe_grip(const cog::grip::GripDimensions& dims) {
        observe("grip.product", dims.product());
        observe("grip.composability", dims.composability);
        observe("grip.differentiability", dims.differentiability);
        observe("grip.executability", dims.executability);
        observe("grip.self_awareness", dims.self_awareness);
        observe("grip.convergence", dims.convergence);
    }

    const PerfCounter* counter(const std::string& name) const {
        auto it = counters_.find(name);
        return (it != counters_.end()) ? &it->second : nullptr;
    }

    /* --- L1: Self-Model --- */

    void update_subsystem_coverage(const std::string& subsystem,
                                    float type_cov, float func_cov,
                                    bool has_tests) {
        for (auto& sc : subsystem_coverage_) {
            if (sc.subsystem == subsystem) {
                sc.type_coverage = type_cov;
                sc.function_coverage = func_cov;
                sc.has_tests = has_tests;
                return;
            }
        }
    }

    float overall_integration_coverage() const {
        if (subsystem_coverage_.empty()) return 0.0f;
        float total = 0.0f;
        for (const auto& sc : subsystem_coverage_) {
            total += sc.combined_coverage();
        }
        return total / subsystem_coverage_.size();
    }

    const std::vector<SubsystemCoverage>& subsystem_coverage() const {
        return subsystem_coverage_;
    }

    /* --- L2: Meta-Cognition (Grip Analysis) --- */

    /**
     * Identify the weakest grip dimension — meta-cognitive reasoning
     * about the system's own cognitive state.
     */
    std::string weakest_dimension(const cog::grip::GripDimensions& dims) const {
        float min_val = dims.composability;
        std::string weakest = "composability";

        if (dims.differentiability < min_val) {
            min_val = dims.differentiability;
            weakest = "differentiability";
        }
        if (dims.executability < min_val) {
            min_val = dims.executability;
            weakest = "executability";
        }
        if (dims.self_awareness < min_val) {
            min_val = dims.self_awareness;
            weakest = "self_awareness";
        }
        if (dims.convergence < min_val) {
            min_val = dims.convergence;
            weakest = "convergence";
        }
        return weakest;
    }

    /**
     * Generate a self-assessment report — the kernel describing itself.
     */
    std::string self_report() const {
        std::ostringstream oss;
        oss << "=== NTAutognosis Self-Report ===\n"
            << "Level: " << cog::grip::autonomy_name(current_level_) << "\n"
            << "Observations: " << total_observations_ << "\n"
            << "Build: " << build_health_.to_string() << "\n"
            << "Integration coverage: "
            << (overall_integration_coverage() * 100.0f) << "%\n"
            << "KSM iterations: " << iterations_.size() << "\n";

        oss << "\nSubsystem Coverage:\n";
        for (const auto& sc : subsystem_coverage_) {
            oss << "  " << sc.subsystem << ": "
                << (sc.combined_coverage() * 100.0f) << "%"
                << (sc.has_tests ? " [tested]" : " [untested]")
                << " via " << sc.bridge_header << "\n";
        }

        oss << "\nPerformance Counters:\n";
        for (const auto& kv : counters_) {
            if (kv.second.sample_count > 0) {
                oss << "  " << kv.first << ": " << kv.second.value
                    << " (samples=" << kv.second.sample_count << ")\n";
            }
        }
        return oss.str();
    }

    /* --- KSM Evolution Memory --- */

    void record_iteration(const KSMIterationRecord& rec) {
        iterations_.push_back(rec);
        observe("ksm.iteration", static_cast<double>(rec.iteration));
        observe("ksm.delta", rec.delta);
    }

    const std::vector<KSMIterationRecord>& iterations() const {
        return iterations_;
    }

    /* --- Level Management --- */

    cog::grip::AutonomyLevel level() const { return current_level_; }

    /**
     * Compute the self_awareness dimension value (0..1).
     * L0 = 0.0, L1 = 0.25, L2 = 0.50, L3 = 0.75, L4 = 1.0
     *
     * But we also factor in observation density — L0 with many
     * observations is worth more than L0 with none.
     */
    float self_awareness_value() const {
        float base = static_cast<float>(
            static_cast<uint8_t>(current_level_)) / 4.0f;
        // Within-level bonus: up to 0.24 based on observation density
        float obs_bonus = std::min(0.24f,
            static_cast<float>(total_observations_) / 1000.0f * 0.24f);
        return std::min(1.0f, base + obs_bonus);
    }

    uint64_t total_observations() const { return total_observations_; }
    const BuildHealth& build_health() const { return build_health_; }

private:
    cog::grip::AutonomyLevel current_level_;
    uint64_t total_observations_;
    std::map<std::string, PerfCounter> counters_;
    std::vector<SubsystemCoverage> subsystem_coverage_;
    std::vector<KSMIterationRecord> iterations_;
    BuildHealth build_health_;

    void check_level_advancement() {
        // L0 → L1: After 10+ observations AND build health recorded
        if (current_level_ == cog::grip::AutonomyLevel::L0_Observation
            && total_observations_ >= 10
            && build_health_.total_source_files > 0) {
            current_level_ = cog::grip::AutonomyLevel::L1_SelfModel;
        }
        // L1 → L2: After 50+ observations AND coverage model populated
        if (current_level_ == cog::grip::AutonomyLevel::L1_SelfModel
            && total_observations_ >= 50
            && overall_integration_coverage() > 0.1f) {
            current_level_ = cog::grip::AutonomyLevel::L2_MetaCognition;
        }
    }

    void init_coverage_model() {
        // Initialize with known NT subsystems and their bridge status
        subsystem_coverage_ = {
            {"ke (Kernel)",       "nt_cognitive_kernel.hpp", 0.3f, 0.2f, true},
            {"mm (Memory)",       "(needed)",                0.0f, 0.0f, false},
            {"ps (Process)",      "nt_cognitive_kernel.hpp", 0.4f, 0.3f, true},
            {"ob (Object)",       "nt_cognitive_kernel.hpp", 0.3f, 0.2f, true},
            {"io (I/O)",          "(needed)",                0.0f, 0.0f, false},
            {"se (Security)",     "(needed)",                0.0f, 0.0f, false},
            {"ex (Executive)",    "nt_cognitive_kernel.hpp", 0.2f, 0.1f, true},
            {"lpc (Comms)",       "nt_cognitive_kernel.hpp", 0.3f, 0.2f, true},
            {"cc (Cache)",        "(needed)",                0.0f, 0.0f, false},
            {"hal (HAL)",         "nt_autognosis.hpp",       0.1f, 0.1f, true},
            {"config (Registry)", "(needed)",                0.0f, 0.0f, false},
            {"w32 (Win32)",       "(needed)",                0.0f, 0.0f, false},
        };
    }
};

} // namespace autognosis
} // namespace winkognn
