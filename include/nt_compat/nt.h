/*++
 * nt.h — Windows NT Master Include (Cross-Platform Compatibility Shim)
 *
 * Part of WinKoGNN Unified AGI-OS Build
 *
 * This is the top-level NT header that most user-mode NT programs and
 * kernel-mode test utilities include.  On Windows, this pulls in the
 * entire NT DDK type system.  Our shim provides the subset needed by
 * the WinKoGNN private/ntos/ source tree.
 *--*/

#ifndef _NT_H_COMPAT_
#define _NT_H_COMPAT_

/* Pull in base definitions first */
#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */
/* Process and Thread types (forward declarations)                     */
/* ------------------------------------------------------------------ */
typedef struct _EPROCESS    EPROCESS, *PEPROCESS;
typedef struct _ETHREAD     ETHREAD, *PETHREAD;
typedef struct _KPROCESS    KPROCESS, *PKPROCESS;
typedef struct _KTHREAD     KTHREAD, *PKTHREAD;

/* Minimal KPROCESS for kernel compilation */
typedef struct _KPROCESS {
    DISPATCHER_HEADER   Header;
    LIST_ENTRY          ProfileListHead;
    ULONG_PTR           DirectoryTableBase;
    ULONG_PTR           LdtDescriptor;
    ULONG_PTR           Int21Descriptor;
    USHORT              IopmOffset;
    UCHAR               Iopl;
    BOOLEAN             VdmFlag;
    ULONG               ActiveProcessors;
    ULONG               KernelTime;
    ULONG               UserTime;
    LIST_ENTRY          ReadyListHead;
    LIST_ENTRY          SwapListEntry;
    LIST_ENTRY          ThreadListHead;
    KSPIN_LOCK          ProcessLock;
    LONG                Affinity;
    USHORT              StackCount;
    CHAR                BasePriority;
    CHAR                ThreadQuantum;
    BOOLEAN             AutoAlignment;
    UCHAR               State;
    UCHAR               ThreadSeed;
    BOOLEAN             DisableBoost;
    UCHAR               PowerState;
    UCHAR               DisableQuantum;
    UCHAR               Spare[2];
} KPROCESS;

/* Minimal KTHREAD for kernel compilation */
typedef struct _KTHREAD {
    DISPATCHER_HEADER   Header;
    LIST_ENTRY          MutantListHead;
    PVOID               InitialStack;
    PVOID               StackLimit;
    PVOID               Teb;
    PVOID               TlsArray;
    PVOID               KernelStack;
    BOOLEAN             DebugActive;
    UCHAR               State;
    BOOLEAN             Alerted[2];
    UCHAR               Iopl;
    UCHAR               NpxState;
    CHAR                Saturation;
    CHAR                Priority;
    PVOID               ApcState;
    ULONG               ContextSwitches;
    LONG                WaitStatus;
    KIRQL               WaitIrql;
    CHAR                WaitMode;
    BOOLEAN             WaitNext;
    UCHAR               WaitReason;
    PVOID               WaitBlockList;
    LIST_ENTRY          WaitListEntry;
    ULONG               WaitTime;
    CHAR                BasePriority;
    UCHAR               DecrementCount;
    CHAR                PriorityDecrement;
    CHAR                Quantum;
    PVOID               WaitBlock;
    PVOID               LegoData;
    ULONG               KernelApcDisable;
    LONG                Affinity;
    BOOLEAN             SystemAffinityActive;
    UCHAR               PowerState;
    UCHAR               NpxIrql;
    UCHAR               Pad[1];
    PVOID               ServiceTable;
    PVOID               Queue;
    KSPIN_LOCK          ApcQueueLock;
    KTIMER              Timer;
    LIST_ENTRY          QueueListEntry;
    LONG                Affinity2;
    BOOLEAN             Preempted;
    BOOLEAN             ProcessReadyQueue;
    BOOLEAN             KernelStackResident;
    UCHAR               NextProcessor;
    PVOID               CallbackStack;
    PVOID               Win32Thread;
    PVOID               TrapFrame;
    PVOID               ApcStatePointer[2];
    CHAR                PreviousMode;
    BOOLEAN             EnableStackSwap;
    BOOLEAN             LargeStack;
    UCHAR               ResourceIndex;
    ULONG               KernelTime;
    ULONG               UserTime;
    PVOID               SavedApcState;
    BOOLEAN             Alertable;
    UCHAR               ApcStateIndex;
    BOOLEAN             ApcQueueable;
    BOOLEAN             AutoAlignment;
    PVOID               StackBase;
    PVOID               SuspendApc;
    PVOID               SuspendSemaphore;
    LIST_ENTRY          ThreadListEntry;
    CHAR                FreezeCount;
    CHAR                SuspendCount;
    UCHAR               IdealProcessor;
    BOOLEAN             DisableBoost;
} KTHREAD;

/* ------------------------------------------------------------------ */
/* System call stubs (no-op implementations for cross-platform build)  */
/* ------------------------------------------------------------------ */

/* Process management */
NTSTATUS NTAPI NtCreateProcess(
    OUT PHANDLE ProcessHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN HANDLE ParentProcess,
    IN BOOLEAN InheritObjectTable,
    IN HANDLE SectionHandle,
    IN HANDLE DebugPort,
    IN HANDLE ExceptionPort
);

NTSTATUS NTAPI NtTerminateProcess(
    IN HANDLE ProcessHandle,
    IN NTSTATUS ExitStatus
);

/* Thread management */
NTSTATUS NTAPI NtCreateThread(
    OUT PHANDLE ThreadHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN HANDLE ProcessHandle,
    OUT PCLIENT_ID ClientId,
    IN PVOID ThreadContext,
    IN PVOID InitialTeb,
    IN BOOLEAN CreateSuspended
);

/* Memory management */
NTSTATUS NTAPI NtAllocateVirtualMemory(
    IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN ULONG_PTR ZeroBits,
    IN OUT PSIZE_T RegionSize,
    IN ULONG AllocationType,
    IN ULONG Protect
);

NTSTATUS NTAPI NtFreeVirtualMemory(
    IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN OUT PSIZE_T RegionSize,
    IN ULONG FreeType
);

/* Object management */
NTSTATUS NTAPI NtClose(IN HANDLE Handle);

NTSTATUS NTAPI NtWaitForSingleObject(
    IN HANDLE Handle,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout
);

/* Event management */
NTSTATUS NTAPI NtCreateEvent(
    OUT PHANDLE EventHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN ULONG EventType,
    IN BOOLEAN InitialState
);

NTSTATUS NTAPI NtSetEvent(
    IN HANDLE EventHandle,
    OUT PLONG PreviousState
);

/* Section (memory-mapped file) management */
NTSTATUS NTAPI NtCreateSection(
    OUT PHANDLE SectionHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN PLARGE_INTEGER MaximumSize,
    IN ULONG SectionPageProtection,
    IN ULONG AllocationAttributes,
    IN HANDLE FileHandle
);

/* LPC (Local Procedure Call) */
NTSTATUS NTAPI NtCreatePort(
    OUT PHANDLE PortHandle,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN ULONG MaxConnectionInfoLength,
    IN ULONG MaxMessageLength,
    IN ULONG MaxPoolUsage
);

/* Memory allocation constants */
#define MEM_COMMIT      0x00001000
#define MEM_RESERVE     0x00002000
#define MEM_DECOMMIT    0x00004000
#define MEM_RELEASE     0x00008000

#define PAGE_NOACCESS           0x01
#define PAGE_READONLY           0x02
#define PAGE_READWRITE          0x04
#define PAGE_WRITECOPY          0x08
#define PAGE_EXECUTE            0x10
#define PAGE_EXECUTE_READ       0x20
#define PAGE_EXECUTE_READWRITE  0x40
#define PAGE_GUARD              0x100
#define PAGE_NOCACHE            0x200

/* ------------------------------------------------------------------ */
/* Kernel-mode function stubs                                          */
/* ------------------------------------------------------------------ */

/* Spinlock operations */
static inline void KeInitializeSpinLock(PKSPIN_LOCK SpinLock) {
    SpinLock->Lock = 0;
}

static inline void KeAcquireSpinLock(PKSPIN_LOCK SpinLock, PKIRQL OldIrql) {
    *OldIrql = PASSIVE_LEVEL;
    while (__sync_lock_test_and_set(&SpinLock->Lock, 1)) { /* spin */ }
}

static inline void KeReleaseSpinLock(PKSPIN_LOCK SpinLock, KIRQL NewIrql) {
    (void)NewIrql;
    __sync_lock_release(&SpinLock->Lock);
}

/* Event operations */
static inline void KeInitializeEvent(PKEVENT Event, ULONG Type, BOOLEAN State) {
    Event->Header.Type = (UCHAR)Type;
    Event->Header.SignalState = State ? 1 : 0;
    InitializeListHead(&Event->Header.WaitListHead);
}

/* DPC operations */
static inline void KeInitializeDpc(PKDPC Dpc, PVOID DeferredRoutine, PVOID DeferredContext) {
    Dpc->DeferredRoutine = DeferredRoutine;
    Dpc->DeferredContext = DeferredContext;
}

/* Timer operations */
static inline void KeInitializeTimer(PKTIMER Timer) {
    Timer->Header.Type = 8; /* TimerNotificationObject */
    Timer->Header.SignalState = 0;
    InitializeListHead(&Timer->Header.WaitListHead);
    Timer->Dpc = NULL;
}

/* Pool allocation stubs */
static inline PVOID ExAllocatePool(POOL_TYPE PoolType, SIZE_T NumberOfBytes) {
    (void)PoolType;
    extern void *malloc(size_t);
    return malloc(NumberOfBytes);
}

static inline PVOID ExAllocatePoolWithTag(POOL_TYPE PoolType, SIZE_T NumberOfBytes, ULONG Tag) {
    (void)PoolType;
    (void)Tag;
    extern void *malloc(size_t);
    return malloc(NumberOfBytes);
}

static inline void ExFreePool(PVOID P) {
    extern void free(void *);
    free(P);
}

static inline void ExFreePoolWithTag(PVOID P, ULONG Tag) {
    (void)Tag;
    extern void free(void *);
    free(P);
}

/* IRQL management */
static inline KIRQL KeGetCurrentIrql(void) { return PASSIVE_LEVEL; }
static inline KIRQL KeRaiseIrqlToDpcLevel(void) { return PASSIVE_LEVEL; }
static inline void KeLowerIrql(KIRQL NewIrql) { (void)NewIrql; }

/* Debug print */
#include <stdio.h>
#define DbgPrint(...) fprintf(stderr, __VA_ARGS__)
#define KdPrint(x)    DbgPrint x

#ifdef __cplusplus
}
#endif

#endif /* _NT_H_COMPAT_ */
