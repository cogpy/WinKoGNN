# OpenCog Workbench for Windows NT4 Cognitive Kernel

Autonomous Multi-Agent Orchestration Workbench with Agent-Zero Hypervisor and Graph Neural Network Integration

## 📚 Documentation

**Quick Links:**
- [📖 Documentation Guide](DOCUMENTATION_GUIDE.md) - Navigate all documentation
- [🏗️ Architecture](ARCHITECTURE.md) - Technical architecture with 17 Mermaid diagrams
- [📐 Formal Specification](FORMAL_SPECIFICATION.md) - Z++ formal specification with 40 schemas
- [📊 Implementation Summary](IMPLEMENTATION_SUMMARY.md) - Implementation details and metrics

## Overview

The OpenCog Workbench provides a comprehensive cognitive architecture for Windows NT4, integrating:

- **Multi-Agent Orchestration**: Coordinate multiple autonomous agents for distributed cognitive processing
- **OpenCog AtomSpace**: Hypergraph-based knowledge representation system
- **Reasoning Engine**: Logical inference and pattern-based reasoning
- **Graph Neural Networks**: Deep learning over knowledge graphs
- **Agent-Zero Hypervisor**: Virtual machine management for agent isolation
- **Windows NT4 Kernel Bridge**: Low-level system integration

## Architecture

For detailed architecture information with comprehensive Mermaid diagrams, see [ARCHITECTURE.md](ARCHITECTURE.md).

For formal specification in Z++ notation, see [FORMAL_SPECIFICATION.md](FORMAL_SPECIFICATION.md).

```
┌─────────────────────────────────────────────────────────────┐
│                  OpenCog Workbench                           │
├─────────────────────────────────────────────────────────────┤
│                                                               │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐      │
│  │ Multi-Agent  │  │  AtomSpace   │  │  Reasoning   │      │
│  │ Orchestrator │◄─┤   Manager    │◄─┤    Engine    │      │
│  └──────────────┘  └──────────────┘  └──────────────┘      │
│         │                  │                                 │
│         ▼                  ▼                                 │
│  ┌──────────────┐  ┌──────────────┐                        │
│  │  Agent-Zero  │  │     GNN      │                        │
│  │  Hypervisor  │  │   Backend    │                        │
│  └──────────────┘  └──────────────┘                        │
│         │                                                    │
│         ▼                                                    │
│  ┌──────────────────────────────┐                          │
│  │  Windows NT4 Kernel Bridge   │                          │
│  └──────────────────────────────┘                          │
│                                                               │
└─────────────────────────────────────────────────────────────┘
```

## Features

### 1. Multi-Agent Orchestration
- **Autonomous Agents**: Self-directed agents with cognitive capabilities
- **Task Scheduling**: Intelligent task distribution based on agent capabilities
- **Capability Matching**: Automatic selection of best agent for each task
- **State Management**: Track and manage agent execution states

### 2. Knowledge Representation (AtomSpace)
- **Hypergraph Structure**: Flexible knowledge representation
- **Truth Values**: Probabilistic logic with confidence measures
- **Attention Values**: Dynamic importance weighting
- **Activation Spreading**: Knowledge propagation through graph

### 3. Reasoning Engine
- **Multiple Inference Rules**: Deduction, induction, abduction, modus ponens
- **Forward Chaining**: Derive conclusions from facts
- **Backward Chaining**: Goal-directed reasoning
- **Probabilistic Inference**: Bayesian reasoning support

### 4. Graph Neural Networks
- **Knowledge Graph Processing**: Learn embeddings from AtomSpace
- **Link Prediction**: Discover potential relationships
- **Pattern Recognition**: Identify structural patterns
- **Self-Supervised Learning**: Train without explicit labels

### 5. Agent-Zero Hypervisor
- **Resource Isolation**: Separate execution contexts for agents
- **Memory Management**: Allocate and track memory per agent
- **CPU Scheduling**: Priority-based scheduling
- **VM Lifecycle**: Create, start, stop, suspend, resume, destroy

### 6. Windows NT4 Integration
- **Kernel Bridge**: Interface to NT4 kernel APIs
- **Process Management**: Create and manage processes
- **Memory Operations**: Low-level memory allocation
- **IPC Support**: Inter-process communication

## Installation

### Requirements
- Python 3.7+
- NumPy
- (Optional) Windows NT4 for full kernel integration

### Install Dependencies

```bash
pip install -r requirements.txt
```

### Basic Setup

```python
from opencog_workbench import OpenCogWorkbench

# Initialize workbench
workbench = OpenCogWorkbench()

# Create agents
workbench.create_agent("agent_1")
workbench.create_agent("agent_2")

# Dispatch tasks
task = {'type': 'reasoning', 'query': {...}}
workbench.dispatch_task(task)
```

## Usage Examples

### Example 1: Basic Usage

```python
from opencog_workbench import OpenCogWorkbench
from opencog_workbench.core.atomspace import AtomType

with OpenCogWorkbench() as workbench:
    # Add knowledge
    node1 = workbench.atomspace.add_atom(
        AtomType.CONCEPT, "ai", truth_value=0.9
    )
    
    # Create agents
    workbench.create_agent("agent_1", memory_kb=20480)
    
    # Train GNN
    workbench.train_gnn(epochs=10)
    
    # Get status
    status = workbench.get_system_status()
```

See `examples/basic_usage.py` for complete example.

### Example 2: Advanced Multi-Agent Coordination

```python
# Create specialized agents
workbench.create_agent("reasoning_agent", 
                      capabilities=["reasoning", "decision_making"])
workbench.create_agent("learning_agent", 
                      capabilities=["learning", "pattern_recognition"])

# Orchestrate tasks
tasks = [
    {'type': 'reasoning', 'query': {...}},
    {'type': 'learning', 'data': {...}},
    {'type': 'pattern_recognition', 'data': [...]}
]

results = workbench.orchestrate_tasks(tasks)
```

See `examples/advanced_coordination.py` for complete example.

## Configuration

Configuration can be provided as a dictionary or JSON file:

```python
config = {
    'orchestrator': {
        'max_agents': 50,
        'task_queue_size': 1000
    },
    'hypervisor': {
        'total_memory_kb': 1024 * 1024,  # 1GB
        'nt4_integration': True
    },
    'gnn': {
        'hidden_dim': 256,
        'num_layers': 3
    }
}

workbench = OpenCogWorkbench(config)
```

## API Reference

### OpenCogWorkbench

Main interface to the cognitive architecture.

**Methods:**
- `create_agent(agent_id, agent_type, capabilities, memory_kb, cpu_quota)`: Create new agent
- `remove_agent(agent_id)`: Remove agent
- `dispatch_task(task, agent_id)`: Send task to agent
- `orchestrate_tasks(tasks)`: Distribute multiple tasks
- `train_gnn(epochs)`: Train graph neural network
- `get_system_status()`: Get comprehensive status
- `shutdown()`: Clean shutdown

### AtomSpaceManager

Manage knowledge representation.

**Methods:**
- `add_atom(atom_type, name, **kwargs)`: Add atom
- `create_link(link_type, source_id, target_id)`: Create link
- `get_atom(atom_id)`: Retrieve atom
- `query_pattern(pattern)`: Query atoms
- `spread_activation(source_id, decay)`: Propagate activation

### ReasoningEngine

Perform inference and reasoning.

**Methods:**
- `infer(query)`: Perform inference
- `forward_chain(start_atoms, max_steps)`: Forward reasoning
- `backward_chain(goal_atom, max_depth)`: Backward reasoning
- `enable_rule(rule, enabled)`: Enable/disable inference rule

### GraphNeuralNetwork

Process knowledge graphs with neural networks.

**Methods:**
- `extract_graph_from_atomspace()`: Extract graph structure
- `train(graph, labels, epochs)`: Train network
- `predict_links(threshold)`: Predict potential links
- `compute_similarity(node_id1, node_id2)`: Compute node similarity

### AgentZeroHypervisor

Manage virtual machines for agents.

**Methods:**
- `create_vm(agent_id, memory_kb, cpu_quota)`: Create VM
- `destroy_vm(vm_id)`: Destroy VM
- `start_vm(vm_id)`, `stop_vm(vm_id)`: Control VM lifecycle
- `get_system_resources()`: Get resource usage

### NT4CognitiveKernel

Bridge to Windows NT4 kernel.

**Methods:**
- `create_kernel_context(context_id)`: Create context
- `allocate_memory(context_id, size_bytes)`: Allocate memory
- `create_process(context_id)`: Create process
- `send_ipc_message(source, target, message)`: IPC

## System Requirements

### Minimum Requirements
- CPU: 486DX2/66 MHz or higher
- RAM: 16 MB (32 MB recommended)
- Disk: 100 MB free space
- OS: Windows NT4 Workstation or Server

### Recommended for Production
- CPU: Pentium 166 MHz or higher
- RAM: 64 MB or more
- Disk: 500 MB free space
- OS: Windows NT4 Server with Service Pack 6a

## Performance Considerations

- **Memory**: Each agent requires 10-30 MB by default
- **CPU**: GNN training is CPU-intensive
- **I/O**: AtomSpace operations are memory-bound
- **Scalability**: Tested with up to 50 concurrent agents

## Troubleshooting

### Common Issues

**Issue**: Agent creation fails
- Check available memory in hypervisor
- Verify CPU quota doesn't exceed 100%

**Issue**: GNN training slow
- Reduce `num_layers` or `hidden_dim`
- Use smaller knowledge graphs for training

**Issue**: NT4 integration unavailable
- System may not be running on NT4
- Check `is_nt4_available` flag
- Workbench functions without NT4, but without kernel integration

## Development

### Project Structure

```
opencog_workbench/
├── __init__.py
├── workbench.py          # Main integration
├── core/
│   ├── orchestrator.py   # Multi-agent orchestration
│   ├── atomspace.py      # Knowledge representation
│   └── reasoning.py      # Inference engine
├── agents/
│   └── cognitive_agent.py # Agent implementations
├── gnn/
│   └── graph_network.py  # Graph neural network
├── hypervisor/
│   └── agent_zero.py     # VM management
├── nt4_bridge/
│   └── kernel_bridge.py  # NT4 kernel interface
└── config/
    └── config_manager.py # Configuration

examples/
├── basic_usage.py
└── advanced_coordination.py
```

## License

See LICENSE file for details.

## Contributing

Contributions welcome! Please submit pull requests or open issues on GitHub.

## Authors

CogPy Team

## Acknowledgments

- OpenCog framework
- Windows NT4 architecture
- Agent-Zero concepts
- Graph Neural Network research
