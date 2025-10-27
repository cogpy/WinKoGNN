"""
Multi-Agent Orchestrator for OpenCog Workbench
Coordinates autonomous agents in the Windows NT4 cognitive kernel environment
"""

import logging
from typing import Dict, List, Any, Optional
from dataclasses import dataclass, field
from enum import Enum
import threading
import queue

logger = logging.getLogger(__name__)


class AgentState(Enum):
    """Agent execution states"""
    IDLE = "idle"
    ACTIVE = "active"
    SUSPENDED = "suspended"
    TERMINATED = "terminated"
    ERROR = "error"


@dataclass
class AgentMetadata:
    """Metadata for autonomous agents"""
    agent_id: str
    agent_type: str
    priority: int = 1
    state: AgentState = AgentState.IDLE
    capabilities: List[str] = field(default_factory=list)
    memory_allocation: int = 0  # In KB
    cpu_affinity: Optional[int] = None
    hypervisor_context: Optional[Dict[str, Any]] = None


class AutonomousAgent:
    """Base class for autonomous agents in the orchestration system"""
    
    def __init__(self, agent_id: str, agent_type: str, capabilities: List[str] = None):
        self.metadata = AgentMetadata(
            agent_id=agent_id,
            agent_type=agent_type,
            capabilities=capabilities or []
        )
        self.task_queue = queue.Queue()
        self.result_queue = queue.Queue()
        
    def execute_task(self, task: Dict[str, Any]) -> Dict[str, Any]:
        """Execute a task and return results"""
        raise NotImplementedError("Subclasses must implement execute_task")
        
    def get_state(self) -> AgentState:
        """Get current agent state"""
        return self.metadata.state
        
    def set_state(self, state: AgentState):
        """Set agent state"""
        self.metadata.state = state
        logger.info(f"Agent {self.metadata.agent_id} state changed to {state.value}")


class MultiAgentOrchestrator:
    """
    Orchestrates multiple autonomous agents for distributed cognitive processing
    Integrates with Windows NT4 kernel for low-level system operations
    """
    
    def __init__(self, config: Optional[Dict[str, Any]] = None):
        self.config = config or {}
        self.agents: Dict[str, AutonomousAgent] = {}
        self.agent_registry: Dict[str, AgentMetadata] = {}
        self.task_scheduler = TaskScheduler()
        self.coordination_lock = threading.Lock()
        
        # OpenCog integration components
        self.atomspace_manager = None
        self.reasoning_engine = None
        self.gnn_backend = None
        self.nt4_bridge = None
        self.hypervisor = None
        
        logger.info("MultiAgentOrchestrator initialized")
        
    def register_agent(self, agent: AutonomousAgent) -> bool:
        """Register a new autonomous agent"""
        with self.coordination_lock:
            if agent.metadata.agent_id in self.agents:
                logger.warning(f"Agent {agent.metadata.agent_id} already registered")
                return False
                
            self.agents[agent.metadata.agent_id] = agent
            self.agent_registry[agent.metadata.agent_id] = agent.metadata
            logger.info(f"Registered agent {agent.metadata.agent_id} with type {agent.metadata.agent_type}")
            return True
            
    def unregister_agent(self, agent_id: str) -> bool:
        """Unregister an agent"""
        with self.coordination_lock:
            if agent_id not in self.agents:
                logger.warning(f"Agent {agent_id} not found")
                return False
                
            agent = self.agents[agent_id]
            agent.set_state(AgentState.TERMINATED)
            del self.agents[agent_id]
            del self.agent_registry[agent_id]
            logger.info(f"Unregistered agent {agent_id}")
            return True
            
    def dispatch_task(self, task: Dict[str, Any], target_agent_id: Optional[str] = None) -> bool:
        """Dispatch a task to an agent or select best agent"""
        with self.coordination_lock:
            if target_agent_id:
                if target_agent_id not in self.agents:
                    logger.error(f"Target agent {target_agent_id} not found")
                    return False
                agent = self.agents[target_agent_id]
            else:
                # Select best agent based on capabilities and state
                agent = self._select_best_agent(task)
                if not agent:
                    logger.error("No suitable agent found for task")
                    return False
                    
            agent.task_queue.put(task)
            logger.debug(f"Task dispatched to agent {agent.metadata.agent_id}")
            return True
            
    def _select_best_agent(self, task: Dict[str, Any]) -> Optional[AutonomousAgent]:
        """Select the best agent for a given task based on capabilities"""
        required_capability = task.get('required_capability')
        
        available_agents = [
            agent for agent in self.agents.values()
            if agent.get_state() == AgentState.IDLE or agent.get_state() == AgentState.ACTIVE
        ]
        
        if not available_agents:
            return None
            
        if required_capability:
            # Filter by capability
            capable_agents = [
                agent for agent in available_agents
                if required_capability in agent.metadata.capabilities
            ]
            if capable_agents:
                # Select by priority
                return max(capable_agents, key=lambda a: a.metadata.priority)
                
        # Default: return highest priority agent
        return max(available_agents, key=lambda a: a.metadata.priority)
        
    def orchestrate(self, tasks: List[Dict[str, Any]]) -> Dict[str, List[Any]]:
        """Orchestrate multiple tasks across agents"""
        results = {'completed': [], 'failed': [], 'pending': []}
        
        for task in tasks:
            if self.dispatch_task(task):
                results['pending'].append(task)
            else:
                results['failed'].append(task)
                
        logger.info(f"Orchestration summary: {len(results['completed'])} completed, "
                   f"{len(results['failed'])} failed, {len(results['pending'])} pending")
        return results
        
    def set_atomspace_manager(self, atomspace_manager):
        """Connect to OpenCog AtomSpace manager"""
        self.atomspace_manager = atomspace_manager
        logger.info("AtomSpace manager connected")
        
    def set_reasoning_engine(self, reasoning_engine):
        """Connect to reasoning engine"""
        self.reasoning_engine = reasoning_engine
        logger.info("Reasoning engine connected")
        
    def set_gnn_backend(self, gnn_backend):
        """Connect to Graph Neural Network backend"""
        self.gnn_backend = gnn_backend
        logger.info("GNN backend connected")
        
    def set_nt4_bridge(self, nt4_bridge):
        """Connect to Windows NT4 kernel bridge"""
        self.nt4_bridge = nt4_bridge
        logger.info("NT4 kernel bridge connected")
        
    def set_hypervisor(self, hypervisor):
        """Connect to Agent-Zero hypervisor"""
        self.hypervisor = hypervisor
        logger.info("Agent-Zero hypervisor connected")
        
    def get_system_status(self) -> Dict[str, Any]:
        """Get comprehensive system status"""
        return {
            'total_agents': len(self.agents),
            'active_agents': len([a for a in self.agents.values() if a.get_state() == AgentState.ACTIVE]),
            'idle_agents': len([a for a in self.agents.values() if a.get_state() == AgentState.IDLE]),
            'components': {
                'atomspace': self.atomspace_manager is not None,
                'reasoning': self.reasoning_engine is not None,
                'gnn': self.gnn_backend is not None,
                'nt4_bridge': self.nt4_bridge is not None,
                'hypervisor': self.hypervisor is not None
            }
        }


class TaskScheduler:
    """Schedules tasks for agent execution"""
    
    def __init__(self):
        self.task_queue = queue.PriorityQueue()
        self.scheduled_tasks: Dict[str, Any] = {}
        
    def schedule_task(self, task: Dict[str, Any], priority: int = 1):
        """Schedule a task with priority"""
        task_id = task.get('task_id', id(task))
        self.task_queue.put((priority, task_id, task))
        self.scheduled_tasks[task_id] = task
        
    def get_next_task(self) -> Optional[Dict[str, Any]]:
        """Get next task from queue"""
        if not self.task_queue.empty():
            _, task_id, task = self.task_queue.get()
            return task
        return None
