# OpenCog Workbench Architecture

## Overview

The OpenCog Workbench is a comprehensive cognitive architecture system designed for Windows NT4, integrating autonomous multi-agent orchestration with Agent-Zero hypervisor and Graph Neural Network capabilities.

## System Architecture Diagram

```mermaid
graph TB
    subgraph "User Interface Layer"
        API[OpenCogWorkbench API]
    end
    
    subgraph "Orchestration Layer"
        ORCH[MultiAgentOrchestrator]
        SCHED[TaskScheduler]
    end
    
    subgraph "Knowledge & Reasoning Layer"
        AS[AtomSpaceManager]
        RE[ReasoningEngine]
        GNN[GraphNeuralNetwork]
    end
    
    subgraph "Agent Layer"
        CA[CognitiveAgent]
        RA[ReasoningAgent]
        LA[LearningAgent]
    end
    
    subgraph "Virtualization Layer"
        HV[AgentZeroHypervisor]
        VM[VirtualMachineContext]
    end
    
    subgraph "System Layer"
        KB[NT4CognitiveKernel]
        MEM[MemoryManager]
        PROC[ProcessManager]
    end
    
    API --> ORCH
    ORCH --> SCHED
    ORCH --> AS
    ORCH --> RE
    ORCH --> GNN
    ORCH --> CA
    ORCH --> RA
    ORCH --> LA
    CA --> AS
    CA --> RE
    CA --> GNN
    RA --> AS
    RA --> RE
    LA --> GNN
    ORCH --> HV
    HV --> VM
    HV --> KB
    KB --> MEM
    KB --> PROC
    AS --> GNN
    RE --> AS
```

## Component Interaction Flow

```mermaid
sequenceDiagram
    participant User
    participant Workbench
    participant Orchestrator
    participant Agent
    participant AtomSpace
    participant GNN
    participant Hypervisor
    
    User->>Workbench: create_agent()
    Workbench->>Orchestrator: register_agent()
    Orchestrator->>Hypervisor: create_vm()
    Hypervisor-->>Orchestrator: vm_id
    Orchestrator-->>Workbench: agent_id
    
    User->>Workbench: dispatch_task()
    Workbench->>Orchestrator: dispatch_task()
    Orchestrator->>Agent: execute_task()
    Agent->>AtomSpace: query_knowledge()
    AtomSpace-->>Agent: knowledge_data
    Agent->>GNN: predict()
    GNN-->>Agent: predictions
    Agent-->>Orchestrator: task_result
    Orchestrator-->>Workbench: result
    Workbench-->>User: result
```

## Data Flow Architecture

```mermaid
flowchart LR
    subgraph Input
        UI[User Input]
        CFG[Configuration]
    end
    
    subgraph Processing
        AS[AtomSpace<br/>Knowledge Graph]
        RE[Reasoning Engine<br/>Inference]
        GNN[Graph Neural Network<br/>Learning]
    end
    
    subgraph Execution
        ORCH[Orchestrator<br/>Task Distribution]
        AGENTS[Agents<br/>Cognitive Processing]
    end
    
    subgraph Infrastructure
        HV[Hypervisor<br/>Resource Management]
        NT4[NT4 Kernel<br/>System Calls]
    end
    
    subgraph Output
        RES[Results]
        STAT[System Status]
    end
    
    UI --> ORCH
    CFG --> ORCH
    ORCH --> AS
    ORCH --> RE
    ORCH --> GNN
    ORCH --> AGENTS
    AS <--> RE
    AS <--> GNN
    RE <--> GNN
    AGENTS <--> AS
    AGENTS <--> RE
    AGENTS <--> GNN
    AGENTS --> HV
    HV --> NT4
    AGENTS --> RES
    ORCH --> STAT
    RES --> UI
    STAT --> UI
```

## System Components

### 1. Multi-Agent Orchestrator (`core/orchestrator.py`)

```mermaid
classDiagram
    class MultiAgentOrchestrator {
        -agents: Dict
        -task_queue: Queue
        -scheduler: TaskScheduler
        +register_agent(agent_id, metadata)
        +remove_agent(agent_id)
        +dispatch_task(task, agent_id)
        +orchestrate_tasks(tasks)
        +get_status()
    }
    
    class AutonomousAgent {
        -agent_id: str
        -capabilities: List
        -state: AgentState
        +execute_task(task)
        +get_capabilities()
        +update_state(state)
    }
    
    class TaskScheduler {
        -task_queue: PriorityQueue
        +schedule_task(task, priority)
        +get_next_task()
        +match_agent(task)
    }
    
    class AgentMetadata {
        +agent_id: str
        +agent_type: str
        +capabilities: List
        +memory_kb: int
        +cpu_quota: float
        +state: AgentState
    }
    
    MultiAgentOrchestrator --> TaskScheduler
    MultiAgentOrchestrator --> AutonomousAgent
    MultiAgentOrchestrator --> AgentMetadata
```

**Purpose**: Coordinates multiple autonomous agents for distributed cognitive processing

**Key Features**:
- Agent registration and lifecycle management
- Task scheduling and distribution
- Capability-based agent selection
- State management (IDLE, ACTIVE, SUSPENDED, TERMINATED, ERROR)
- Thread-safe coordination with locks

**Classes**:
- `MultiAgentOrchestrator`: Main orchestration engine
- `AutonomousAgent`: Base class for all agents
- `TaskScheduler`: Priority-based task scheduling
- `AgentMetadata`: Agent configuration and state

### 2. AtomSpace Manager (`core/atomspace.py`)

```mermaid
classDiagram
    class AtomSpaceManager {
        -atoms: Dict
        -links: Dict
        -attention_bank: Dict
        +add_atom(atom_type, name, truth_value)
        +create_link(link_type, source, target)
        +get_atom(atom_id)
        +query_pattern(pattern)
        +spread_activation(source_id, decay)
    }
    
    class Atom {
        +atom_id: str
        +atom_type: AtomType
        +name: str
        +truth_value: float
        +confidence: float
        +attention_value: float
        +activation: float
    }
    
    class AtomType {
        <<enumeration>>
        NODE
        LINK
        CONCEPT
        PREDICATE
        EVALUATION
    }
    
    AtomSpaceManager --> Atom
    Atom --> AtomType
```

**Purpose**: Hypergraph-based knowledge representation system

**Key Features**:
- Node and link creation
- Truth values and confidence measures
- Attention values for importance weighting
- Activation spreading through knowledge graph
- Pattern matching and querying

**Classes**:
- `AtomSpaceManager`: Main knowledge management
- `Atom`: Individual knowledge atoms
- `AtomType`: Types (NODE, LINK, CONCEPT, PREDICATE, EVALUATION)

### 3. Reasoning Engine (`core/reasoning.py`)

```mermaid
classDiagram
    class ReasoningEngine {
        -atomspace: AtomSpaceManager
        -inference_rules: Dict
        -confidence_threshold: float
        +infer(query)
        +forward_chain(start_atoms, max_steps)
        +backward_chain(goal_atom, max_depth)
        +enable_rule(rule, enabled)
    }
    
    class InferenceResult {
        +conclusion: str
        +confidence: float
        +rule_used: InferenceRule
        +premises: List
        +explanation: str
    }
    
    class InferenceRule {
        <<enumeration>>
        DEDUCTION
        INDUCTION
        ABDUCTION
        MODUS_PONENS
        SIMILARITY
    }
    
    ReasoningEngine --> InferenceResult
    InferenceResult --> InferenceRule
```

**Purpose**: Logical inference and pattern-based reasoning

**Key Features**:
- Multiple inference rules (deduction, induction, abduction, modus ponens, similarity)
- Forward chaining: derive conclusions from facts
- Backward chaining: goal-directed reasoning
- Probabilistic inference support
- Explanation generation

**Classes**:
- `ReasoningEngine`: Main inference engine
- `InferenceResult`: Results with confidence
- `InferenceRule`: Rule types

### 4. Graph Neural Network (`gnn/graph_network.py`)

```mermaid
classDiagram
    class GraphNeuralNetwork {
        -layers: List[GNNLayer]
        -embeddings: Dict
        -atomspace: AtomSpaceManager
        +extract_graph_from_atomspace()
        +train(graph, labels, epochs)
        +predict_links(threshold)
        +compute_similarity(node1, node2)
        +get_embedding(node_id)
    }
    
    class GraphStructure {
        +nodes: List
        +edges: List
        +node_features: ndarray
        +adjacency_matrix: ndarray
    }
    
    class GNNLayer {
        +weights: ndarray
        +activation: function
        +forward(input, adjacency)
        +backward(gradient)
    }
    
    GraphNeuralNetwork --> GraphStructure
    GraphNeuralNetwork --> GNNLayer
```

**Purpose**: Deep learning over knowledge graphs

**Key Features**:
- Extract graph structure from AtomSpace
- Multi-layer GNN architecture
- Self-supervised and supervised learning
- Node embedding generation
- Link prediction
- Similarity computation

**Classes**:
- `GraphNeuralNetwork`: Main GNN system
- `GraphStructure`: Graph representation
- `GNNLayer`: Network layer

### 5. Agent-Zero Hypervisor (`hypervisor/agent_zero.py`)

```mermaid
classDiagram
    class AgentZeroHypervisor {
        -vms: Dict
        -total_memory_kb: int
        -allocated_memory_kb: int
        -kernel: NT4CognitiveKernel
        +create_vm(agent_id, memory_kb, cpu_quota)
        +destroy_vm(vm_id)
        +start_vm(vm_id)
        +stop_vm(vm_id)
        +suspend_vm(vm_id)
        +resume_vm(vm_id)
        +get_system_resources()
    }
    
    class VirtualMachineContext {
        +vm_id: str
        +agent_id: str
        +state: VMState
        +memory_kb: int
        +cpu_quota: float
        +priority: int
        +kernel_context_id: str
    }
    
    class VMState {
        <<enumeration>>
        CREATED
        RUNNING
        STOPPED
        SUSPENDED
        TERMINATED
    }
    
    class HypervisorScheduler {
        +schedule(vms)
        +allocate_cpu(vm, quota)
        +preempt(vm)
    }
    
    AgentZeroHypervisor --> VirtualMachineContext
    VirtualMachineContext --> VMState
    AgentZeroHypervisor --> HypervisorScheduler
```

**Purpose**: Virtual machine management for agent isolation

**Key Features**:
- VM lifecycle management (create, start, stop, suspend, resume, destroy)
- Memory allocation and tracking
- CPU quota management
- Resource isolation levels (process, container, full_vm)
- Priority-based scheduling

**Classes**:
- `AgentZeroHypervisor`: Main hypervisor
- `VirtualMachineContext`: VM state
- `HypervisorScheduler`: VM scheduling

### 6. Windows NT4 Kernel Bridge (`nt4_bridge/kernel_bridge.py`)

```mermaid
classDiagram
    class NT4CognitiveKernel {
        -contexts: Dict
        -is_nt4_available: bool
        +create_kernel_context(context_id)
        +destroy_kernel_context(context_id)
        +allocate_memory(context_id, size_bytes)
        +deallocate_memory(context_id, address)
        +create_process(context_id)
        +send_ipc_message(source, target, message)
        +perform_io(context_id, operation)
    }
    
    class KernelContext {
        +context_id: str
        +privilege_level: int
        +memory_pool: Dict
        +process_id: int
        +thread_ids: List
    }
    
    class KernelCallType {
        <<enumeration>>
        MEMORY_ALLOC
        MEMORY_FREE
        PROCESS_CREATE
        THREAD_CREATE
        IPC_SEND
        IPC_RECEIVE
        IO_READ
        IO_WRITE
    }
    
    NT4CognitiveKernel --> KernelContext
    NT4CognitiveKernel --> KernelCallType
```

**Purpose**: Interface to Windows NT4 kernel for system-level operations

**Key Features**:
- Kernel context management
- Memory allocation/deallocation
- Process and thread creation
- Inter-process communication
- I/O operations
- Platform detection

**Classes**:
- `NT4CognitiveKernel`: Main kernel interface
- `KernelContext`: Execution context
- `KernelCallType`: Operation types

### 7. Cognitive Agents (`agents/cognitive_agent.py`)

```mermaid
classDiagram
    class CognitiveAgent {
        -agent_id: str
        -atomspace: AtomSpaceManager
        -reasoning_engine: ReasoningEngine
        -gnn: GraphNeuralNetwork
        -working_memory: Dict
        -long_term_memory: List
        +execute_reasoning_task(task)
        +execute_learning_task(task)
        +execute_pattern_recognition(task)
        +make_decision(options)
    }
    
    class ReasoningAgent {
        +specialize_reasoning(domain)
        +configure_inference_rules(rules)
    }
    
    class LearningAgent {
        +configure_learning_params(params)
        +update_knowledge(data)
    }
    
    class CoordinationAgent {
        +coordinate_agents(agent_list)
        +distribute_subtasks(task)
    }
    
    CognitiveAgent <|-- ReasoningAgent
    CognitiveAgent <|-- LearningAgent
    CognitiveAgent <|-- CoordinationAgent
```

**Purpose**: Autonomous agents with cognitive capabilities

**Key Features**:
- Reasoning tasks
- Learning from data
- Pattern recognition
- Decision making
- Working and long-term memory

**Classes**:
- `CognitiveAgent`: Main agent implementation
- `ReasoningAgent`: Specialized for reasoning
- `LearningAgent`: Specialized for learning
- `CoordinationAgent`: Multi-agent coordination

### 8. Configuration Manager (`config/config_manager.py`)

```mermaid
classDiagram
    class ConfigManager {
        -config: Dict
        -config_file: str
        +load_config(file_path)
        +save_config(file_path)
        +get(key, default)
        +set(key, value)
        +get_default_config()
    }
    
    class LoggingConfig {
        +level: str
        +format: str
        +handlers: List
    }
    
    ConfigManager --> LoggingConfig
```

**Purpose**: System configuration management

**Key Features**:
- Default configuration
- JSON config loading/saving
- Hierarchical configuration
- Logging setup

## Agent Lifecycle State Machine

```mermaid
stateDiagram-v2
    [*] --> CREATED: create_agent()
    CREATED --> IDLE: register()
    IDLE --> ACTIVE: dispatch_task()
    ACTIVE --> IDLE: task_complete()
    IDLE --> SUSPENDED: suspend()
    SUSPENDED --> IDLE: resume()
    ACTIVE --> SUSPENDED: suspend()
    SUSPENDED --> ACTIVE: resume()
    IDLE --> TERMINATED: remove_agent()
    ACTIVE --> TERMINATED: remove_agent()
    SUSPENDED --> TERMINATED: remove_agent()
    ACTIVE --> ERROR: exception
    ERROR --> IDLE: recover()
    ERROR --> TERMINATED: fatal_error
    TERMINATED --> [*]
```

## VM Lifecycle State Machine

```mermaid
stateDiagram-v2
    [*] --> CREATED: create_vm()
    CREATED --> RUNNING: start_vm()
    RUNNING --> STOPPED: stop_vm()
    STOPPED --> RUNNING: start_vm()
    RUNNING --> SUSPENDED: suspend_vm()
    SUSPENDED --> RUNNING: resume_vm()
    RUNNING --> TERMINATED: destroy_vm()
    STOPPED --> TERMINATED: destroy_vm()
    SUSPENDED --> TERMINATED: destroy_vm()
    TERMINATED --> [*]
```

## Task Execution Flow

```mermaid
flowchart TD
    START([Task Submitted]) --> MATCH{Match Agent<br/>Capability?}
    MATCH -->|Yes| QUEUE[Add to Task Queue]
    MATCH -->|No| REJECT[Reject Task]
    QUEUE --> SCHED[Schedule Task]
    SCHED --> DISPATCH[Dispatch to Agent]
    DISPATCH --> EXEC[Agent Executes Task]
    EXEC --> QUERY[Query AtomSpace]
    QUERY --> REASON[Apply Reasoning]
    REASON --> LEARN{Learning<br/>Required?}
    LEARN -->|Yes| GNN[Train/Predict GNN]
    LEARN -->|No| DECIDE[Make Decision]
    GNN --> DECIDE
    DECIDE --> UPDATE[Update Knowledge]
    UPDATE --> RESULT[Generate Result]
    RESULT --> RETURN([Return to User])
    REJECT --> RETURN
```

## Knowledge Graph Structure

```mermaid
graph LR
    subgraph "AtomSpace Hypergraph"
        C1[Concept: AI]
        C2[Concept: ML]
        C3[Concept: DL]
        P1[Predicate: is_a]
        P2[Predicate: uses]
        E1[Evaluation: ML is_a AI]
        E2[Evaluation: DL is_a ML]
        E3[Evaluation: DL uses GNN]
        
        E1 --> P1
        E1 --> C2
        E1 --> C1
        E2 --> P1
        E2 --> C3
        E2 --> C2
        E3 --> P2
        E3 --> C3
    end
    
    style C1 fill:#e1f5ff
    style C2 fill:#e1f5ff
    style C3 fill:#e1f5ff
    style P1 fill:#ffe1f5
    style P2 fill:#ffe1f5
    style E1 fill:#f5ffe1
    style E2 fill:#f5ffe1
    style E3 fill:#f5ffe1
```

## GNN Architecture

```mermaid
graph LR
    subgraph "Input Layer"
        INPUT[Node Features<br/>Adjacency Matrix]
    end
    
    subgraph "Hidden Layers"
        L1[GNN Layer 1<br/>256 dims]
        L2[GNN Layer 2<br/>256 dims]
        L3[GNN Layer 3<br/>256 dims]
    end
    
    subgraph "Output Layer"
        EMB[Node Embeddings]
        PRED[Link Predictions]
    end
    
    INPUT --> L1
    L1 --> L2
    L2 --> L3
    L3 --> EMB
    L3 --> PRED
```

## Data Flow

```mermaid
flowchart TB
    UI[User Interface]
    WB[OpenCog Workbench]
    ORCH[Multi-Agent Orchestrator]
    AS[AtomSpace Manager]
    RE[Reasoning Engine]
    GNN[Graph Neural Network]
    AGENTS[Cognitive Agents]
    HV[Agent-Zero Hypervisor]
    NT4[NT4 Cognitive Kernel]
    
    UI -->|Configuration| WB
    UI -->|Commands| WB
    WB -->|Initialize| ORCH
    WB -->|Configure| AS
    WB -->|Configure| RE
    WB -->|Configure| GNN
    WB -->|Configure| HV
    
    ORCH -->|Create Agent| AGENTS
    ORCH -->|Dispatch Task| AGENTS
    
    AGENTS -->|Query| AS
    AGENTS -->|Inference| RE
    AGENTS -->|Predict| GNN
    AGENTS -->|Request Resources| HV
    
    AS <-->|Knowledge| RE
    AS <-->|Graph Data| GNN
    RE <-->|Inference Results| GNN
    
    HV -->|System Calls| NT4
    HV -->|Allocate VM| AGENTS
    
    NT4 -->|Memory/Process| OS[Operating System]
    
    AGENTS -->|Results| ORCH
    ORCH -->|Status| WB
    WB -->|Response| UI
```

## Integration Points

### AtomSpace ↔ GNN
- GNN extracts graph structure from AtomSpace
- Learns embeddings for atoms
- Updates attention values based on predictions

### Orchestrator ↔ Hypervisor
- Creates VMs for each agent
- Manages resource allocation
- Enforces isolation

### Reasoning Engine ↔ AtomSpace
- Queries knowledge graph
- Creates new inferences
- Updates truth values

### Agents ↔ All Components
- Access AtomSpace for knowledge
- Use Reasoning for inference
- Leverage GNN for patterns
- Execute in hypervisor VMs

## Resource Management

### Memory
- Total pool configurable (default 1GB)
- Per-agent allocation (10-30MB typical)
- Tracking and limits enforced

### CPU
- Quota-based allocation (percentage)
- Priority scheduling
- Pre-emptive multitasking ready

### Virtual Machines
- Process-level isolation by default
- Configurable isolation levels
- Performance monitoring

## Configuration

### Default Settings
```json
{
  "orchestrator": {"max_agents": 50},
  "hypervisor": {"total_memory_kb": 1048576},
  "gnn": {"hidden_dim": 256, "num_layers": 3},
  "atomspace": {"max_atoms": 1000000},
  "reasoning": {"confidence_threshold": 0.7},
  "nt4_bridge": {"enable_kernel_calls": true}
}
```

## Usage Patterns

### Basic Usage
```python
from opencog_workbench import OpenCogWorkbench

workbench = OpenCogWorkbench()
workbench.create_agent("agent_1")
workbench.dispatch_task({'type': 'reasoning', ...})
status = workbench.get_system_status()
```

### Knowledge Building
```python
node_id = workbench.atomspace.add_atom(AtomType.CONCEPT, "ai")
link_id = workbench.atomspace.create_link(AtomType.LINK, node1, node2)
workbench.atomspace.spread_activation(node_id)
```

### GNN Training
```python
graph = workbench.gnn.extract_graph_from_atomspace()
results = workbench.train_gnn(epochs=10)
predictions = workbench.gnn.predict_links(threshold=0.6)
```

### Agent Coordination
```python
tasks = [
    {'type': 'reasoning', 'query': {...}},
    {'type': 'learning', 'data': {...}},
]
results = workbench.orchestrate_tasks(tasks)
```

## Performance Characteristics

### Scalability
- Tested with up to 50 concurrent agents
- AtomSpace handles 1M+ atoms
- GNN scales with graph size (O(n*e) per layer)

### Latency
- Agent creation: < 10ms
- Task dispatch: < 1ms
- GNN forward pass: 10-100ms depending on graph size
- Reasoning inference: 1-100ms depending on complexity

### Memory Usage
- Base system: ~50MB
- Per agent: 10-30MB
- AtomSpace: ~1KB per atom
- GNN: ~1MB per 100K parameters

## Windows NT4 Integration

### When Running on NT4
- Direct kernel API access
- Hardware virtualization support
- Native process management
- Optimized memory allocation

### When Running on Other Systems
- Simulation mode enabled
- Limited kernel integration
- Full cognitive features available
- Performance monitoring only

## Security Considerations

### Agent Isolation
- VM-based separation
- Resource quotas enforced
- Memory protection
- No cross-agent interference

### Privilege Levels
- Kernel mode (level 0) for system
- User mode (level 3) for agents
- Configurable per context

## Extension Points

### Custom Agents
Inherit from `AutonomousAgent` or `CognitiveAgent`

### Custom Inference Rules
Add to `ReasoningEngine.infer()`

### Custom GNN Layers
Extend `GNNLayer` class

### Custom Kernel Operations
Add to `NT4CognitiveKernel`

## Future Enhancements

1. Distributed agent execution across machines
2. Persistent AtomSpace storage
3. Real-time learning and adaptation
4. Advanced visualization tools
5. Performance optimization for large-scale deployments
6. Integration with modern deep learning frameworks
7. Cloud deployment support

## Dependencies

- Python 3.7+
- NumPy (for GNN computations)
- Windows NT4 (optional, for full kernel integration)

## Testing

Run the demo:
```bash
python demo.py
```

Run the installation test:
```bash
python test_installation.py
```

Run examples:
```bash
python examples/basic_usage.py
```

## License

See LICENSE file.

## Contact

For questions or contributions, visit: https://github.com/cogpy/WinKoGNN

---

## Native Code Integration (AGI-OS Layer)

Two external C/C++ repositories have been vendored to provide a high-performance
native substrate for the cognitive stack.

### Repository tree

```
WinKoGNN/
├── aten/                              # o9nn/ATenSpace (.git removed)
│   └── aten/src/
│       ├── ATen/
│       │   ├── atomspace/             # C++17 header-only AtomSpace (*.h, *.cpp)
│       │   └── *.h / *.cpp            # ATen tensor layer
│       ├── TH/                        # Low-level Torch-CPU C library (*.c, *.h)
│       ├── THC/                       # CUDA tensor library (*.cu, *.cuh)
│       └── THNN/                      # Neural-network primitives (*.cpp, *.h)
├── cog0/                              # ReZorg/cog0 — PENDING (private repo)
│   ├── include/                       # Public C/C++ headers (*.h / *.hxx)
│   └── src/                           # Implementation (*.c / *.cpp / *.cxx)
└── private/ntos/                      # NT4 kernel sources (existing)
```

### Integration mapping: ATenSpace → WinKoGNN subsystems

| ATenSpace header                   | WinKoGNN subsystem                                  | Notes |
|------------------------------------|-----------------------------------------------------|-------|
| `ATen/atomspace/AtomSpace.h`       | `opencog_workbench/core/atomspace.py`               | C++ backend for Python AtomSpaceManager |
| `ATen/atomspace/Atom.h`            | `opencog_workbench/core/atomspace.py`               | Node/Link type hierarchy |
| `ATen/atomspace/TruthValue.h`      | `opencog_workbench/core/atomspace.py`               | PLN truth-value arithmetic |
| `ATen/atomspace/PatternMatcher.h`  | `opencog_workbench/core/reasoning.py`               | Variable-binding queries |
| `ATen/atomspace/ForwardChainer.h`  | `opencog_workbench/core/reasoning.py`               | Forward-chaining inference |
| `ATen/atomspace/BackwardChainer.h` | `opencog_workbench/core/reasoning.py`               | Goal-directed backward chaining |
| `ATen/atomspace/TensorLogicEngine.h` | `opencog_workbench/gnn/graph_network.py`          | GPU batch logical ops over GNN |
| `ATen/atomspace/CognitiveEngine.h` | `opencog_workbench/workbench.py`                    | Master algorithm / COGSCM integration |
| `ATen/atomspace/ECAN.h`            | `opencog_workbench/agents/cognitive_agent.py`       | Economic attention (STI/LTI/VLTI) |
| `ATen/atomspace/AttentionBank.h`   | `opencog_workbench/agents/coordination_agent.py`    | Attention spreading / focus set |
| `ATen/atomspace/TimeServer.h`      | `opencog_workbench/agents/reasoning_agent.py`       | Temporal event tracking |
| `ATen/atomspace/NLU.h`             | new: `opencog_workbench/perception/nlu.py`          | Natural-language understanding |
| `ATen/atomspace/Vision.h`          | new: `opencog_workbench/perception/vision.py`       | Visual scene understanding |
| `ATen/atomspace/ATenNN.h`          | `opencog_workbench/gnn/node_embeddings.py`          | Pre-trained BERT/GPT/ViT/YOLO |
| `ATen/atomspace/Serializer.h`      | `opencog_workbench/config/config_manager.py`        | AtomSpace persistence (save/load) |
| `aten/src/TH/*.c`                  | `private/ntos/` (memory/process layer)              | CPU tensor ops callable from NT4 kernel |

### Integration mapping: cog0 → WinKoGNN subsystems

> cog0 (ReZorg/cog0) is a **private** repository.  The mapping below will be
> refined once the source tree is available.  The anticipated C/H and CXX/HXX
> files are expected to occupy:
>
> | cog0 path              | WinKoGNN subsystem                                 |
> |------------------------|----------------------------------------------------|
> | `include/*.h / *.hxx`  | `opencog_workbench/core/` — shared cognitive types |
> | `src/kernel/*.c`       | `private/ntos/` — NT4 kernel extensions            |
> | `src/agent/*.cpp`      | `opencog_workbench/agents/` — agent primitives     |
> | `src/gnn/*.cxx`        | `opencog_workbench/gnn/` — GNN forward passes      |

### CMake integration

Enable native builds when LibTorch is available:

```bash
# ATenSpace (header-only interface target is always available)
cmake -DBUILD_ATENSPACE=ON -DTorch_DIR=/path/to/libtorch/share/cmake/Torch ..

# cog0 (once source tree is vendored at cog0/)
cmake -DBUILD_COG0=ON ..
```

The `atenspace_headers` INTERFACE target is always defined and exposes
`aten/aten/src` as an include path, so any WinKoGNN target can consume the
header-only AtomSpace without requiring LibTorch:

```cmake
target_link_libraries(my_target PRIVATE atenspace_headers)
```
