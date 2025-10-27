"""Coordination Agent implementation."""

from .cognitive_agent import CognitiveAgent


class CoordinationAgent(CognitiveAgent):
    """Specialized agent for coordinating other agents"""
    
    def __init__(self, agent_id: str):
        super().__init__(agent_id, capabilities=["coordination", "reasoning", "decision_making"])
