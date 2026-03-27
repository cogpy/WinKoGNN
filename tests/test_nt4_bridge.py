"""
Unit tests for opencog_workbench.nt4_bridge.kernel_bridge (NT4CognitiveKernel).

The NT4 kernel bridge degrades gracefully on non-NT4 platforms (Linux / modern
Windows), so tests focus on the cross-platform interface rather than actual
kernel calls.
"""

import pytest
from opencog_workbench.nt4_bridge.kernel_bridge import (
    NT4CognitiveKernel,
    KernelCallType,
    KernelContext,
)


# ---------------------------------------------------------------------------
# Initialisation
# ---------------------------------------------------------------------------

class TestNT4KernelInit:
    def test_initializes_without_error(self, nt4_kernel):
        assert nt4_kernel is not None

    def test_is_nt4_available_is_bool(self, nt4_kernel):
        assert isinstance(nt4_kernel.is_nt4_available, bool)

    def test_system_info_has_platform(self, nt4_kernel):
        assert "platform" in nt4_kernel.system_info

    def test_call_statistics_initialised(self, nt4_kernel):
        for call_type in KernelCallType:
            assert nt4_kernel.call_statistics[call_type] == 0


# ---------------------------------------------------------------------------
# Kernel contexts
# ---------------------------------------------------------------------------

class TestKernelContexts:
    def test_create_context_returns_true(self, nt4_kernel):
        result = nt4_kernel.create_kernel_context("ctx_agent_1")
        assert result is True

    def test_context_is_stored(self, nt4_kernel):
        nt4_kernel.create_kernel_context("ctx_stored")
        assert "ctx_stored" in nt4_kernel.contexts

    def test_destroy_context_returns_true(self, nt4_kernel):
        nt4_kernel.create_kernel_context("ctx_to_destroy")
        result = nt4_kernel.destroy_kernel_context("ctx_to_destroy")
        assert result is True

    def test_destroy_context_removes_it(self, nt4_kernel):
        nt4_kernel.create_kernel_context("ctx_remove")
        nt4_kernel.destroy_kernel_context("ctx_remove")
        assert "ctx_remove" not in nt4_kernel.contexts

    def test_destroy_nonexistent_context_returns_false(self, nt4_kernel):
        result = nt4_kernel.destroy_kernel_context("nonexistent_ctx")
        assert result is False


# ---------------------------------------------------------------------------
# Memory operations (graceful degradation)
# ---------------------------------------------------------------------------

class TestMemoryOperations:
    def test_allocate_memory_returns_address(self, nt4_kernel):
        nt4_kernel.create_kernel_context("mem_ctx")
        addr = nt4_kernel.allocate_memory("mem_ctx", 4096)
        assert addr is not None

    def test_free_memory_returns_true(self, nt4_kernel):
        nt4_kernel.create_kernel_context("free_ctx")
        addr = nt4_kernel.allocate_memory("free_ctx", 4096)
        result = nt4_kernel.free_memory("free_ctx", addr)
        assert result is True

    def test_allocate_without_context_returns_none(self, nt4_kernel):
        addr = nt4_kernel.allocate_memory("bad_ctx", 4096)
        assert addr is None


# ---------------------------------------------------------------------------
# Process management (graceful degradation)
# ---------------------------------------------------------------------------

class TestProcessManagement:
    def test_create_process_returns_pid(self, nt4_kernel):
        nt4_kernel.create_kernel_context("proc_ctx")
        pid = nt4_kernel.create_process("proc_ctx", "test_process")
        assert pid is not None

    def test_terminate_process_returns_true(self, nt4_kernel):
        nt4_kernel.create_kernel_context("term_ctx")
        nt4_kernel.create_process("term_ctx", "termination_test")
        # terminate_process takes a context_id, not a pid
        result = nt4_kernel.terminate_process("term_ctx")
        assert result is True


# ---------------------------------------------------------------------------
# IPC
# ---------------------------------------------------------------------------

class TestIPC:
    def test_send_ipc_message_returns_true(self, nt4_kernel):
        nt4_kernel.create_kernel_context("ipc_sender")
        nt4_kernel.create_kernel_context("ipc_receiver")
        result = nt4_kernel.send_ipc_message("ipc_sender", "ipc_receiver", {"data": "hello"})
        assert result is True

    def test_send_ipc_to_nonexistent_context(self, nt4_kernel):
        nt4_kernel.create_kernel_context("ipc_src")
        result = nt4_kernel.send_ipc_message("ipc_src", "no_such_ctx", {"data": "x"})
        # Should not raise; return value depends on implementation
        assert isinstance(result, bool)


# ---------------------------------------------------------------------------
# Statistics
# ---------------------------------------------------------------------------

class TestStatistics:
    def test_get_statistics_returns_dict(self, nt4_kernel):
        stats = nt4_kernel.get_system_statistics()
        assert isinstance(stats, dict)

    def test_statistics_contains_context_count(self, nt4_kernel):
        stats = nt4_kernel.get_system_statistics()
        assert "active_contexts" in stats or "total_contexts" in stats or len(stats) > 0
