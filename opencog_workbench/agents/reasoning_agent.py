"""Reasoning Agent implementation."""

import logging
from typing import Any, Dict, List, Optional

from .cognitive_agent import CognitiveAgent, AgentState

logger = logging.getLogger(__name__)


class ReasoningAgent(CognitiveAgent):
    """
    Specialized agent focused on logical inference and knowledge derivation.

    Extends CognitiveAgent with multi-step chain reasoning and explanation
    generation, preferring the attached ReasoningEngine when available.
    """

    def __init__(self, agent_id: str,
                 capabilities: Optional[List[str]] = None):
        default_caps = ["reasoning", "decision_making"]
        super().__init__(agent_id, capabilities=capabilities or default_caps)
        self.inference_depth: int = 3        # max forward-chain depth
        self.explanation_log: List[str] = [] # audit trail of conclusions

    # ------------------------------------------------------------------
    # Task routing override
    # ------------------------------------------------------------------

    def execute_task(self, task: Dict[str, Any]) -> Dict[str, Any]:
        """Route all tasks through the reasoning pipeline first."""
        task_type = task.get("type", "unknown")

        # Augment non-reasoning tasks with an initial reasoning step
        if task_type not in ("reasoning", "decision_making") and self.reasoning_engine:
            preamble = self._chain_reasoning(task)
            task = dict(task)
            task["reasoning_context"] = preamble.get("conclusions", [])

        return super().execute_task(task)

    # ------------------------------------------------------------------
    # Specialised reasoning
    # ------------------------------------------------------------------

    def _perform_reasoning(self, task: Dict[str, Any]) -> Dict[str, Any]:
        """Deep multi-step reasoning with explanation capture."""
        if self.reasoning_engine:
            result = self._chain_reasoning(task)
            self.explanation_log.extend(result.get("conclusions", []))
            return result

        # Fallback to base implementation
        return super()._perform_reasoning(task)

    def _chain_reasoning(self, task: Dict[str, Any]) -> Dict[str, Any]:
        """
        Iteratively apply forward chaining up to *self.inference_depth* steps.
        """
        query = task.get("query", task.get("reasoning_context", {}))
        all_conclusions: List[Any] = []

        current_query = query
        for step in range(self.inference_depth):
            step_result = self.reasoning_engine.forward_chain(current_query)
            conclusions = step_result.get("conclusions", [])
            if not conclusions:
                break
            all_conclusions.extend(conclusions)
            # Feed conclusions back as next query context
            current_query = {"context": conclusions}
            logger.debug(
                "ReasoningAgent %s: chain step %d yielded %d conclusions",
                self.agent_id, step + 1, len(conclusions),
            )

        return {
            "success": True,
            "conclusions": all_conclusions,
            "steps": len(all_conclusions),
        }

    def get_explanation(self) -> List[str]:
        """Return the accumulated explanation log."""
        return list(self.explanation_log)

    def clear_explanation_log(self):
        """Reset the explanation log."""
        self.explanation_log.clear()
