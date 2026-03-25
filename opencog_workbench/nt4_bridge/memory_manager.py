"""NT4 Memory Manager."""

import logging
import threading
from typing import Dict, List, Optional, Tuple

logger = logging.getLogger(__name__)


class MemoryPool:
    """A named memory pool with a fixed capacity (in bytes)."""

    def __init__(self, name: str, capacity_bytes: int):
        self.name = name
        self.capacity_bytes = capacity_bytes
        self.used_bytes: int = 0
        # address -> size mapping for outstanding allocations
        self._allocations: Dict[int, int] = {}
        self._next_address: int = 0x1000  # simulated base address

    def allocate(self, size_bytes: int) -> Optional[int]:
        if self.used_bytes + size_bytes > self.capacity_bytes:
            return None
        address = self._next_address
        self._allocations[address] = size_bytes
        self.used_bytes += size_bytes
        self._next_address += size_bytes
        return address

    def free(self, address: int) -> bool:
        size = self._allocations.pop(address, None)
        if size is None:
            return False
        self.used_bytes -= size
        return True

    @property
    def available_bytes(self) -> int:
        return self.capacity_bytes - self.used_bytes

    @property
    def utilization(self) -> float:
        return self.used_bytes / max(self.capacity_bytes, 1)


class NT4MemoryManager:
    """
    Manages named memory pools for NT4 kernel contexts.

    Each pool represents a region of (simulated) kernel memory.
    The manager provides thread-safe allocation and deallocation.
    """

    DEFAULT_POOL_SIZE = 64 * 1024 * 1024  # 64 MB per pool

    def __init__(self):
        self._lock = threading.Lock()
        self.memory_pools: Dict[str, MemoryPool] = {}
        logger.info("NT4MemoryManager initialized")

    # ------------------------------------------------------------------
    # Pool management
    # ------------------------------------------------------------------

    def create_pool(self, pool_name: str,
                    capacity_bytes: int = DEFAULT_POOL_SIZE) -> bool:
        """Create a named memory pool. Returns False if it already exists."""
        with self._lock:
            if pool_name in self.memory_pools:
                logger.warning("Memory pool '%s' already exists", pool_name)
                return False
            self.memory_pools[pool_name] = MemoryPool(pool_name, capacity_bytes)
            logger.debug("Created memory pool '%s' (%d bytes)", pool_name, capacity_bytes)
            return True

    def destroy_pool(self, pool_name: str) -> bool:
        """Destroy a named memory pool and free all its allocations."""
        with self._lock:
            if pool_name not in self.memory_pools:
                logger.warning("Memory pool '%s' not found", pool_name)
                return False
            del self.memory_pools[pool_name]
            logger.debug("Destroyed memory pool '%s'", pool_name)
            return True

    # ------------------------------------------------------------------
    # Allocation
    # ------------------------------------------------------------------

    def allocate(self, pool_name: str, size_bytes: int) -> Optional[int]:
        """
        Allocate *size_bytes* from *pool_name*.

        Creates a default-sized pool automatically if one does not exist.
        Returns a simulated address or None on failure.
        """
        with self._lock:
            if pool_name not in self.memory_pools:
                self.memory_pools[pool_name] = MemoryPool(
                    pool_name, self.DEFAULT_POOL_SIZE
                )
            address = self.memory_pools[pool_name].allocate(size_bytes)
            if address is None:
                logger.error(
                    "Failed to allocate %d bytes from pool '%s'", size_bytes, pool_name
                )
            else:
                logger.debug(
                    "Allocated %d bytes from pool '%s' → 0x%x",
                    size_bytes, pool_name, address,
                )
            return address

    def free(self, pool_name: str, address: int) -> bool:
        """Free a previously allocated address in *pool_name*."""
        with self._lock:
            pool = self.memory_pools.get(pool_name)
            if pool is None:
                logger.warning("Memory pool '%s' not found", pool_name)
                return False
            ok = pool.free(address)
            if ok:
                logger.debug("Freed 0x%x from pool '%s'", address, pool_name)
            else:
                logger.warning("Address 0x%x not found in pool '%s'", address, pool_name)
            return ok

    # ------------------------------------------------------------------
    # Queries
    # ------------------------------------------------------------------

    def get_pool_stats(self, pool_name: str) -> Optional[Dict]:
        """Return statistics for a pool, or None if not found."""
        with self._lock:
            pool = self.memory_pools.get(pool_name)
            if pool is None:
                return None
            return {
                "capacity_bytes": pool.capacity_bytes,
                "used_bytes": pool.used_bytes,
                "available_bytes": pool.available_bytes,
                "utilization": pool.utilization,
                "allocations": len(pool._allocations),
            }

    def get_all_stats(self) -> Dict[str, Dict]:
        """Return statistics for all pools."""
        with self._lock:
            return {
                name: {
                    "capacity_bytes": pool.capacity_bytes,
                    "used_bytes": pool.used_bytes,
                    "available_bytes": pool.available_bytes,
                    "utilization": pool.utilization,
                    "allocations": len(pool._allocations),
                }
                for name, pool in self.memory_pools.items()
            }
