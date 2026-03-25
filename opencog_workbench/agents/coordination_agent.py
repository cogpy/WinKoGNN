"""Coordination Agent implementation."""

import logging
from typing import Any, Dict, List, Optional

from .cognitive_agent import CognitiveAgent, AgentState

logger = logging.getLogger(__name__)


class CoordinationAgent(CognitiveAgent):
    """
    Agent responsible for orchestrating other agents.

    Maintains a registry of peer agents, distributes sub-tasks based on
    peer capabilities, and aggregates their results.
    """

    def __init__(self, agent_id: str):
        super().__init__(
            agent_id,
            capabilities=["coordination", "reasoning", "decision_making"],
        )
        self._peers: Dict[str, CognitiveAgent] = {}   # agent_id -> agent
        self._delegation_log: List[Dict[str, Any]] = []

    # ------------------------------------------------------------------
    # Peer management
    # ------------------------------------------------------------------

    def register_peer(self, agent: CognitiveAgent):
        """Register a peer agent that this coordinator can delegate to."""
        self._peers[agent.agent_id] = agent
        logger.debug(
            "CoordinationAgent %s: registered peer %s", self.agent_id, agent.agent_id
        )

    def deregister_peer(self, agent_id: str):
        """Remove a peer agent from the registry."""
        self._peers.pop(agent_id, None)

    # ------------------------------------------------------------------
    # Task routing override
    # ------------------------------------------------------------------

    def execute_task(self, task: Dict[str, Any]) -> Dict[str, Any]:
        """
        Try to delegate to the most capable peer; fall back to self.
        """
        required = task.get("required_capabilities", [])
        if required:
            delegate = self._find_best_peer(required)
            if delegate and delegate.agent_id != self.agent_id:
                return self._delegate(delegate, task)

        return super().execute_task(task)

    # ------------------------------------------------------------------
    # Coordination helpers
    # ------------------------------------------------------------------

    def broadcast_task(self, task: Dict[str, Any]) -> Dict[str, List[Any]]:
        """
        Send the same task to every registered peer and collect results.
        """
        results = {}
        for peer_id, peer in list(self._peers.items()):
            try:
                results[peer_id] = peer.execute_task(dict(task))
            except Exception as exc:
                logger.warning(
                    "CoordinationAgent %s: peer %s failed: %s",
                    self.agent_id, peer_id, exc,
                )
                results[peer_id] = {"success": False, "error": str(exc)}
        return results

    def gather_status(self) -> Dict[str, Any]:
        """Return status dict for this agent and all peers."""
        statuses = {self.agent_id: self.get_status()}
        for peer_id, peer in self._peers.items():
            statuses[peer_id] = peer.get_status()
        return statuses

    # ------------------------------------------------------------------
    # Internal helpers
    # ------------------------------------------------------------------

    def _find_best_peer(self, required: List[str]) -> Optional[CognitiveAgent]:
        """Return the idle peer with the most matching capabilities."""
        best: Optional[CognitiveAgent] = None
        best_score = -1

        for peer in self._peers.values():
            if peer.get_status().get("state") not in ("idle", "active"):
                continue
            score = sum(1 for cap in required if cap in peer.capabilities)
            if score > best_score:
                best_score = score
                best = peer

        return best

    def _delegate(self, peer: CognitiveAgent,
                  task: Dict[str, Any]) -> Dict[str, Any]:
        """Delegate *task* to *peer* and record the delegation."""
        logger.info(
            "CoordinationAgent %s: delegating task '%s' to %s",
            self.agent_id, task.get("type", "?"), peer.agent_id,
        )
        result = peer.execute_task(task)
        self._delegation_log.append({
            "task_type": task.get("type"),
            "delegated_to": peer.agent_id,
            "success": result.get("success", False),
        })
        return result

    def get_delegation_log(self) -> List[Dict[str, Any]]:
        """Return a copy of the delegation history."""
        return list(self._delegation_log)
