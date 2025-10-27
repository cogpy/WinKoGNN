"""Learning Agent implementation."""

from .cognitive_agent import CognitiveAgent


class LearningAgent(CognitiveAgent):
    """Specialized agent for learning tasks"""
    
    def __init__(self, agent_id: str):
        super().__init__(agent_id, capabilities=["learning", "pattern_recognition"])
