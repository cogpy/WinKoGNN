/*++
 * atenspace_bridge.hpp — ATenSpace ↔ cogpy-hpp Module Integration
 *
 * Part of WinKoGNN Unified AGI-OS Build
 *--*/

#pragma once

#include <cog/core/core.hpp>
#include <cog/prime/prime.hpp>
#include <cog/gml/gml.hpp>
#include <cog/pilot/pilot.hpp>
#include <cog/grip/grip.hpp>
#include <cog/glow/glow.hpp>
#include <cog/plan9/plan9.hpp>
#include <cog/webvm/webvm.hpp>
#include <cog/mach/mach.hpp>

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <cmath>
#include <sstream>
#include <algorithm>

namespace winkognn {
namespace atenspace_bridge {

/* ================================================================== */
/* Truth Value Adapter                                                 */
/* ================================================================== */
class TruthValueAdapter {
public:
    enum class TVType { SIMPLE, COUNT, INDEFINITE, FUZZY };

    struct ExtendedTV {
        TVType type; float strength; float confidence;
        float count; float lower; float upper;
    };

    static ExtendedTV from_cog(const cog::TruthValue& tv) {
        return { TVType::SIMPLE, tv.strength, tv.confidence, 0, 0, 0 };
    }

    static cog::TruthValue to_cog(const ExtendedTV& etv) {
        return cog::TruthValue(etv.strength, etv.confidence);
    }

    static ExtendedTV revision(const ExtendedTV& a, const ExtendedTV& b) {
        float ca = confidence_to_count(a.confidence);
        float cb = confidence_to_count(b.confidence);
        float total = ca + cb;
        float s = (a.strength * ca + b.strength * cb) / total;
        float c = count_to_confidence(total);
        return { TVType::SIMPLE, s, c, total, 0, 0 };
    }

    static ExtendedTV deduction(const ExtendedTV& ab, const ExtendedTV& bc) {
        float sAB = ab.strength, sBC = bc.strength;
        float sC = 0.5f;
        float sAC = sAB * sBC + (1.0f - sAB) * (sC - sAB * sBC) / (1.0f - sAB + 1e-10f);
        sAC = std::max(0.0f, std::min(1.0f, sAC));
        return { TVType::SIMPLE, sAC, ab.confidence * bc.confidence * 0.9f, 0, 0, 0 };
    }

    static ExtendedTV induction(const ExtendedTV& ab, const ExtendedTV& cb) {
        return { TVType::SIMPLE, ab.strength * cb.strength,
                 ab.confidence * cb.confidence * 0.8f, 0, 0, 0 };
    }

    static ExtendedTV abduction(const ExtendedTV& ab, const ExtendedTV& cb) {
        float sAC = ab.strength * cb.strength / 0.5f;
        sAC = std::max(0.0f, std::min(1.0f, sAC));
        return { TVType::SIMPLE, sAC, ab.confidence * cb.confidence * 0.7f, 0, 0, 0 };
    }

private:
    static float confidence_to_count(float c, float k = 800.0f) {
        return k * c / (1.0f - c + 1e-10f);
    }
    static float count_to_confidence(float n, float k = 800.0f) {
        return n / (n + k);
    }
};

/* ================================================================== */
/* ECAN Adapter                                                        */
/* ================================================================== */
class ECANAdapter {
public:
    struct AttentionValue { float sti; float lti; float vlti; };
    struct ECANConfig {
        float rent_rate; float wage_rate; float spreading_rate;
        float focus_boundary; size_t max_af_size;
    };

    explicit ECANAdapter(ECANConfig config = default_config())
        : config_(config), total_sti_(0.0f), cycle_(0) {}

    void set_attention(cog::Handle h, float sti, float lti, float vlti) {
        attention_[h] = {sti, lti, vlti};
        total_sti_ += sti;
    }

    AttentionValue get_attention(cog::Handle h) const {
        auto it = attention_.find(h);
        return (it != attention_.end()) ? it->second : AttentionValue{0,0,0};
    }

    void cycle(const cog::AtomSpace& space) {
        for (auto& [h, av] : attention_) av.sti *= (1.0f - config_.rent_rate);
        for (auto& [h, av] : attention_) {
            if (av.sti > config_.focus_boundary) {
                const cog::Atom* atom = space.get_atom(h);
                if (atom && atom->is_link()) {
                    float spread = av.sti * config_.spreading_rate;
                    for (auto oh : atom->outgoing) {
                        auto oit = attention_.find(oh);
                        if (oit != attention_.end())
                            oit->second.sti += spread / (float)atom->outgoing.size();
                    }
                }
            }
        }
        cycle_++;
    }

    std::vector<cog::Handle> attentional_focus() const {
        std::vector<cog::Handle> af;
        for (const auto& [h, av] : attention_)
            if (av.sti > config_.focus_boundary) af.push_back(h);
        return af;
    }

    size_t cycle_count() const { return cycle_; }
    float total_sti() const { return total_sti_; }

    static ECANConfig default_config() { return { 0.01f, 0.1f, 0.05f, 0.5f, 100 }; }

private:
    ECANConfig config_;
    std::map<cog::Handle, AttentionValue> attention_;
    float total_sti_;
    size_t cycle_;
};

/* ================================================================== */
/* Reservoir Bridge                                                    */
/* ================================================================== */
class ReservoirBridge {
public:
    struct ReservoirConfig {
        size_t reservoir_size; float spectral_radius; float input_scaling;
        float leak_rate; float regularization;
    };

    explicit ReservoirBridge(ReservoirConfig config = default_config())
        : config_(config), state_(config.reservoir_size, 0.0f), step_count_(0) {}

    void observe(const std::vector<float>& input) {
        std::vector<float> new_state(config_.reservoir_size, 0.0f);
        for (size_t i = 0; i < config_.reservoir_size; ++i) {
            float activation = 0.0f;
            for (size_t j = 0; j < config_.reservoir_size; ++j)
                activation += pseudo_weight(i, j) * state_[j];
            for (size_t k = 0; k < input.size() && k < config_.reservoir_size; ++k)
                activation += config_.input_scaling * input[k];
            new_state[i] = (1.0f - config_.leak_rate) * state_[i]
                         + config_.leak_rate * std::tanh(activation);
        }
        state_ = new_state;
        step_count_++;
    }

    const std::vector<float>& state() const { return state_; }
    size_t step_count() const { return step_count_; }

    static ReservoirConfig default_config() { return { 64, 0.9f, 0.1f, 0.3f, 1e-6f }; }

private:
    float pseudo_weight(size_t i, size_t j) const {
        uint32_t seed = (uint32_t)(i * 65537 + j * 2654435761u);
        seed ^= seed >> 16; seed *= 0x45d9f3b; seed ^= seed >> 16;
        float w = ((float)(seed & 0xFFFF) / 32768.0f) - 1.0f;
        return w * config_.spectral_radius / std::sqrt((float)config_.reservoir_size);
    }

    ReservoirConfig config_;
    std::vector<float> state_;
    size_t step_count_;
};

/* ================================================================== */
/* Cognitive 9P Filesystem                                             */
/* ================================================================== */
class Cognitive9PFS {
public:
    struct FSEntry { std::string path; std::string content; bool is_dir; };

    std::vector<FSEntry> generate_fs(
        const cog::AtomSpace& space,
        const ECANAdapter& ecan,
        const ReservoirBridge& reservoir,
        cog::grip::CognitiveGrip& grip) const
    {
        std::vector<FSEntry> entries;
        entries.push_back({"/", "", true});
        entries.push_back({"/atoms", "", true});
        entries.push_back({"/types", "", true});
        entries.push_back({"/attention", "", true});
        entries.push_back({"/inference", "", true});
        entries.push_back({"/reservoir", "", true});
        entries.push_back({"/grip", "", true});

        for (size_t i = 1; i <= space.size(); ++i) {
            cog::Handle h = (cog::Handle)i;
            const cog::Atom* atom = space.get_atom(h);
            if (atom) {
                std::string content = atom->name + "\n";
                content += "type: " + std::string(cog::atom_type_name(atom->type)) + "\n";
                content += "tv: " + atom->tv.to_string() + "\n";
                auto av = ecan.get_attention(h);
                content += "sti: " + std::to_string(av.sti) + "\n";
                entries.push_back({"/atoms/" + std::to_string(h), content, false});
            }
        }

        {
            std::string content = "step: " + std::to_string(reservoir.step_count()) + "\n";
            content += "size: " + std::to_string(reservoir.state().size()) + "\n";
            entries.push_back({"/reservoir/state", content, false});
        }

        {
            auto dims = grip.compute_grip();
            std::string content;
            content += "composability: " + std::to_string(dims.composability) + "\n";
            content += "differentiability: " + std::to_string(dims.differentiability) + "\n";
            content += "executability: " + std::to_string(dims.executability) + "\n";
            content += "self_awareness: " + std::to_string(dims.self_awareness) + "\n";
            content += "convergence: " + std::to_string(dims.convergence) + "\n";
            content += "product: " + std::to_string(dims.product()) + "\n";
            entries.push_back({"/grip/dimensions", content, false});
        }

        return entries;
    }
};

/* ================================================================== */
/* ATenSpaceBridge — Top-level integration object                      */
/* ================================================================== */
class ATenSpaceBridge {
public:
    explicit ATenSpaceBridge(size_t reservoir_size = 64)
        : space_(), ecan_()
        , reservoir_(ReservoirBridge::ReservoirConfig{reservoir_size, 0.9f, 0.1f, 0.3f, 1e-6f})
        , grip_(reservoir_size), fs_()
    {
        grip_.set_composability(0.5f);
        grip_.update_differentiability(0.5f, 0.5f);
    }

    cog::Handle add_node(cog::AtomType type, const std::string& name,
                         float strength = 1.0f, float confidence = 1.0f) {
        auto h = space_.add_node(type, name, cog::TruthValue(strength, confidence));
        ecan_.set_attention(h, 0.5f, 0.5f, 0.5f);
        return h;
    }

    cog::Handle add_link(cog::AtomType type,
                         const std::vector<cog::Handle>& outgoing,
                         float strength = 1.0f, float confidence = 1.0f) {
        auto h = space_.add_link(type, outgoing, cog::TruthValue(strength, confidence));
        ecan_.set_attention(h, 0.5f, 0.3f, 0.3f);
        return h;
    }

    void step() {
        ecan_.cycle(space_);
        auto af = ecan_.attentional_focus();
        std::vector<float> obs(reservoir_.state().size(), 0.0f);
        for (size_t i = 0; i < af.size() && i < obs.size(); ++i) {
            auto av = ecan_.get_attention(af[i]);
            obs[i] = av.sti;
        }
        reservoir_.observe(obs);
        grip_.workflow().advance();
        grip_.evolution_step();
    }

    cog::AtomSpace& space() { return space_; }
    ECANAdapter& ecan() { return ecan_; }
    ReservoirBridge& reservoir() { return reservoir_; }
    cog::grip::CognitiveGrip& grip() { return grip_; }

    std::vector<Cognitive9PFS::FSEntry> snapshot() {
        return fs_.generate_fs(space_, ecan_, reservoir_, grip_);
    }

    size_t atom_count() const { return space_.size(); }
    size_t ecan_cycles() const { return ecan_.cycle_count(); }
    size_t reservoir_steps() const { return reservoir_.step_count(); }

private:
    cog::AtomSpace space_;
    ECANAdapter ecan_;
    ReservoirBridge reservoir_;
    cog::grip::CognitiveGrip grip_;
    Cognitive9PFS fs_;
};

} // namespace atenspace_bridge
} // namespace winkognn
