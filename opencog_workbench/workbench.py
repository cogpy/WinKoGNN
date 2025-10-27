"""
OpenCog Workbench - Main Integration
Integrates all components for autonomous multi-agent orchestration
"""

import logging
from typing import Dict, Any, Optional, List

from .core.orchestrator import MultiAgentOrchestrator
from .core.atomspace import AtomSpaceManager
from .core.reasoning import ReasoningEngine
from .hypervisor.agent_zero import AgentZeroHypervisor
from .gnn.graph_network import GraphNeuralNetwork
from .nt4_bridge.kernel_bridge import NT4CognitiveKernel
from .agents.cognitive_agent import CognitiveAgent
from .config.config_manager import ConfigManager

logger = logging.getLogger(__name__)


class OpenCogWorkbench:
    """
    Main OpenCog Workbench class
    Provides autonomous multi-agent orchestration for Windows NT4 cognitive kernel
    with Agent-Zero hypervisor and Graph Neural Network integration
    """
    
    def __init__(self, config: Optional[Dict[str, Any]] = None):
        """
        Initialize the OpenCog Workbench
        
        Args:
            config: Configuration dictionary or path to config file
        """
        # Load configuration
        if isinstance(config, str):
            self.config_manager = ConfigManager(config)
        elif isinstance(config, dict):
            self.config_manager = ConfigManager()
            for key, value in config.items():
                self.config_manager.config[key] = value
        else:
            self.config_manager = ConfigManager()
            
        # Initialize core components
        logger.info("Initializing OpenCog Workbench...")
        
        # 1. AtomSpace for knowledge representation
        self.atomspace = AtomSpaceManager(
            self.config_manager.get_section('atomspace')
        )
        
        # 2. Reasoning Engine
        self.reasoning = ReasoningEngine(
            self.atomspace,
            self.config_manager.get_section('reasoning')
        )
        
        # 3. Graph Neural Network
        self.gnn = GraphNeuralNetwork(
            self.config_manager.get_section('gnn')
        )
        self.gnn.set_atomspace_manager(self.atomspace)
        
        # 4. Windows NT4 Kernel Bridge
        self.nt4_kernel = NT4CognitiveKernel(
            self.config_manager.get_section('nt4_bridge')
        )
        
        # 5. Agent-Zero Hypervisor
        self.hypervisor = AgentZeroHypervisor(
            self.config_manager.get_section('hypervisor')
        )
        
        # 6. Multi-Agent Orchestrator
        self.orchestrator = MultiAgentOrchestrator(
            self.config_manager.get_section('orchestrator')
        )
        
        # Connect components
        self._connect_components()
        
        self.is_initialized = True
        logger.info("OpenCog Workbench initialized successfully")
        
    def _connect_components(self):
        """Connect all components together"""
        # Connect orchestrator to other components
        self.orchestrator.set_atomspace_manager(self.atomspace)
        self.orchestrator.set_reasoning_engine(self.reasoning)
        self.orchestrator.set_gnn_backend(self.gnn)
        self.orchestrator.set_nt4_bridge(self.nt4_kernel)
        self.orchestrator.set_hypervisor(self.hypervisor)
        
        logger.info("All components connected")
        
    def create_agent(self, agent_id: str, agent_type: str = "cognitive",
                    capabilities: Optional[List[str]] = None,
                    memory_kb: int = 10240, cpu_quota: float = 10.0) -> bool:
        """
        Create and register a new autonomous agent
        
        Args:
            agent_id: Unique identifier for the agent
            agent_type: Type of agent (currently only 'cognitive' supported)
            capabilities: List of agent capabilities
            memory_kb: Memory allocation in KB
            cpu_quota: CPU quota percentage
            
        Returns:
            True if agent created successfully
        """
        # Create agent instance
        if agent_type == "cognitive":
            agent = CognitiveAgent(agent_id, capabilities)
        else:
            logger.error(f"Unknown agent type: {agent_type}")
            return False
            
        # Connect agent to components
        agent.set_atomspace_manager(self.atomspace)
        agent.set_reasoning_engine(self.reasoning)
        agent.set_gnn_backend(self.gnn)
        
        # Create VM context in hypervisor
        vm_id = self.hypervisor.create_vm(
            agent_id=agent_id,
            memory_kb=memory_kb,
            cpu_quota=cpu_quota
        )
        
        if not vm_id:
            logger.error(f"Failed to create VM for agent {agent_id}")
            return False
            
        # Start VM
        self.hypervisor.start_vm(vm_id)
        
        # Create kernel context
        self.nt4_kernel.create_kernel_context(agent_id)
        
        # Register with orchestrator
        if not self.orchestrator.register_agent(agent):
            logger.error(f"Failed to register agent {agent_id}")
            self.hypervisor.destroy_vm(vm_id)
            return False
            
        logger.info(f"Created agent {agent_id} with VM {vm_id}")
        return True
        
    def remove_agent(self, agent_id: str) -> bool:
        """Remove an agent from the system"""
        # Unregister from orchestrator
        if not self.orchestrator.unregister_agent(agent_id):
            logger.warning(f"Agent {agent_id} not found in orchestrator")
            
        # Destroy kernel context
        self.nt4_kernel.destroy_kernel_context(agent_id)
        
        # Destroy VM
        vm_id = f"vm_{agent_id}_{id(agent_id)}"
        self.hypervisor.destroy_vm(vm_id)
        
        logger.info(f"Removed agent {agent_id}")
        return True
        
    def dispatch_task(self, task: Dict[str, Any], agent_id: Optional[str] = None) -> bool:
        """
        Dispatch a task to an agent or let orchestrator select
        
        Args:
            task: Task dictionary with 'type', 'data', etc.
            agent_id: Optional specific agent ID
            
        Returns:
            True if task dispatched successfully
        """
        return self.orchestrator.dispatch_task(task, agent_id)
        
    def orchestrate_tasks(self, tasks: List[Dict[str, Any]]) -> Dict[str, List[Any]]:
        """
        Orchestrate multiple tasks across agents
        
        Args:
            tasks: List of task dictionaries
            
        Returns:
            Results dictionary with 'completed', 'failed', 'pending'
        """
        return self.orchestrator.orchestrate(tasks)
        
    def train_gnn(self, epochs: int = 10) -> Dict[str, Any]:
        """
        Train the Graph Neural Network on current AtomSpace
        
        Args:
            epochs: Number of training epochs
            
        Returns:
            Training results
        """
        logger.info(f"Training GNN for {epochs} epochs...")
        graph = self.gnn.extract_graph_from_atomspace()
        results = self.gnn.train(graph, epochs=epochs)
        logger.info(f"GNN training completed: final loss = {results['final_loss']:.4f}")
        return results
        
    def get_system_status(self) -> Dict[str, Any]:
        """Get comprehensive system status"""
        return {
            'workbench': {
                'initialized': self.is_initialized,
                'components_connected': True
            },
            'orchestrator': self.orchestrator.get_system_status(),
            'atomspace': self.atomspace.get_statistics(),
            'reasoning': self.reasoning.get_statistics(),
            'gnn': self.gnn.get_statistics(),
            'hypervisor': self.hypervisor.get_system_resources(),
            'nt4_kernel': self.nt4_kernel.get_system_statistics()
        }
        
    def shutdown(self):
        """Shutdown the workbench and all components"""
        logger.info("Shutting down OpenCog Workbench...")
        
        # Shutdown hypervisor (will stop all VMs)
        self.hypervisor.shutdown()
        
        # Clear AtomSpace
        self.atomspace.clear()
        
        # Clear reasoning history
        self.reasoning.clear_history()
        
        self.is_initialized = False
        logger.info("OpenCog Workbench shutdown complete")
        
    def __enter__(self):
        """Context manager entry"""
        return self
        
    def __exit__(self, exc_type, exc_val, exc_tb):
        """Context manager exit"""
        self.shutdown()
        return False
