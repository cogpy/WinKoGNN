"""
Unit tests for opencog_workbench.core.orchestrator (MultiAgentOrchestrator).
"""

import pytest
from opencog_workbench.core.orchestrator import (
    MultiAgentOrchestrator,
    AutonomousAgent,
    AgentState,
    TaskScheduler,
)
from opencog_workbench.core.atomspace import AtomSpaceManager, AtomType


# ---------------------------------------------------------------------------
# Minimal stub agent
# ---------------------------------------------------------------------------

class StubAgent(AutonomousAgent):
    """Simple agent for orchestrator tests."""

    def __init__(self, agent_id, capabilities=None):
        super().__init__(agent_id, "stub", capabilities or [])

    def execute_task(self, task):
        return {"status": "ok", "task": task}


# ---------------------------------------------------------------------------
# Orchestrator initialisation
# ---------------------------------------------------------------------------

class TestOrchestratorInit:
    def test_initializes_empty(self):
        orch = MultiAgentOrchestrator()
        assert len(orch.agents) == 0

    def test_status_keys(self):
        orch = MultiAgentOrchestrator()
        status = orch.get_system_status()
        assert "total_agents" in status
        assert "components" in status


# ---------------------------------------------------------------------------
# Agent registration
# ---------------------------------------------------------------------------

class TestAgentRegistration:
    def test_register_agent(self):
        orch = MultiAgentOrchestrator()
        agent = StubAgent("reg_agent_1")
        assert orch.register_agent(agent) is True
        assert "reg_agent_1" in orch.agents

    def test_register_duplicate_returns_false(self):
        orch = MultiAgentOrchestrator()
        agent = StubAgent("dup_agent")
        orch.register_agent(agent)
        assert orch.register_agent(agent) is False

    def test_unregister_agent(self):
        orch = MultiAgentOrchestrator()
        agent = StubAgent("unreg_agent")
        orch.register_agent(agent)
        assert orch.unregister_agent("unreg_agent") is True
        assert "unreg_agent" not in orch.agents

    def test_unregister_nonexistent_returns_false(self):
        orch = MultiAgentOrchestrator()
        assert orch.unregister_agent("ghost_agent") is False

    def test_agent_state_becomes_terminated_on_unregister(self):
        orch = MultiAgentOrchestrator()
        agent = StubAgent("term_agent")
        orch.register_agent(agent)
        orch.unregister_agent("term_agent")
        assert agent.get_state() == AgentState.TERMINATED


# ---------------------------------------------------------------------------
# Task dispatch
# ---------------------------------------------------------------------------

class TestTaskDispatch:
    def test_dispatch_to_specific_agent(self):
        orch = MultiAgentOrchestrator()
        agent = StubAgent("target_dispatch_agent")
        orch.register_agent(agent)
        task = {"type": "reasoning", "data": "x"}
        assert orch.dispatch_task(task, target_agent_id="target_dispatch_agent") is True

    def test_dispatch_to_nonexistent_agent_returns_false(self):
        orch = MultiAgentOrchestrator()
        assert orch.dispatch_task({"type": "test"}, target_agent_id="no_such") is False

    def test_dispatch_auto_selects_agent(self):
        orch = MultiAgentOrchestrator()
        agent = StubAgent("auto_agent")
        orch.register_agent(agent)
        assert orch.dispatch_task({"type": "generic"}) is True

    def test_dispatch_without_agents_returns_false(self):
        orch = MultiAgentOrchestrator()
        assert orch.dispatch_task({"type": "generic"}) is False

    def test_dispatch_task_appears_in_agent_queue(self):
        orch = MultiAgentOrchestrator()
        agent = StubAgent("queue_agent")
        orch.register_agent(agent)
        task = {"type": "test_task", "id": 42}
        orch.dispatch_task(task, target_agent_id="queue_agent")
        queued = agent.task_queue.get_nowait()
        assert queued["id"] == 42


# ---------------------------------------------------------------------------
# Capability-based selection
# ---------------------------------------------------------------------------

class TestCapabilitySelection:
    def test_capable_agent_selected(self):
        orch = MultiAgentOrchestrator()
        general = StubAgent("general_agent", capabilities=["generic"])
        specialist = StubAgent("specialist_agent", capabilities=["reasoning"])
        orch.register_agent(general)
        orch.register_agent(specialist)
        task = {"type": "infer", "required_capability": "reasoning"}
        orch.dispatch_task(task)
        # Specialist should have the task
        assert not specialist.task_queue.empty()
        assert general.task_queue.empty()


# ---------------------------------------------------------------------------
# Orchestrate (batch dispatch)
# ---------------------------------------------------------------------------

class TestOrchestrate:
    def test_orchestrate_returns_result_dict(self):
        orch = MultiAgentOrchestrator()
        agent = StubAgent("batch_agent")
        orch.register_agent(agent)
        tasks = [{"type": "t1"}, {"type": "t2"}]
        results = orch.orchestrate(tasks)
        assert "completed" in results
        assert "failed" in results
        assert "pending" in results

    def test_orchestrate_no_agents_all_fail(self):
        orch = MultiAgentOrchestrator()
        results = orch.orchestrate([{"type": "x"}, {"type": "y"}])
        assert len(results["failed"]) == 2


# ---------------------------------------------------------------------------
# Component wiring
# ---------------------------------------------------------------------------

class TestComponentWiring:
    def test_set_atomspace_manager(self):
        orch = MultiAgentOrchestrator()
        asm = AtomSpaceManager()
        orch.set_atomspace_manager(asm)
        assert orch.atomspace_manager is asm

    def test_status_reflects_components(self):
        orch = MultiAgentOrchestrator()
        asm = AtomSpaceManager()
        orch.set_atomspace_manager(asm)
        status = orch.get_system_status()
        assert status["components"]["atomspace"] is True
        assert status["components"]["reasoning"] is False


# ---------------------------------------------------------------------------
# TaskScheduler
# ---------------------------------------------------------------------------

class TestTaskScheduler:
    def test_schedule_and_get(self):
        sched = TaskScheduler()
        task = {"type": "sched_task", "task_id": "t1"}
        sched.schedule_task(task, priority=5)
        result = sched.get_next_task()
        assert result is not None
        assert result["task_id"] == "t1"

    def test_empty_queue_returns_none(self):
        sched = TaskScheduler()
        assert sched.get_next_task() is None
