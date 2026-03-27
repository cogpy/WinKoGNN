/*++
 * nt.h — Windows NT Master Include (Cross-Platform Compatibility Shim)
 *
 * Part of WinKoGNN Unified AGI-OS Build
 *
 * This is the top-level NT header that most user-mode NT programs and
 * kernel-mode test utilities include.  On Windows, this pulls in the
 * entire NT DDK type system.  Our shim provides the subset needed by
 * the WinKoGNN private/ntos/ source tree.
 *
 * CRITICAL DESIGN RULE:
 *   Types that are fully defined in the NT4 source tree's own headers
 *   (ke.h, ntosdef.h, etc.) MUST NOT be fully defined here.  We provide
 *   only forward declarations so the NT source's own definitions are
 *   authoritative.  Types that are NOT defined anywhere in the NT4 source
 *   (base scalars, NTSTATUS, etc.) are fully defined in ntdef.h.
 *--*/

#ifndef _NT_H_COMPAT_
#define _NT_H_COMPAT_

/* Pull in base definitions first */
#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */
/* Forward declarations for kernel objects                             */
/* These are FULLY DEFINED in ke.h — do NOT redefine here.            */
/* We only provide forward-declared pointer typedefs so that headers   */
/* included BEFORE ke.h can reference them.                            */
/* ------------------------------------------------------------------ */
struct _KPROCESS;
struct _KTHREAD;
struct _EPROCESS;
struct _ETHREAD;

typedef struct _KPROCESS    *PKPROCESS, *PRKPROCESS;
typedef struct _KTHREAD     *PKTHREAD, *PRKTHREAD;
typedef struct _EPROCESS    EPROCESS, *PEPROCESS;
typedef struct _ETHREAD     ETHREAD, *PETHREAD;

/* ------------------------------------------------------------------ */
/* TIME_FIELDS — used by arc.h, ntrtl.h, hal                          */
/* Defined here because it's NOT in ke.h/ntosdef.h                    */
/* ------------------------------------------------------------------ */
#ifndef _TIME_FIELDS_DEFINED
#define _TIME_FIELDS_DEFINED
typedef struct _TIME_FIELDS {
    CSHORT Year;
    CSHORT Month;
    CSHORT Day;
    CSHORT Hour;
    CSHORT Minute;
    CSHORT Second;
    CSHORT Milliseconds;
    CSHORT Weekday;
} TIME_FIELDS, *PTIME_FIELDS;
#endif

/* ------------------------------------------------------------------ */
/* Interrupt types — used by ke.h KINTERRUPT struct                    */
/* ------------------------------------------------------------------ */
#ifndef _KINTERRUPT_MODE_DEFINED
#define _KINTERRUPT_MODE_DEFINED
typedef enum _KINTERRUPT_MODE {
    LevelSensitive,
    Latched
} KINTERRUPT_MODE;
#endif

#ifndef _KINTERRUPT_ROUTINE_DEFINED
#define _KINTERRUPT_ROUTINE_DEFINED
typedef BOOLEAN (NTAPI *PKINTERRUPT_ROUTINE)(
    IN struct _KINTERRUPT *Interrupt,
    IN PVOID ServiceContext
);
/* Forward-declare KINTERRUPT for the function pointer above */
struct _KINTERRUPT;
typedef struct _KINTERRUPT *PKINTERRUPT;
#endif

/* ------------------------------------------------------------------ */
/* PACCESS_TOKEN — used by ntosdef.h SECURITY_CLIENT_CONTEXT           */
/* ------------------------------------------------------------------ */
#ifndef _PACCESS_TOKEN_DEFINED
#define _PACCESS_TOKEN_DEFINED
typedef PVOID PACCESS_TOKEN;
typedef PVOID PACCESS_TOKEN_ATTRIBUTES;
#endif

/* ------------------------------------------------------------------ */
/* Video mode attribute flags — used by video drivers                   */
/* These come from the SDK ntddvdeo.h but our interception header      */
/* blocks it, so we define them here.                                  */
/* ------------------------------------------------------------------ */
#ifndef VIDEO_MODE_COLOR
#define VIDEO_MODE_COLOR            0x0001
#define VIDEO_MODE_GRAPHICS         0x0002
#define VIDEO_MODE_PALETTE_DRIVEN   0x0004
#define VIDEO_MODE_MANAGED_PALETTE  0x0008
#endif

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
/* Only active when NOT compiling NT kernel source (which provides its */
/* own declarations with __declspec(dllimport) linkage).               */
/* ------------------------------------------------------------------ */
#ifndef _NT_KERNEL_BUILD

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
#ifndef _KEINITIALIZEDPC_DEFINED
#define _KEINITIALIZEDPC_DEFINED
static inline void KeInitializeDpc(PKDPC Dpc, PVOID DeferredRoutine, PVOID DeferredContext) {
    Dpc->DeferredRoutine = DeferredRoutine;
    Dpc->DeferredContext = DeferredContext;
}
#endif

/* Timer operations */
#ifndef _KEINITIALIZETIMER_DEFINED
#define _KEINITIALIZETIMER_DEFINED
static inline void KeInitializeTimer(PKTIMER Timer) {
    Timer->Header.Type = 8; /* TimerNotificationObject */
    Timer->Header.SignalState = 0;
    InitializeListHead(&Timer->Header.WaitListHead);
    Timer->Dpc = NULL;
}
#endif

/* Pool allocation stubs — stdlib.h is included by ntdef.h */
#ifndef ExAllocatePool
static inline PVOID ExAllocatePool(POOL_TYPE PoolType, SIZE_T NumberOfBytes) {
    (void)PoolType;
    return (PVOID)malloc((size_t)NumberOfBytes);
}

static inline PVOID ExAllocatePoolWithTag(POOL_TYPE PoolType, SIZE_T NumberOfBytes, ULONG Tag) {
    (void)PoolType;
    (void)Tag;
    return (PVOID)malloc((size_t)NumberOfBytes);
}

static inline void ExFreePool(PVOID P) {
    free(P);
}

static inline void ExFreePoolWithTag(PVOID P, ULONG Tag) {
    (void)Tag;
    free(P);
}
#endif /* !ExAllocatePool */

/* IRQL management */
static inline KIRQL KeGetCurrentIrql(void) { return PASSIVE_LEVEL; }
static inline KIRQL KeRaiseIrqlToDpcLevel(void) { return PASSIVE_LEVEL; }
static inline void KeLowerIrql(KIRQL NewIrql) { (void)NewIrql; }

#endif /* _NT_KERNEL_BUILD */

/* Debug print */
#include <stdio.h>
#define DbgPrint(...) fprintf(stderr, __VA_ARGS__)
#define KdPrint(x)    DbgPrint x

#ifdef __cplusplus
}
#endif

#endif /* _NT_H_COMPAT_ */
