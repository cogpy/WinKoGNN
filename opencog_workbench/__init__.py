"""
OpenCog Workbench - Autonomous Multi-Agent Orchestration System
for Windows NT4 Cognitive Kernel with Agent-Zero Hypervisor GNN Integration
"""

__version__ = "1.0.0"
__author__ = "CogPy Team"

from .core.orchestrator import MultiAgentOrchestrator
from .core.visualization import AtomSpaceVisualizer
from .hypervisor.agent_zero import AgentZeroHypervisor
from .gnn.graph_network import GraphNeuralNetwork
from .nt4_bridge.kernel_bridge import NT4CognitiveKernel
from .nt4_bridge.nt4_source_analyzer import NT4SourceAnalyzer
from .workbench import OpenCogWorkbench

__all__ = [
    'MultiAgentOrchestrator',
    'AgentZeroHypervisor',
    'GraphNeuralNetwork',
    'NT4CognitiveKernel',
    'NT4SourceAnalyzer',
    'AtomSpaceVisualizer',
    'OpenCogWorkbench',
]
