"""Learning Agent implementation."""

import logging
from typing import Any, Dict, List, Optional

from .cognitive_agent import CognitiveAgent

logger = logging.getLogger(__name__)


class LearningAgent(CognitiveAgent):
    """
    Specialized agent for incremental learning and GNN training.

    Extends CognitiveAgent with online learning capabilities: it keeps a
    buffer of recent observations and periodically triggers GNN re-training
    when the buffer reaches a configurable threshold.
    """

    def __init__(self, agent_id: str, buffer_size: int = 50,
                 train_epochs: int = 5,
                 capabilities: Optional[List[str]] = None):
        default_caps = ["learning", "pattern_recognition"]
        super().__init__(agent_id, capabilities=capabilities or default_caps)
        self.buffer_size = buffer_size
        self.train_epochs = train_epochs
        self._observation_buffer: List[Dict[str, Any]] = []
        self._training_cycles: int = 0

    # ------------------------------------------------------------------
    # Task routing override
    # ------------------------------------------------------------------

    def execute_task(self, task: Dict[str, Any]) -> Dict[str, Any]:
        """Buffer observations and trigger learning when ready."""
        # Accumulate any data attached to the task
        data = task.get("data") or task.get("observations")
        if data:
            if isinstance(data, list):
                self._observation_buffer.extend(data)
            else:
                self._observation_buffer.append(data)

        result = super().execute_task(task)

        # Auto-trigger GNN training when buffer is full
        if len(self._observation_buffer) >= self.buffer_size and self.gnn_backend:
            self._flush_buffer_to_gnn()

        return result

    # ------------------------------------------------------------------
    # Specialised learning
    # ------------------------------------------------------------------

    def _perform_learning(self, task: Dict[str, Any]) -> Dict[str, Any]:
        """
        Incorporate new observations and (re-)train the GNN if available.
        """
        new_knowledge = task.get("knowledge", {})
        if new_knowledge and self.atomspace_manager:
            from opencog_workbench.core.atomspace import AtomType
            for name, props in new_knowledge.items():
                self.atomspace_manager.add_atom(
                    AtomType.CONCEPT, name,
                    truth_value=props.get("truth_value", 1.0),
                    confidence=props.get("confidence", 1.0),
                )
            logger.debug(
                "LearningAgent %s: added %d concepts to AtomSpace",
                self.agent_id, len(new_knowledge),
            )

        training_result = {}
        if self.gnn_backend and self.atomspace_manager:
            training_result = self._trigger_gnn_training()

        return {
            "success": True,
            "atoms_added": len(new_knowledge),
            "gnn_training": training_result,
        }

    def _trigger_gnn_training(self) -> Dict[str, Any]:
        """Extract the current graph and run a training cycle."""
        try:
            graph = self.gnn_backend.extract_graph_from_atomspace()
            metrics = self.gnn_backend.train(graph, epochs=self.train_epochs)
            self._training_cycles += 1
            logger.info(
                "LearningAgent %s: GNN training cycle %d complete",
                self.agent_id, self._training_cycles,
            )
            return metrics
        except Exception as exc:
            logger.error("LearningAgent %s: GNN training failed: %s", self.agent_id, exc)
            return {"error": str(exc)}

    def _flush_buffer_to_gnn(self):
        """Transfer buffered observations to AtomSpace and retrain."""
        if not self.atomspace_manager:
            self._observation_buffer.clear()
            return

        from opencog_workbench.core.atomspace import AtomType
        for obs in self._observation_buffer:
            if isinstance(obs, dict) and "name" in obs:
                self.atomspace_manager.add_atom(
                    AtomType.CONCEPT, obs["name"],
                    truth_value=obs.get("truth_value", 1.0),
                )
        self._observation_buffer.clear()
        self._trigger_gnn_training()

    # ------------------------------------------------------------------
    # Introspection
    # ------------------------------------------------------------------

    def get_learning_stats(self) -> Dict[str, Any]:
        """Return summary statistics for this agent's learning activity."""
        return {
            "training_cycles": self._training_cycles,
            "buffer_fill": len(self._observation_buffer),
            "buffer_capacity": self.buffer_size,
        }
