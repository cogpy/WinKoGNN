"""
Shared pytest fixtures for the OpenCog Workbench test suite.
"""

import sys
import os
import json
import tempfile

import pytest

# Ensure the package root is on sys.path when tests are run from any directory.
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)


# ---------------------------------------------------------------------------
# Lightweight config (small memory / CPU footprint for CI)
# ---------------------------------------------------------------------------
TEST_CONFIG = {
    "orchestrator": {
        "max_agents": 10,
        "task_queue_size": 100,
        "coordination_timeout": 5,
    },
    "hypervisor": {
        "total_memory_kb": 65536,   # 64 MB
        "total_cpu_units": 100,
        "nt4_integration": False,   # disabled in CI
        "default_isolation": "process",
    },
    "gnn": {
        "input_dim": 16,
        "hidden_dim": 32,
        "output_dim": 8,
        "num_layers": 2,
        "learning_rate": 0.01,
    },
    "atomspace": {
        "max_atoms": 10000,
        "attention_decay": 0.9,
        "enable_persistence": False,
    },
    "reasoning": {
        "max_inference_steps": 10,
        "confidence_threshold": 0.7,
        "enabled_rules": ["deduction", "modus_ponens", "similarity"],
    },
    "nt4_bridge": {
        "enable_kernel_calls": False,
        "default_privilege_level": 3,
        "memory_pool_size_kb": 4096,
    },
    "logging": {
        "level": "WARNING",
        "format": "%(asctime)s - %(name)s - %(levelname)s - %(message)s",
        "file": None,
    },
}


@pytest.fixture(scope="session")
def test_config():
    """Return the shared test configuration dictionary."""
    return TEST_CONFIG


@pytest.fixture
def atomspace():
    """Return a fresh AtomSpaceManager."""
    from opencog_workbench.core.atomspace import AtomSpaceManager
    return AtomSpaceManager(TEST_CONFIG["atomspace"])


@pytest.fixture
def reasoning_engine(atomspace):
    """Return a ReasoningEngine wired to *atomspace*."""
    from opencog_workbench.core.reasoning import ReasoningEngine
    return ReasoningEngine(atomspace, TEST_CONFIG["reasoning"])


@pytest.fixture
def gnn(atomspace):
    """Return a GraphNeuralNetwork wired to *atomspace*."""
    from opencog_workbench.gnn.graph_network import GraphNeuralNetwork
    net = GraphNeuralNetwork(TEST_CONFIG["gnn"])
    net.set_atomspace_manager(atomspace)
    return net


@pytest.fixture
def hypervisor():
    """Return an AgentZeroHypervisor with a small resource budget."""
    from opencog_workbench.hypervisor.agent_zero import AgentZeroHypervisor
    hv = AgentZeroHypervisor(TEST_CONFIG["hypervisor"])
    yield hv
    hv.shutdown()


@pytest.fixture
def nt4_kernel():
    """Return an NT4CognitiveKernel (gracefully degraded when not on NT4)."""
    from opencog_workbench.nt4_bridge.kernel_bridge import NT4CognitiveKernel
    return NT4CognitiveKernel(TEST_CONFIG["nt4_bridge"])


@pytest.fixture
def workbench():
    """Return a fully initialised OpenCogWorkbench with test config."""
    from opencog_workbench import OpenCogWorkbench
    wb = OpenCogWorkbench(TEST_CONFIG)
    yield wb
    wb.shutdown()


@pytest.fixture
def tmp_json(tmp_path):
    """Return a helper that writes a dict to a temp JSON file and returns the path."""
    def _write(data, filename="data.json"):
        path = str(tmp_path / filename)
        with open(path, "w") as f:
            json.dump(data, f)
        return path
    return _write
