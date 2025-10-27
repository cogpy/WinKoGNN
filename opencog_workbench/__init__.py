"""
OpenCog Workbench - Autonomous Multi-Agent Orchestration System
for Windows NT4 Cognitive Kernel with Agent-Zero Hypervisor GNN Integration
"""

__version__ = "1.0.0"
__author__ = "CogPy Team"

from .core.orchestrator import MultiAgentOrchestrator
from .hypervisor.agent_zero import AgentZeroHypervisor
from .gnn.graph_network import GraphNeuralNetwork
from .nt4_bridge.kernel_bridge import NT4CognitiveKernel
from .workbench import OpenCogWorkbench

__all__ = [
    'MultiAgentOrchestrator',
    'AgentZeroHypervisor', 
    'GraphNeuralNetwork',
    'NT4CognitiveKernel',
    'OpenCogWorkbench'
]
