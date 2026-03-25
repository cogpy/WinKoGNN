"""NT4 Process Manager."""

import logging
import threading
import time
from typing import Dict, List, Optional, Any
from dataclasses import dataclass, field
from enum import Enum

logger = logging.getLogger(__name__)


class ProcessState(Enum):
    CREATED = "created"
    RUNNING = "running"
    SUSPENDED = "suspended"
    TERMINATED = "terminated"


@dataclass
class ProcessRecord:
    """Tracks state for a single (simulated) NT4 process."""
    process_id: int
    context_id: str
    program_path: str
    state: ProcessState = ProcessState.CREATED
    created_at: float = field(default_factory=time.time)
    terminated_at: Optional[float] = None
    exit_code: Optional[int] = None
    threads: List[int] = field(default_factory=list)


class NT4ProcessManager:
    """
    Manages processes created within NT4 kernel contexts.

    Tracks process and thread lifecycles, maintains exit codes,
    and provides process listing / statistics.
    """

    def __init__(self):
        self._lock = threading.Lock()
        self.processes: Dict[int, ProcessRecord] = {}
        self._pid_counter: int = 0x0100  # simulated PID base
        self._tid_counter: int = 0x1000  # monotonic thread ID counter
        logger.info("NT4ProcessManager initialized")

    # ------------------------------------------------------------------
    # Process lifecycle
    # ------------------------------------------------------------------

    def create_process(self, context_id: str,
                       program_path: str = "<kernel>") -> int:
        """
        Register a new (simulated) process for *context_id*.

        Returns the assigned process ID.
        """
        with self._lock:
            pid = self._pid_counter
            self._pid_counter += 1
            record = ProcessRecord(
                process_id=pid,
                context_id=context_id,
                program_path=program_path,
                state=ProcessState.RUNNING,
            )
            self.processes[pid] = record
            logger.debug(
                "Created process PID=%d for context '%s' (%s)",
                pid, context_id, program_path,
            )
            return pid

    def terminate_process(self, process_id: int,
                          exit_code: int = 0) -> bool:
        """Mark a process as terminated."""
        with self._lock:
            record = self.processes.get(process_id)
            if record is None:
                logger.warning("Process PID=%d not found", process_id)
                return False
            if record.state == ProcessState.TERMINATED:
                logger.warning("Process PID=%d already terminated", process_id)
                return False
            record.state = ProcessState.TERMINATED
            record.terminated_at = time.time()
            record.exit_code = exit_code
            logger.debug("Terminated process PID=%d (exit_code=%d)", process_id, exit_code)
            return True

    def suspend_process(self, process_id: int) -> bool:
        """Suspend a running process."""
        with self._lock:
            record = self.processes.get(process_id)
            if record is None or record.state != ProcessState.RUNNING:
                return False
            record.state = ProcessState.SUSPENDED
            logger.debug("Suspended process PID=%d", process_id)
            return True

    def resume_process(self, process_id: int) -> bool:
        """Resume a suspended process."""
        with self._lock:
            record = self.processes.get(process_id)
            if record is None or record.state != ProcessState.SUSPENDED:
                return False
            record.state = ProcessState.RUNNING
            logger.debug("Resumed process PID=%d", process_id)
            return True

    # ------------------------------------------------------------------
    # Thread management
    # ------------------------------------------------------------------

    def create_thread(self, process_id: int) -> Optional[int]:
        """Create a (simulated) thread within *process_id*."""
        with self._lock:
            record = self.processes.get(process_id)
            if record is None or record.state == ProcessState.TERMINATED:
                logger.warning(
                    "Cannot create thread: process PID=%d not available", process_id
                )
                return None
            tid = self._tid_counter
            self._tid_counter += 1
            record.threads.append(tid)
            logger.debug("Created thread TID=%d in PID=%d", tid, process_id)
            return tid

    def terminate_thread(self, process_id: int, thread_id: int) -> bool:
        """Remove a thread from its parent process."""
        with self._lock:
            record = self.processes.get(process_id)
            if record is None:
                return False
            if thread_id in record.threads:
                record.threads.remove(thread_id)
                logger.debug("Terminated thread TID=%d in PID=%d", thread_id, process_id)
                return True
            return False

    # ------------------------------------------------------------------
    # Queries
    # ------------------------------------------------------------------

    def get_process(self, process_id: int) -> Optional[Dict[str, Any]]:
        """Return process info dict, or None if not found."""
        with self._lock:
            record = self.processes.get(process_id)
            if record is None:
                return None
            return {
                "process_id": record.process_id,
                "context_id": record.context_id,
                "program_path": record.program_path,
                "state": record.state.value,
                "created_at": record.created_at,
                "terminated_at": record.terminated_at,
                "exit_code": record.exit_code,
                "thread_count": len(record.threads),
            }

    def list_processes(self, context_id: Optional[str] = None,
                       state: Optional[ProcessState] = None) -> List[Dict[str, Any]]:
        """List processes, optionally filtered by context and/or state."""
        with self._lock:
            results = []
            for record in self.processes.values():
                if context_id and record.context_id != context_id:
                    continue
                if state and record.state != state:
                    continue
                results.append({
                    "process_id": record.process_id,
                    "context_id": record.context_id,
                    "state": record.state.value,
                    "thread_count": len(record.threads),
                })
            return results

    def get_statistics(self) -> Dict[str, Any]:
        """Return process manager statistics."""
        with self._lock:
            states: Dict[str, int] = {}
            for record in self.processes.values():
                key = record.state.value
                states[key] = states.get(key, 0) + 1
            return {
                "total_processes": len(self.processes),
                "by_state": states,
            }
