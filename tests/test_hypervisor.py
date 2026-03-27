"""
Unit tests for opencog_workbench.hypervisor.agent_zero (AgentZeroHypervisor).
"""

import pytest
from opencog_workbench.hypervisor.agent_zero import (
    AgentZeroHypervisor,
    HypervisorState,
    VirtualMachineContext,
)


# ---------------------------------------------------------------------------
# Initialisation
# ---------------------------------------------------------------------------

class TestHypervisorInit:
    def test_initializes_in_running_state(self, hypervisor):
        assert hypervisor.state == HypervisorState.RUNNING

    def test_initial_resource_allocation_zero(self, hypervisor):
        assert hypervisor.allocated_memory_kb == 0
        assert hypervisor.allocated_cpu_units == pytest.approx(0.0)

    def test_resource_totals_match_config(self, hypervisor):
        from tests.conftest import TEST_CONFIG
        cfg = TEST_CONFIG["hypervisor"]
        assert hypervisor.total_memory_kb == cfg["total_memory_kb"]
        assert hypervisor.total_cpu_units == cfg["total_cpu_units"]


# ---------------------------------------------------------------------------
# VM lifecycle: create / start / stop / destroy
# ---------------------------------------------------------------------------

class TestVMLifecycle:
    def test_create_vm_returns_id(self, hypervisor):
        vm_id = hypervisor.create_vm("agent_lifecycle", memory_kb=1024, cpu_quota=5.0)
        assert vm_id is not None

    def test_create_vm_allocates_resources(self, hypervisor):
        hypervisor.create_vm("agent_alloc", memory_kb=2048, cpu_quota=10.0)
        assert hypervisor.allocated_memory_kb == 2048
        assert hypervisor.allocated_cpu_units == pytest.approx(10.0)

    def test_start_vm(self, hypervisor):
        vm_id = hypervisor.create_vm("agent_start", memory_kb=512, cpu_quota=5.0)
        assert hypervisor.start_vm(vm_id) is True
        info = hypervisor.get_vm_info(vm_id)
        assert info["state"] == "running"

    def test_stop_vm(self, hypervisor):
        vm_id = hypervisor.create_vm("agent_stop", memory_kb=512, cpu_quota=5.0)
        hypervisor.start_vm(vm_id)
        assert hypervisor.stop_vm(vm_id) is True
        info = hypervisor.get_vm_info(vm_id)
        assert info["state"] == "stopped"

    def test_destroy_vm_releases_resources(self, hypervisor):
        vm_id = hypervisor.create_vm("agent_destroy", memory_kb=4096, cpu_quota=20.0)
        hypervisor.destroy_vm(vm_id)
        assert hypervisor.allocated_memory_kb == 0
        assert hypervisor.allocated_cpu_units == pytest.approx(0.0)

    def test_destroy_vm_removes_from_registry(self, hypervisor):
        vm_id = hypervisor.create_vm("agent_gone", memory_kb=1024, cpu_quota=5.0)
        hypervisor.destroy_vm(vm_id)
        assert hypervisor.get_vm_info(vm_id) is None

    def test_destroy_nonexistent_vm_returns_false(self, hypervisor):
        assert hypervisor.destroy_vm("does_not_exist") is False

    def test_start_nonexistent_vm_returns_false(self, hypervisor):
        assert hypervisor.start_vm("ghost_vm") is False

    def test_stop_nonexistent_vm_returns_false(self, hypervisor):
        assert hypervisor.stop_vm("ghost_vm") is False


# ---------------------------------------------------------------------------
# Suspend / resume
# ---------------------------------------------------------------------------

class TestSuspendResume:
    def test_suspend_and_resume(self, hypervisor):
        vm_id = hypervisor.create_vm("agent_susp", memory_kb=512, cpu_quota=5.0)
        hypervisor.start_vm(vm_id)
        assert hypervisor.suspend_vm(vm_id) is True
        assert hypervisor.get_vm_info(vm_id)["state"] == "suspended"
        assert hypervisor.resume_vm(vm_id) is True
        assert hypervisor.get_vm_info(vm_id)["state"] == "running"

    def test_resume_non_suspended_returns_false(self, hypervisor):
        vm_id = hypervisor.create_vm("agent_no_susp", memory_kb=512, cpu_quota=5.0)
        hypervisor.start_vm(vm_id)
        assert hypervisor.resume_vm(vm_id) is False  # not suspended


# ---------------------------------------------------------------------------
# Resource limits
# ---------------------------------------------------------------------------

class TestResourceLimits:
    def test_memory_exhaustion_returns_none(self, hypervisor):
        from tests.conftest import TEST_CONFIG
        total = TEST_CONFIG["hypervisor"]["total_memory_kb"]
        vm_id = hypervisor.create_vm("big_agent", memory_kb=total + 1, cpu_quota=1.0)
        assert vm_id is None

    def test_cpu_exhaustion_returns_none(self, hypervisor):
        from tests.conftest import TEST_CONFIG
        total = TEST_CONFIG["hypervisor"]["total_cpu_units"]
        vm_id = hypervisor.create_vm("cpu_hog", memory_kb=1024, cpu_quota=total + 1.0)
        assert vm_id is None

    def test_multiple_vms_aggregate_resources(self, hypervisor):
        hypervisor.create_vm("a1", memory_kb=1024, cpu_quota=10.0)
        hypervisor.create_vm("a2", memory_kb=2048, cpu_quota=20.0)
        assert hypervisor.allocated_memory_kb == 3072
        assert hypervisor.allocated_cpu_units == pytest.approx(30.0)


# ---------------------------------------------------------------------------
# System resources report
# ---------------------------------------------------------------------------

class TestSystemResources:
    def test_resource_report_keys(self, hypervisor):
        report = hypervisor.get_system_resources()
        assert "memory" in report
        assert "cpu" in report
        assert "vms" in report

    def test_resource_utilisation_after_create(self, hypervisor):
        hypervisor.create_vm("util_agent", memory_kb=1024, cpu_quota=5.0)
        report = hypervisor.get_system_resources()
        assert report["memory"]["allocated_kb"] == 1024
        assert report["vms"]["total"] == 1

    def test_running_vm_count(self, hypervisor):
        vm_id = hypervisor.create_vm("run_agent", memory_kb=512, cpu_quota=5.0)
        hypervisor.start_vm(vm_id)
        report = hypervisor.get_system_resources()
        assert report["vms"]["running"] >= 1


# ---------------------------------------------------------------------------
# Shutdown
# ---------------------------------------------------------------------------

class TestShutdown:
    def test_shutdown_clears_all_vms(self):
        from tests.conftest import TEST_CONFIG
        hv = AgentZeroHypervisor(TEST_CONFIG["hypervisor"])
        hv.create_vm("s1", memory_kb=512, cpu_quota=5.0)
        hv.create_vm("s2", memory_kb=512, cpu_quota=5.0)
        hv.shutdown()
        assert len(hv.vm_contexts) == 0
        assert hv.state == HypervisorState.SHUTDOWN

    def test_shutdown_releases_all_resources(self):
        from tests.conftest import TEST_CONFIG
        hv = AgentZeroHypervisor(TEST_CONFIG["hypervisor"])
        hv.create_vm("r1", memory_kb=1024, cpu_quota=10.0)
        hv.shutdown()
        assert hv.allocated_memory_kb == 0
        assert hv.allocated_cpu_units == pytest.approx(0.0)
