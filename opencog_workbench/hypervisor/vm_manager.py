"""Virtual Machine Manager for hypervisor."""

import logging
from typing import Dict, List, Optional, Any

logger = logging.getLogger(__name__)


class VirtualMachineManager:
    """
    High-level manager for virtual machine lifecycle operations.

    Delegates low-level operations to AgentZeroHypervisor while providing
    a simplified interface for creating, querying, and managing agent VMs.
    """

    def __init__(self, hypervisor=None):
        self.hypervisor = hypervisor
        self._vm_metadata: Dict[str, Dict[str, Any]] = {}
        logger.info("VirtualMachineManager initialized")

    def set_hypervisor(self, hypervisor):
        """Attach an AgentZeroHypervisor instance."""
        self.hypervisor = hypervisor

    def create(self, agent_id: str, memory_kb: int = 65536,
               cpu_quota: float = 10.0, priority: int = 1,
               tags: Optional[List[str]] = None) -> Optional[str]:
        """Create a VM for *agent_id* and return its vm_id."""
        if self.hypervisor is None:
            logger.error("No hypervisor attached")
            return None

        vm_id = self.hypervisor.create_vm(
            agent_id=agent_id,
            memory_kb=memory_kb,
            cpu_quota=cpu_quota,
            priority=priority,
        )
        if vm_id:
            self._vm_metadata[vm_id] = {
                "agent_id": agent_id,
                "memory_kb": memory_kb,
                "cpu_quota": cpu_quota,
                "priority": priority,
                "tags": tags or [],
            }
        return vm_id

    def start(self, vm_id: str) -> bool:
        """Start a VM."""
        if self.hypervisor is None:
            return False
        return self.hypervisor.start_vm(vm_id)

    def stop(self, vm_id: str) -> bool:
        """Stop a VM."""
        if self.hypervisor is None:
            return False
        return self.hypervisor.stop_vm(vm_id)

    def destroy(self, vm_id: str) -> bool:
        """Destroy a VM and release its resources."""
        if self.hypervisor is None:
            return False
        ok = self.hypervisor.destroy_vm(vm_id)
        if ok:
            self._vm_metadata.pop(vm_id, None)
        return ok

    def list_vms(self, tag: Optional[str] = None) -> List[Dict[str, Any]]:
        """Return metadata for all (optionally tag-filtered) VMs."""
        vms = []
        for vm_id, meta in self._vm_metadata.items():
            if tag and tag not in meta.get("tags", []):
                continue
            entry = dict(meta)
            entry["vm_id"] = vm_id
            if self.hypervisor:
                ctx = self.hypervisor.get_vm_info(vm_id)
                entry["state"] = ctx.get("state") if ctx else "unknown"
            vms.append(entry)
        return vms

    def get_vm_info(self, vm_id: str) -> Optional[Dict[str, Any]]:
        """Return combined metadata and live state for a VM."""
        if self.hypervisor is None:
            return self._vm_metadata.get(vm_id)
        ctx = self.hypervisor.get_vm_info(vm_id)
        meta = self._vm_metadata.get(vm_id, {})
        if ctx is None and not meta:
            return None
        combined = dict(meta)
        if ctx:
            combined.update(ctx)
        return combined

    def get_statistics(self) -> Dict[str, Any]:
        """Return summary statistics."""
        total_mem = sum(m["memory_kb"] for m in self._vm_metadata.values())
        total_cpu = sum(m["cpu_quota"] for m in self._vm_metadata.values())
        return {
            "total_vms": len(self._vm_metadata),
            "total_memory_kb": total_mem,
            "total_cpu_quota": total_cpu,
        }
