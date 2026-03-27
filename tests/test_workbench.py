"""
End-to-end tests for the OpenCogWorkbench (full system integration).

These tests exercise the public API of OpenCogWorkbench from initialization
through task dispatch, GNN training, AtomSpace persistence, and shutdown.
"""

import pytest
from opencog_workbench import OpenCogWorkbench
from opencog_workbench.core.atomspace import AtomType


# ---------------------------------------------------------------------------
# Lifecycle
# ---------------------------------------------------------------------------

class TestWorkbenchLifecycle:
    def test_creates_and_shuts_down(self, test_config):
        wb = OpenCogWorkbench(test_config)
        assert wb.is_initialized is True
        wb.shutdown()
        assert wb.is_initialized is False

    def test_context_manager(self, test_config):
        with OpenCogWorkbench(test_config) as wb:
            assert wb.is_initialized is True
        assert wb.is_initialized is False

    def test_components_available_after_init(self, workbench):
        assert workbench.atomspace is not None
        assert workbench.reasoning is not None
        assert workbench.gnn is not None
        assert workbench.hypervisor is not None
        assert workbench.nt4_kernel is not None
        assert workbench.orchestrator is not None


# ---------------------------------------------------------------------------
# Agent management
# ---------------------------------------------------------------------------

class TestAgentManagement:
    def test_create_cognitive_agent(self, workbench):
        assert workbench.create_agent("e2e_cognitive", agent_type="cognitive",
                                      memory_kb=1024, cpu_quota=5.0) is True

    def test_create_reasoning_agent(self, workbench):
        assert workbench.create_agent("e2e_reasoning", agent_type="reasoning",
                                      memory_kb=1024, cpu_quota=5.0) is True

    def test_create_learning_agent(self, workbench):
        assert workbench.create_agent("e2e_learning", agent_type="learning",
                                      memory_kb=1024, cpu_quota=5.0) is True

    def test_create_coordination_agent(self, workbench):
        assert workbench.create_agent("e2e_coordination", agent_type="coordination",
                                      memory_kb=1024, cpu_quota=5.0) is True

    def test_create_unknown_agent_type_returns_false(self, workbench):
        assert workbench.create_agent("e2e_unknown", agent_type="telekinesis",
                                      memory_kb=512, cpu_quota=2.0) is False

    def test_remove_agent(self, workbench):
        workbench.create_agent("e2e_removable", memory_kb=512, cpu_quota=2.0)
        assert workbench.remove_agent("e2e_removable") is True

    def test_agent_count_in_status(self, workbench):
        workbench.create_agent("e2e_count_agent", memory_kb=512, cpu_quota=2.0)
        status = workbench.get_system_status()
        assert status["orchestrator"]["total_agents"] >= 1


# ---------------------------------------------------------------------------
# AtomSpace via workbench
# ---------------------------------------------------------------------------

class TestWorkbenchAtomSpace:
    def test_add_atoms(self, workbench):
        atom_id = workbench.atomspace.add_atom(AtomType.CONCEPT, "e2e_concept")
        assert atom_id is not None

    def test_create_link(self, workbench):
        a = workbench.atomspace.add_atom(AtomType.CONCEPT, "node_a")
        b = workbench.atomspace.add_atom(AtomType.CONCEPT, "node_b")
        link = workbench.atomspace.create_link(AtomType.LINK, a, b)
        assert link is not None

    def test_query_pattern(self, workbench):
        workbench.atomspace.add_atom(AtomType.CONCEPT, "e2e_query_node")
        results = workbench.atomspace.query_pattern({"name": "e2e_query"})
        assert len(results) >= 1


# ---------------------------------------------------------------------------
# Task dispatch
# ---------------------------------------------------------------------------

class TestTaskDispatch:
    def test_dispatch_task_to_any_agent(self, workbench):
        workbench.create_agent("e2e_dispatch_agent", memory_kb=512, cpu_quota=5.0)
        task = {"type": "reasoning", "data": {"query": "is AI conscious?"}}
        assert workbench.dispatch_task(task) is True

    def test_dispatch_task_to_named_agent(self, workbench):
        workbench.create_agent("e2e_named_dispatch", memory_kb=512, cpu_quota=5.0)
        task = {"type": "learning", "data": {"samples": [1, 2, 3]}}
        assert workbench.dispatch_task(task, agent_id="e2e_named_dispatch") is True

    def test_orchestrate_multiple_tasks(self, workbench):
        workbench.create_agent("e2e_orch_agent", memory_kb=512, cpu_quota=5.0)
        tasks = [
            {"type": "reasoning"},
            {"type": "pattern_recognition"},
            {"type": "decision_making"},
        ]
        results = workbench.orchestrate_tasks(tasks)
        assert "completed" in results
        assert "failed" in results
        assert "pending" in results


# ---------------------------------------------------------------------------
# GNN training
# ---------------------------------------------------------------------------

class TestGNNTraining:
    def test_train_gnn(self, workbench):
        workbench.atomspace.add_atom(AtomType.CONCEPT, "gnn_node_1")
        workbench.atomspace.add_atom(AtomType.CONCEPT, "gnn_node_2")
        results = workbench.train_gnn(epochs=3)
        assert "final_loss" in results
        assert results["epochs"] == 3

    def test_node_embeddings_after_training(self, workbench):
        workbench.atomspace.add_atom(AtomType.CONCEPT, "embed_node")
        workbench.train_gnn(epochs=1)
        embeddings = workbench.get_node_embeddings()
        assert isinstance(embeddings, dict)
        assert len(embeddings) >= 1
        for emb in embeddings.values():
            assert isinstance(emb, list)


# ---------------------------------------------------------------------------
# AtomSpace persistence via workbench
# ---------------------------------------------------------------------------

class TestAtomSpacePersistence:
    def test_save_and_load_atomspace(self, workbench, tmp_path):
        workbench.atomspace.add_atom(AtomType.CONCEPT, "persist_e2e")
        path = str(tmp_path / "wb_atomspace.json")
        assert workbench.save_atomspace(path) is True

        # Load into a new workbench
        from tests.conftest import TEST_CONFIG
        wb2 = OpenCogWorkbench(TEST_CONFIG)
        assert wb2.load_atomspace(path) is True
        atoms = wb2.atomspace.get_atoms_by_name("persist_e2e")
        assert len(atoms) >= 1
        wb2.shutdown()

    def test_load_merge(self, workbench, tmp_path):
        workbench.atomspace.add_atom(AtomType.CONCEPT, "original_atom")
        path = str(tmp_path / "merge_wb.json")
        workbench.save_atomspace(path)

        workbench.atomspace.add_atom(AtomType.CONCEPT, "new_atom_after_save")
        workbench.load_atomspace(path, merge=True)

        names = {a.name for a in workbench.atomspace.atomspace.values()}
        assert "original_atom" in names
        assert "new_atom_after_save" in names


# ---------------------------------------------------------------------------
# System status
# ---------------------------------------------------------------------------

class TestSystemStatus:
    def test_status_has_required_keys(self, workbench):
        status = workbench.get_system_status()
        for key in ("workbench", "orchestrator", "atomspace", "reasoning",
                    "gnn", "hypervisor", "nt4_kernel"):
            assert key in status

    def test_workbench_initialized_flag(self, workbench):
        assert workbench.get_system_status()["workbench"]["initialized"] is True


# ---------------------------------------------------------------------------
# Full end-to-end workflow
# ---------------------------------------------------------------------------

class TestFullWorkflow:
    def test_complete_workflow(self, test_config):
        """
        Full workflow: create workbench → populate knowledge base →
        create agents → dispatch tasks → train GNN → check status → shutdown.
        """
        with OpenCogWorkbench(test_config) as wb:
            # Build knowledge base
            ai = wb.atomspace.add_atom(AtomType.CONCEPT, "artificial_intelligence")
            ml = wb.atomspace.add_atom(AtomType.CONCEPT, "machine_learning")
            nn = wb.atomspace.add_atom(AtomType.CONCEPT, "neural_networks")
            wb.atomspace.create_link(AtomType.LINK, ai, ml)
            wb.atomspace.create_link(AtomType.LINK, ml, nn)

            # Create agents
            assert wb.create_agent("worker_1", agent_type="cognitive",
                                   memory_kb=1024, cpu_quota=10.0)
            assert wb.create_agent("worker_2", agent_type="reasoning",
                                   memory_kb=1024, cpu_quota=10.0)

            # Dispatch tasks
            assert wb.dispatch_task({"type": "reasoning", "query": "relationship"})
            assert wb.dispatch_task({"type": "learning", "data": [1, 2, 3]})

            # Train GNN
            results = wb.train_gnn(epochs=2)
            assert results["final_loss"] >= 0.0

            # Activation spread
            wb.atomspace.spread_activation(ai, decay=0.8)

            # System status
            status = wb.get_system_status()
            assert status["orchestrator"]["total_agents"] == 2
            assert status["atomspace"]["total_atoms"] >= 3
