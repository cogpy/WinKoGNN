"""Resource Allocator for hypervisor."""

import logging
import threading
from typing import Dict, Optional, Tuple

logger = logging.getLogger(__name__)


class ResourceAllocator:
    """
    Tracks and allocates shared system resources (memory and CPU) for VMs.

    Provides reservation, release, and utilization queries with
    thread-safe access via an internal lock.
    """

    def __init__(self, total_memory_kb: int = 1024 * 1024,
                 total_cpu_units: float = 100.0):
        self.total_memory_kb = total_memory_kb
        self.total_cpu_units = total_cpu_units

        self._lock = threading.Lock()
        self._allocations: Dict[str, Tuple[int, float]] = {}  # vm_id -> (mem_kb, cpu)

        logger.info(
            "ResourceAllocator initialized (memory=%dKB, cpu=%.1f%%)",
            total_memory_kb, total_cpu_units,
        )

    # ------------------------------------------------------------------
    # Allocation
    # ------------------------------------------------------------------

    def reserve(self, resource_id: str, memory_kb: int,
                cpu_quota: float) -> bool:
        """
        Reserve *memory_kb* KiB and *cpu_quota* % of CPU for *resource_id*.

        Returns True on success, False if resources are insufficient.
        """
        with self._lock:
            if resource_id in self._allocations:
                logger.warning("Resource %s already allocated", resource_id)
                return False

            used_mem, used_cpu = self._used()
            if used_mem + memory_kb > self.total_memory_kb:
                logger.error(
                    "Insufficient memory: requested %dKB, available %dKB",
                    memory_kb, self.total_memory_kb - used_mem,
                )
                return False
            if used_cpu + cpu_quota > self.total_cpu_units:
                logger.error(
                    "Insufficient CPU: requested %.1f%%, available %.1f%%",
                    cpu_quota, self.total_cpu_units - used_cpu,
                )
                return False

            self._allocations[resource_id] = (memory_kb, cpu_quota)
            logger.debug(
                "Reserved resources for %s: mem=%dKB, cpu=%.1f%%",
                resource_id, memory_kb, cpu_quota,
            )
            return True

    def release(self, resource_id: str) -> bool:
        """Release resources held by *resource_id*."""
        with self._lock:
            if resource_id not in self._allocations:
                logger.warning("No allocation found for %s", resource_id)
                return False
            del self._allocations[resource_id]
            logger.debug("Released resources for %s", resource_id)
            return True

    # ------------------------------------------------------------------
    # Queries
    # ------------------------------------------------------------------

    def available_memory_kb(self) -> int:
        """Return free memory in KiB."""
        with self._lock:
            used, _ = self._used()
            return self.total_memory_kb - used

    def available_cpu(self) -> float:
        """Return free CPU quota in percent."""
        with self._lock:
            _, used = self._used()
            return self.total_cpu_units - used

    def utilization(self) -> Dict[str, float]:
        """Return memory and CPU utilization as fractions (0.0 – 1.0)."""
        with self._lock:
            used_mem, used_cpu = self._used()
            return {
                "memory": used_mem / max(self.total_memory_kb, 1),
                "cpu": used_cpu / max(self.total_cpu_units, 1),
            }

    def get_allocation(self, resource_id: str) -> Optional[Tuple[int, float]]:
        """Return (memory_kb, cpu_quota) for *resource_id*, or None."""
        with self._lock:
            return self._allocations.get(resource_id)

    def list_allocations(self) -> Dict[str, Tuple[int, float]]:
        """Return a snapshot of all current allocations."""
        with self._lock:
            return dict(self._allocations)

    # ------------------------------------------------------------------
    # Internal helpers
    # ------------------------------------------------------------------

    def _used(self) -> Tuple[int, float]:
        """Return (total_used_memory_kb, total_used_cpu) without locking."""
        mem = sum(v[0] for v in self._allocations.values())
        cpu = sum(v[1] for v in self._allocations.values())
        return mem, cpu
