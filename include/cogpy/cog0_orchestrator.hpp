/*++
 * cog0_orchestrator.hpp — cog0 Agent-Zero Orchestration Integration
 *
 * Part of WinKoGNN Unified AGI-OS Build
 *--*/

#pragma once

#include "cogpy_bridge.hpp"
#include "nt_cognitive_kernel.hpp"
#include "atenspace_bridge.hpp"

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <queue>
#include <sstream>

namespace winkognn {
namespace orchestrator {

/* ================================================================== */
/* Orchestration Mode                                                  */
/* ================================================================== */
enum class OrcMode : uint8_t {
    REACTIVE=0, PROACTIVE=1, GOAL_DIRECTED=2, EXPLORATORY=3,
    BALANCED=4, AUTONOMOUS=5, COLLABORATIVE=6, SCRIPTED=7, NUM_MODES=8
};

inline const char* mode_name(OrcMode m) {
    static const char* names[] = {
        "REACTIVE","PROACTIVE","GOAL_DIRECTED","EXPLORATORY",
        "BALANCED","AUTONOMOUS","COLLABORATIVE","SCRIPTED"
    };
    return names[static_cast<int>(m)];
}

/* ================================================================== */
/* Goal / Task / Percept                                               */
/* ================================================================== */
struct Goal {
    uint32_t id; std::string name; std::string description;
    float priority; bool achieved; uint32_t parent_id;
    std::vector<uint32_t> sub_goals;
};

struct Task {
    uint32_t id; std::string name; std::string description;
    uint32_t goal_id; float priority; bool completed; std::string result;
};

struct Percept {
    enum class Modality : uint8_t { TEXT, NUMERIC, STRUCTURED, VISUAL, AUDITORY };
    Modality modality; std::string content; float salience; uint64_t timestamp;
};

/* ================================================================== */
/* Memory System                                                       */
/* ================================================================== */
class MemorySystem {
public:
    struct MemoryItem {
        std::string content; float importance; uint64_t timestamp;
        uint32_t access_count;
        enum class Type { WORKING, EPISODIC, LONG_TERM, PROCEDURAL } type;
    };

    void store(const std::string& content, float importance, MemoryItem::Type type) {
        items_.push_back({content, importance, next_timestamp_++, 0, type});
    }

    std::vector<MemoryItem> recall(float min_importance = 0.0f, size_t max_items = 10) {
        std::vector<MemoryItem> result;
        for (auto& item : items_) {
            if (item.importance >= min_importance) {
                item.access_count++;
                result.push_back(item);
                if (result.size() >= max_items) break;
            }
        }
        return result;
    }

    void decay(float rate = 0.01f) {
        for (auto& item : items_)
            if (item.type != MemoryItem::Type::LONG_TERM)
                item.importance *= (1.0f - rate);
    }

    size_t size() const { return items_.size(); }

private:
    std::vector<MemoryItem> items_;
    uint64_t next_timestamp_ = 0;
};

/* ================================================================== */
/* Tool Registry                                                       */
/* ================================================================== */
class ToolRegistry {
public:
    struct Tool {
        std::string name; std::string description;
        std::function<std::string(const std::string&)> handler; bool enabled;
    };

    void register_tool(const std::string& name, const std::string& desc,
                       std::function<std::string(const std::string&)> handler) {
        tools_[name] = {name, desc, handler, true};
    }

    std::string invoke(const std::string& name, const std::string& input) {
        auto it = tools_.find(name);
        if (it == tools_.end() || !it->second.enabled)
            return "ERROR: Tool not found: " + name;
        return it->second.handler(input);
    }

    std::vector<std::string> list_tools() const {
        std::vector<std::string> names;
        for (const auto& [name, tool] : tools_)
            if (tool.enabled) names.push_back(name);
        return names;
    }

    size_t tool_count() const { return tools_.size(); }

private:
    std::map<std::string, Tool> tools_;
};

/* ================================================================== */
/* Script Engine                                                       */
/* ================================================================== */
class ScriptEngine {
public:
    struct Command { std::string verb; std::vector<std::string> args; };

    static std::vector<Command> parse(const std::string& script) {
        std::vector<Command> commands;
        std::istringstream stream(script);
        std::string line;
        while (std::getline(stream, line)) {
            if (line.empty() || line[0] == '#') continue;
            size_t start = line.find_first_not_of(" \t");
            if (start == std::string::npos) continue;
            line = line.substr(start);
            Command cmd;
            std::istringstream ls(line);
            ls >> cmd.verb;
            std::string arg;
            while (ls >> arg) cmd.args.push_back(arg);
            commands.push_back(cmd);
        }
        return commands;
    }
};

/* ================================================================== */
/* Cog0Orchestrator — Main orchestration engine                        */
/* ================================================================== */
class Cog0Orchestrator {
public:
    struct Config {
        OrcMode default_mode; size_t reservoir_size;
        size_t max_goals; size_t max_tasks; size_t max_agents;
        float memory_decay_rate;
    };

    explicit Cog0Orchestrator(Config config = default_config())
        : config_(config), mode_(config.default_mode)
        , bridge_(config.reservoir_size), kernel_()
        , memory_(), tools_()
        , next_goal_id_(1), next_task_id_(1), cycle_count_(0)
    {
        register_builtin_tools();
        auto& proc = kernel_.create_process("cog0-orchestrator");
        proc.set_attention(1.0f, 1.0f, 1.0f);
        proc.activate();

        auto self = bridge_.add_node(cog::AtomType::CONCEPT_NODE, "cog0-orchestrator");
        auto mode_node = bridge_.add_node(cog::AtomType::CONCEPT_NODE, mode_name(mode_));
        auto mode_pred = bridge_.add_node(cog::AtomType::PREDICATE_NODE, "operating-mode");
        bridge_.add_link(cog::AtomType::EVALUATION_LINK, {mode_pred, self, mode_node});
    }

    OrcMode mode() const { return mode_; }
    void set_mode(OrcMode m) {
        mode_ = m;
        bridge_.add_node(cog::AtomType::CONCEPT_NODE, mode_name(m));
    }

    uint32_t add_goal(const std::string& name, const std::string& desc,
                      float priority = 0.5f, uint32_t parent = 0) {
        Goal g; g.id = next_goal_id_++; g.name = name; g.description = desc;
        g.priority = priority; g.achieved = false; g.parent_id = parent;
        goals_[g.id] = g;
        bridge_.add_node(cog::AtomType::CONCEPT_NODE, "goal:" + name);
        if (parent > 0) {
            auto it = goals_.find(parent);
            if (it != goals_.end()) it->second.sub_goals.push_back(g.id);
        }
        return g.id;
    }

    uint32_t add_task(const std::string& name, const std::string& desc,
                      uint32_t goal_id = 0, float priority = 0.5f) {
        Task t; t.id = next_task_id_++; t.name = name; t.description = desc;
        t.goal_id = goal_id; t.priority = priority; t.completed = false;
        tasks_[t.id] = t;
        return t.id;
    }

    void perceive(const Percept& percept) {
        memory_.store(percept.content, percept.salience,
                      MemorySystem::MemoryItem::Type::WORKING);
        bridge_.add_node(cog::AtomType::CONCEPT_NODE, percept.content,
                         percept.salience, 0.8f);
        std::vector<float> input(bridge_.reservoir().state().size(), 0.0f);
        for (size_t i = 0; i < percept.content.size() && i < input.size(); ++i)
            input[i] = (float)percept.content[i] / 255.0f;
        bridge_.reservoir().observe(input);
    }

    void perceive_text(const std::string& text, float salience = 0.5f) {
        perceive({Percept::Modality::TEXT, text, salience, cycle_count_});
    }

    void step() {
        bridge_.step();
        kernel_.run_cycle();
        memory_.decay(config_.memory_decay_rate);
        switch (mode_) {
            case OrcMode::PROACTIVE: process_next_task(); break;
            case OrcMode::GOAL_DIRECTED: advance_top_goal(); break;
            case OrcMode::EXPLORATORY: explore(); break;
            case OrcMode::AUTONOMOUS:
                process_next_task(); advance_top_goal(); explore(); break;
            default: break;
        }
        cycle_count_++;
    }

    void run(size_t n_cycles) { for (size_t i = 0; i < n_cycles; ++i) step(); }

    std::string execute_script(const std::string& script) {
        auto commands = ScriptEngine::parse(script);
        std::ostringstream output;
        for (const auto& cmd : commands) {
            if (cmd.verb == "goal" && !cmd.args.empty()) {
                std::string desc = cmd.args.size() > 1 ? cmd.args[1] : "";
                uint32_t id = add_goal(cmd.args[0], desc);
                output << "Goal " << id << ": " << cmd.args[0] << "\n";
            } else if (cmd.verb == "task" && !cmd.args.empty()) {
                std::string desc = cmd.args.size() > 1 ? cmd.args[1] : "";
                uint32_t id = add_task(cmd.args[0], desc);
                output << "Task " << id << ": " << cmd.args[0] << "\n";
            } else if (cmd.verb == "percept" && !cmd.args.empty()) {
                std::string text; for (const auto& a : cmd.args) text += a + " ";
                perceive_text(text);
                output << "Percept: " << text << "\n";
            } else if (cmd.verb == "run") {
                size_t n = cmd.args.empty() ? 1 : std::stoul(cmd.args[0]);
                run(n); output << "Ran " << n << " cycles\n";
            } else if (cmd.verb == "mode" && !cmd.args.empty()) {
                for (int i = 0; i < (int)OrcMode::NUM_MODES; ++i)
                    if (cmd.args[0] == mode_name((OrcMode)i))
                        { set_mode((OrcMode)i); output << "Mode: " << cmd.args[0] << "\n"; break; }
            } else if (cmd.verb == "status") { output << status_string();
            } else if (cmd.verb == "atoms") { output << "Atoms: " << bridge_.atom_count() << "\n";
            } else if (cmd.verb == "goals") {
                for (const auto& [id, g] : goals_)
                    output << "  [" << id << "] " << g.name << " (p=" << g.priority
                           << ", done=" << (g.achieved?"y":"n") << ")\n";
            } else if (cmd.verb == "infer") { bridge_.step(); output << "Inference pass complete\n";
            } else if (cmd.verb == "tool" && cmd.args.size() >= 2) {
                output << tools_.invoke(cmd.args[0], cmd.args[1]) << "\n";
            } else if (cmd.verb == "help") {
                output << "Commands: goal, task, percept, run, mode, status, atoms, goals, infer, tool, help, quit\n";
            } else if (cmd.verb == "quit") { break;
            } else { output << "Unknown: " << cmd.verb << "\n"; }
        }
        return output.str();
    }

    struct Status {
        OrcMode mode; size_t cycles; size_t atoms; size_t goals; size_t tasks;
        size_t processes; size_t memory_items; size_t tools;
        size_t ecan_cycles; size_t reservoir_steps; float grip_product;
    };

    Status status() {
        auto grip = bridge_.grip().compute_grip();
        return { mode_, cycle_count_, bridge_.atom_count(), goals_.size(), tasks_.size(),
                 kernel_.process_count(), memory_.size(), tools_.tool_count(),
                 bridge_.ecan_cycles(), bridge_.reservoir_steps(), grip.product() };
    }

    std::string status_string() {
        auto s = status();
        std::ostringstream out;
        out << "=== cog0 Orchestrator Status ===\n"
            << "Mode:            " << mode_name(s.mode) << "\n"
            << "Cycles:          " << s.cycles << "\n"
            << "Atoms:           " << s.atoms << "\n"
            << "Goals:           " << s.goals << "\n"
            << "Tasks:           " << s.tasks << "\n"
            << "Processes:       " << s.processes << "\n"
            << "Memory items:    " << s.memory_items << "\n"
            << "Tools:           " << s.tools << "\n"
            << "ECAN cycles:     " << s.ecan_cycles << "\n"
            << "Reservoir steps: " << s.reservoir_steps << "\n"
            << "Grip product:    " << s.grip_product << "\n";
        return out.str();
    }

    atenspace_bridge::ATenSpaceBridge& bridge() { return bridge_; }
    nt_cognitive::NTCognitiveKernel& kernel() { return kernel_; }
    MemorySystem& memory() { return memory_; }
    ToolRegistry& tools() { return tools_; }

    static Config default_config() {
        return { OrcMode::BALANCED, 64, 100, 1000, 50, 0.01f };
    }

private:
    void register_builtin_tools() {
        tools_.register_tool("echo", "Echo input",
            [](const std::string& input) { return input; });
        tools_.register_tool("atom_count", "Get atom count",
            [this](const std::string&) { return std::to_string(bridge_.atom_count()); });
        tools_.register_tool("grip", "Get grip product",
            [this](const std::string&) {
                return std::to_string(bridge_.grip().compute_grip().product()); });
        tools_.register_tool("reservoir_state", "Get reservoir summary",
            [this](const std::string&) {
                auto& st = bridge_.reservoir().state();
                float sum = 0; for (auto v : st) sum += v*v;
                return "energy=" + std::to_string(sum) +
                       " steps=" + std::to_string(bridge_.reservoir_steps()); });
    }

    void process_next_task() {
        Task* best = nullptr;
        for (auto& [id, task] : tasks_)
            if (!task.completed && (!best || task.priority > best->priority)) best = &task;
        if (best) {
            best->completed = true;
            best->result = "completed at cycle " + std::to_string(cycle_count_);
            memory_.store("Completed: " + best->name, 0.7f,
                          MemorySystem::MemoryItem::Type::EPISODIC);
        }
    }

    void advance_top_goal() {
        /* Stub: in full implementation, check sub-goal completion */
    }

    void explore() {
        auto& state = bridge_.reservoir().state();
        float energy = 0; for (auto v : state) energy += v*v;
        if (energy > 0.1f)
            bridge_.add_node(cog::AtomType::CONCEPT_NODE,
                             "exploration-" + std::to_string(cycle_count_),
                             energy / (float)state.size(), 0.5f);
    }

    Config config_;
    OrcMode mode_;
    atenspace_bridge::ATenSpaceBridge bridge_;
    nt_cognitive::NTCognitiveKernel kernel_;
    MemorySystem memory_;
    ToolRegistry tools_;
    std::map<uint32_t, Goal> goals_;
    std::map<uint32_t, Task> tasks_;
    uint32_t next_goal_id_;
    uint32_t next_task_id_;
    size_t cycle_count_;
};

} // namespace orchestrator
} // namespace winkognn
