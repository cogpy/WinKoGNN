"""Reasoning Agent implementation."""

from .cognitive_agent import CognitiveAgent


class ReasoningAgent(CognitiveAgent):
    """Specialized agent for reasoning tasks"""
    
    def __init__(self, agent_id: str):
        super().__init__(agent_id, capabilities=["reasoning", "decision_making"])
