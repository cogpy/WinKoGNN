"""
Cognitive Agent Implementation
Autonomous agent with cognitive capabilities
"""

import logging
from typing import Dict, List, Any, Optional
from ..core.orchestrator import AutonomousAgent, AgentState

logger = logging.getLogger(__name__)


class CognitiveAgent(AutonomousAgent):
    """
    Cognitive agent with reasoning, learning, and decision-making capabilities
    Integrates with OpenCog AtomSpace and GNN for knowledge processing
    """
    
    def __init__(self, agent_id: str, capabilities: List[str] = None):
        super().__init__(agent_id, "cognitive", capabilities or [
            "reasoning", "learning", "pattern_recognition", "decision_making"
        ])
        
        # Cognitive components
        self.knowledge_base = {}
        self.working_memory = []
        self.long_term_memory = []
        self.decision_threshold = 0.7
        
        # Integration points
        self.atomspace_manager = None
        self.reasoning_engine = None
        self.gnn_backend = None
        
        logger.info(f"CognitiveAgent {agent_id} initialized")
        
    def execute_task(self, task: Dict[str, Any]) -> Dict[str, Any]:
        """Execute a cognitive task"""
        task_type = task.get('type', 'unknown')
        
        self.set_state(AgentState.ACTIVE)
        
        try:
            if task_type == 'reasoning':
                result = self._perform_reasoning(task)
            elif task_type == 'learning':
                result = self._perform_learning(task)
            elif task_type == 'pattern_recognition':
                result = self._recognize_patterns(task)
            elif task_type == 'decision_making':
                result = self._make_decision(task)
            else:
                result = {'success': False, 'error': f'Unknown task type: {task_type}'}
                
            self.set_state(AgentState.IDLE)
            return result
            
        except Exception as e:
            logger.error(f"Error executing task: {e}")
            self.set_state(AgentState.ERROR)
            return {'success': False, 'error': str(e)}
            
    def _perform_reasoning(self, task: Dict[str, Any]) -> Dict[str, Any]:
        """Perform reasoning task"""
        query = task.get('query', {})
        
        if self.reasoning_engine:
            inferences = self.reasoning_engine.infer(query)
            return {
                'success': True,
                'inferences': len(inferences),
                'results': [
                    {
                        'rule': inf.rule.value,
                        'conclusion': inf.conclusion,
                        'confidence': inf.confidence
                    }
                    for inf in inferences[:10]  # Return top 10
                ]
            }
        else:
            return {'success': False, 'error': 'Reasoning engine not connected'}
            
    def _perform_learning(self, task: Dict[str, Any]) -> Dict[str, Any]:
        """Perform learning task"""
        data = task.get('data', {})
        learning_type = task.get('learning_type', 'supervised')
        
        # Store in working memory
        self.working_memory.append(data)
        
        # If GNN is available, use it for learning
        if self.gnn_backend:
            # Extract graph and train
            return {
                'success': True,
                'learning_type': learning_type,
                'samples_processed': 1
            }
        else:
            # Fallback to simple memory storage
            self.long_term_memory.append(data)
            return {
                'success': True,
                'learning_type': 'memory_based',
                'samples_stored': len(self.long_term_memory)
            }
            
    def _recognize_patterns(self, task: Dict[str, Any]) -> Dict[str, Any]:
        """Recognize patterns in data"""
        data = task.get('data', [])
        
        patterns_found = []
        
        # Use GNN for pattern recognition if available
        if self.gnn_backend and self.atomspace_manager:
            # Get embeddings and find similar patterns
            for item in data:
                # Simplified pattern matching
                patterns_found.append({
                    'pattern': 'similarity_cluster',
                    'confidence': 0.8
                })
                
        return {
            'success': True,
            'patterns_found': len(patterns_found),
            'patterns': patterns_found[:5]  # Return top 5
        }
        
    def _make_decision(self, task: Dict[str, Any]) -> Dict[str, Any]:
        """Make a decision based on available information"""
        options = task.get('options', [])
        context = task.get('context', {})
        
        if not options:
            return {'success': False, 'error': 'No options provided'}
            
        # Score each option
        scores = []
        for option in options:
            score = self._evaluate_option(option, context)
            scores.append((option, score))
            
        # Select best option
        scores.sort(key=lambda x: x[1], reverse=True)
        best_option, best_score = scores[0]
        
        decision_made = best_score >= self.decision_threshold
        
        return {
            'success': True,
            'decision': best_option if decision_made else None,
            'confidence': best_score,
            'threshold_met': decision_made,
            'all_scores': [
                {'option': opt, 'score': score}
                for opt, score in scores
            ]
        }
        
    def _evaluate_option(self, option: Any, context: Dict[str, Any]) -> float:
        """Evaluate an option based on context"""
        # Simplified evaluation
        # In full implementation, would use reasoning engine and knowledge base
        base_score = 0.5
        
        # Adjust based on working memory
        if option in self.working_memory:
            base_score += 0.2
            
        return min(base_score, 1.0)
        
    def set_atomspace_manager(self, atomspace_manager):
        """Connect to AtomSpace manager"""
        self.atomspace_manager = atomspace_manager
        logger.debug(f"Agent {self.metadata.agent_id} connected to AtomSpace")
        
    def set_reasoning_engine(self, reasoning_engine):
        """Connect to reasoning engine"""
        self.reasoning_engine = reasoning_engine
        logger.debug(f"Agent {self.metadata.agent_id} connected to reasoning engine")
        
    def set_gnn_backend(self, gnn_backend):
        """Connect to GNN backend"""
        self.gnn_backend = gnn_backend
        logger.debug(f"Agent {self.metadata.agent_id} connected to GNN backend")
        
    def get_status(self) -> Dict[str, Any]:
        """Get agent status"""
        return {
            'agent_id': self.metadata.agent_id,
            'state': self.metadata.state.value,
            'capabilities': self.metadata.capabilities,
            'working_memory_size': len(self.working_memory),
            'long_term_memory_size': len(self.long_term_memory),
            'knowledge_base_size': len(self.knowledge_base),
            'components_connected': {
                'atomspace': self.atomspace_manager is not None,
                'reasoning': self.reasoning_engine is not None,
                'gnn': self.gnn_backend is not None
            }
        }
