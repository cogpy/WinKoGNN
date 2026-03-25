"""
Windows NT4 Cognitive Kernel Bridge
Provides interface to Windows NT4 kernel for cognitive operations
Integrates with OpenCog workbench and Agent-Zero hypervisor
"""

import logging
import os
from typing import Dict, List, Any, Optional
from dataclasses import dataclass
from enum import Enum
import ctypes
import platform

logger = logging.getLogger(__name__)


class KernelCallType(Enum):
    """Types of kernel calls"""
    MEMORY_ALLOC = "memory_alloc"
    MEMORY_FREE = "memory_free"
    PROCESS_CREATE = "process_create"
    PROCESS_TERMINATE = "process_terminate"
    THREAD_CREATE = "thread_create"
    IPC_SEND = "ipc_send"
    IPC_RECEIVE = "ipc_receive"
    IO_OPERATION = "io_operation"


@dataclass
class KernelContext:
    """Kernel execution context"""
    context_id: str
    process_id: int
    thread_ids: List[int]
    memory_regions: List[Dict[str, Any]]
    kernel_objects: List[str]
    privilege_level: int = 0  # 0=kernel, 3=user


class NT4CognitiveKernel:
    """
    Bridge to Windows NT4 kernel for cognitive processing
    Provides low-level system integration for OpenCog workbench
    """
    
    def __init__(self, config: Optional[Dict[str, Any]] = None):
        self.config = config or {}
        
        # Kernel state
        self.is_nt4_available = self._check_nt4_availability()
        self.contexts: Dict[str, KernelContext] = {}
        
        # System information
        self.system_info = self._get_system_info()
        
        # Kernel call statistics
        self.call_statistics: Dict[KernelCallType, int] = {
            call_type: 0 for call_type in KernelCallType
        }
        
        # Memory management
        self.memory_manager = None
        
        # Process management
        self.process_manager = None
        
        logger.info(f"NT4CognitiveKernel initialized (NT4 available: {self.is_nt4_available})")
        
    def _check_nt4_availability(self) -> bool:
        """Check if running on Windows NT4"""
        try:
            system = platform.system()
            if system == "Windows":
                # Check version
                version = platform.version()
                # NT4 is version 4.0
                if "4.0" in version or "NT 4" in version:
                    return True
                else:
                    logger.info(f"Running on Windows but not NT4 (version: {version})")
                    return False
            else:
                logger.info(f"Not running on Windows (system: {system})")
                return False
        except Exception as e:
            logger.warning(f"Error checking NT4 availability: {e}")
            return False
            
    def _get_system_info(self) -> Dict[str, Any]:
        """Get system information"""
        return {
            'platform': platform.system(),
            'version': platform.version(),
            'architecture': platform.machine(),
            'processor': platform.processor(),
            'is_nt4': self.is_nt4_available
        }
        
    def create_kernel_context(self, context_id: str, privilege_level: int = 3) -> bool:
        """Create a kernel execution context"""
        if context_id in self.contexts:
            logger.warning(f"Kernel context {context_id} already exists")
            return False
            
        context = KernelContext(
            context_id=context_id,
            process_id=0,  # Will be set when process created
            thread_ids=[],
            memory_regions=[],
            kernel_objects=[],
            privilege_level=privilege_level
        )
        
        self.contexts[context_id] = context
        logger.info(f"Created kernel context {context_id}")
        return True
        
    def destroy_kernel_context(self, context_id: str) -> bool:
        """Destroy a kernel execution context"""
        if context_id not in self.contexts:
            logger.warning(f"Kernel context {context_id} not found")
            return False
            
        # Clean up resources
        context = self.contexts[context_id]
        
        # Free memory regions
        for region in context.memory_regions:
            self._free_memory(region)
            
        # Terminate threads and processes
        for thread_id in context.thread_ids:
            self._terminate_thread(thread_id)
            
        if context.process_id:
            self._terminate_process(context.process_id)
            
        del self.contexts[context_id]
        logger.info(f"Destroyed kernel context {context_id}")
        return True
        
    def allocate_memory(self, context_id: str, size_bytes: int) -> Optional[int]:
        """Allocate memory for a context"""
        if context_id not in self.contexts:
            logger.error(f"Kernel context {context_id} not found")
            return None
            
        context = self.contexts[context_id]
        
        # Simulate memory allocation
        memory_region = {
            'address': id(context) + len(context.memory_regions),
            'size': size_bytes,
            'allocated': True
        }
        
        context.memory_regions.append(memory_region)
        self.call_statistics[KernelCallType.MEMORY_ALLOC] += 1
        
        logger.debug(f"Allocated {size_bytes} bytes for context {context_id}")
        return memory_region['address']
        
    def free_memory(self, context_id: str, address: int) -> bool:
        """Free allocated memory"""
        if context_id not in self.contexts:
            logger.error(f"Kernel context {context_id} not found")
            return False
            
        context = self.contexts[context_id]
        
        # Find and free memory region
        for region in context.memory_regions:
            if region['address'] == address:
                region['allocated'] = False
                self.call_statistics[KernelCallType.MEMORY_FREE] += 1
                logger.debug(f"Freed memory at address {address}")
                return True
                
        logger.warning(f"Memory address {address} not found in context {context_id}")
        return False
        
    def _free_memory(self, region: Dict[str, Any]):
        """Internal memory free"""
        region['allocated'] = False
        
    def create_process(self, context_id: str, program_path: str = None) -> Optional[int]:
        """Create a process in the kernel context"""
        if context_id not in self.contexts:
            logger.error(f"Kernel context {context_id} not found")
            return None
            
        context = self.contexts[context_id]
        
        # Simulate process creation
        process_id = id(context) + len(self.contexts)
        context.process_id = process_id
        
        self.call_statistics[KernelCallType.PROCESS_CREATE] += 1
        logger.info(f"Created process {process_id} in context {context_id}")
        return process_id
        
    def terminate_process(self, context_id: str) -> bool:
        """Terminate the process in a context"""
        if context_id not in self.contexts:
            logger.error(f"Kernel context {context_id} not found")
            return False
            
        context = self.contexts[context_id]
        
        if not context.process_id:
            logger.warning(f"No process to terminate in context {context_id}")
            return False
            
        self._terminate_process(context.process_id)
        context.process_id = 0
        
        self.call_statistics[KernelCallType.PROCESS_TERMINATE] += 1
        logger.info(f"Terminated process in context {context_id}")
        return True
        
    def _terminate_process(self, process_id: int):
        """Internal process termination"""
        logger.debug(f"Terminating process {process_id}")
        
    def create_thread(self, context_id: str) -> Optional[int]:
        """Create a thread in the kernel context"""
        if context_id not in self.contexts:
            logger.error(f"Kernel context {context_id} not found")
            return None
            
        context = self.contexts[context_id]
        
        # Simulate thread creation
        thread_id = id(context) + len(context.thread_ids)
        context.thread_ids.append(thread_id)
        
        self.call_statistics[KernelCallType.THREAD_CREATE] += 1
        logger.debug(f"Created thread {thread_id} in context {context_id}")
        return thread_id
        
    def _terminate_thread(self, thread_id: int):
        """Internal thread termination"""
        logger.debug(f"Terminating thread {thread_id}")
        
    def send_ipc_message(self, source_context: str, target_context: str, 
                        message: Dict[str, Any]) -> bool:
        """Send inter-process communication message"""
        if source_context not in self.contexts or target_context not in self.contexts:
            logger.error("Source or target context not found")
            return False
            
        self.call_statistics[KernelCallType.IPC_SEND] += 1
        logger.debug(f"Sent IPC message from {source_context} to {target_context}")
        return True
        
    def perform_io_operation(self, context_id: str, operation: str, 
                            data: Any = None) -> Dict[str, Any]:
        """Perform I/O operation through kernel"""
        if context_id not in self.contexts:
            logger.error(f"Kernel context {context_id} not found")
            return {'success': False, 'error': 'Context not found'}
            
        self.call_statistics[KernelCallType.IO_OPERATION] += 1
        
        result = {
            'success': True,
            'operation': operation,
            'context': context_id
        }
        
        logger.debug(f"Performed I/O operation {operation} in context {context_id}")
        return result
        
    def get_context_info(self, context_id: str) -> Optional[Dict[str, Any]]:
        """Get information about a kernel context"""
        if context_id not in self.contexts:
            return None
            
        context = self.contexts[context_id]
        return {
            'context_id': context.context_id,
            'process_id': context.process_id,
            'num_threads': len(context.thread_ids),
            'num_memory_regions': len(context.memory_regions),
            'total_memory': sum(r['size'] for r in context.memory_regions if r['allocated']),
            'privilege_level': context.privilege_level
        }
        
    def get_system_statistics(self) -> Dict[str, Any]:
        """Get kernel bridge statistics"""
        return {
            'system_info': self.system_info,
            'total_contexts': len(self.contexts),
            'kernel_calls': {
                call_type.value: count 
                for call_type, count in self.call_statistics.items()
            },
            'total_calls': sum(self.call_statistics.values())
        }
        
    def set_memory_manager(self, memory_manager):
        """Set memory manager"""
        self.memory_manager = memory_manager
        logger.info("Memory manager connected")
        
    def set_process_manager(self, process_manager):
        """Set process manager"""
        self.process_manager = process_manager
        logger.info("Process manager connected")

    def discover_nt4_apis(self, source_root: Optional[str] = None) -> Dict[str, Any]:
        """
        Scan the NT4 kernel source tree and return a catalogue of available
        kernel APIs grouped by subsystem.

        Parameters
        ----------
        source_root:
            Path to the NT4 kernel source directory.  When *None* the method
            looks for ``private/ntos`` relative to this file's package root,
            which is the conventional location in this repository.

        Returns
        -------
        dict
            Serialisable summary produced by
            :meth:`NT4SourceAnalyzer.to_dict`.  If the source tree is not
            found an empty dict with a ``"warning"`` key is returned.
        """
        from .nt4_source_analyzer import NT4SourceAnalyzer

        if source_root is None:
            # Resolve <repo_root>/private/ntos
            pkg_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
            source_root = os.path.join(os.path.dirname(pkg_dir), 'private', 'ntos')

        if not os.path.isdir(source_root):
            logger.warning("NT4 source tree not found at %s", source_root)
            return {"warning": f"NT4 source tree not found at {source_root}"}

        analyzer = NT4SourceAnalyzer(source_root)
        analyzer.scan()
        logger.info("NT4 API discovery complete: %s", analyzer.summarize())
        return analyzer.to_dict()
