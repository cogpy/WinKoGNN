/*++
 * nt_stubs.c — NT System Call Stub Implementations
 *
 * Provides no-op / minimal implementations of NT system calls
 * declared in nt.h for cross-platform compilation.
 *--*/

#include "nt.h"
#include "ntrtl.h"
#include "ntstatus.h"

/* ------------------------------------------------------------------ */
/* Process management stubs                                            */
/* ------------------------------------------------------------------ */

NTSTATUS NTAPI NtCreateProcess(
    OUT PHANDLE ProcessHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN HANDLE ParentProcess,
    IN BOOLEAN InheritObjectTable,
    IN HANDLE SectionHandle,
    IN HANDLE DebugPort,
    IN HANDLE ExceptionPort)
{
    (void)DesiredAccess; (void)ObjectAttributes; (void)ParentProcess;
    (void)InheritObjectTable; (void)SectionHandle; (void)DebugPort;
    (void)ExceptionPort;
    static ULONG_PTR fake_handle = 0x1000;
    *ProcessHandle = (HANDLE)(fake_handle++);
    return STATUS_SUCCESS;
}

NTSTATUS NTAPI NtTerminateProcess(
    IN HANDLE ProcessHandle,
    IN NTSTATUS ExitStatus)
{
    (void)ProcessHandle; (void)ExitStatus;
    return STATUS_SUCCESS;
}

/* ------------------------------------------------------------------ */
/* Thread management stubs                                             */
/* ------------------------------------------------------------------ */

NTSTATUS NTAPI NtCreateThread(
    OUT PHANDLE ThreadHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN HANDLE ProcessHandle,
    OUT PCLIENT_ID ClientId,
    IN PVOID ThreadContext,
    IN PVOID InitialTeb,
    IN BOOLEAN CreateSuspended)
{
    (void)DesiredAccess; (void)ObjectAttributes; (void)ProcessHandle;
    (void)ThreadContext; (void)InitialTeb; (void)CreateSuspended;
    static ULONG_PTR fake_thread = 0x2000;
    *ThreadHandle = (HANDLE)(fake_thread);
    if (ClientId) {
        ClientId->UniqueProcess = (HANDLE)1;
        ClientId->UniqueThread = (HANDLE)(fake_thread);
    }
    fake_thread++;
    return STATUS_SUCCESS;
}

/* ------------------------------------------------------------------ */
/* Memory management stubs                                             */
/* ------------------------------------------------------------------ */

NTSTATUS NTAPI NtAllocateVirtualMemory(
    IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN ULONG_PTR ZeroBits,
    IN OUT PSIZE_T RegionSize,
    IN ULONG AllocationType,
    IN ULONG Protect)
{
    (void)ProcessHandle; (void)ZeroBits; (void)AllocationType; (void)Protect;
    *BaseAddress = malloc(*RegionSize);
    if (!*BaseAddress) return STATUS_NO_MEMORY;
    memset(*BaseAddress, 0, *RegionSize);
    return STATUS_SUCCESS;
}

NTSTATUS NTAPI NtFreeVirtualMemory(
    IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN OUT PSIZE_T RegionSize,
    IN ULONG FreeType)
{
    (void)ProcessHandle; (void)RegionSize; (void)FreeType;
    if (*BaseAddress) {
        free(*BaseAddress);
        *BaseAddress = NULL;
    }
    return STATUS_SUCCESS;
}

/* ------------------------------------------------------------------ */
/* Object management stubs                                             */
/* ------------------------------------------------------------------ */

NTSTATUS NTAPI NtClose(IN HANDLE Handle)
{
    (void)Handle;
    return STATUS_SUCCESS;
}

NTSTATUS NTAPI NtWaitForSingleObject(
    IN HANDLE Handle,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout)
{
    (void)Handle; (void)Alertable; (void)Timeout;
    return STATUS_SUCCESS;
}

/* ------------------------------------------------------------------ */
/* Event management stubs                                              */
/* ------------------------------------------------------------------ */

NTSTATUS NTAPI NtCreateEvent(
    OUT PHANDLE EventHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN ULONG EventType,
    IN BOOLEAN InitialState)
{
    (void)DesiredAccess; (void)ObjectAttributes; (void)EventType; (void)InitialState;
    static ULONG_PTR fake_event = 0x3000;
    *EventHandle = (HANDLE)(fake_event++);
    return STATUS_SUCCESS;
}

NTSTATUS NTAPI NtSetEvent(
    IN HANDLE EventHandle,
    OUT PLONG PreviousState)
{
    (void)EventHandle;
    if (PreviousState) *PreviousState = 0;
    return STATUS_SUCCESS;
}

/* ------------------------------------------------------------------ */
/* Section management stubs                                            */
/* ------------------------------------------------------------------ */

NTSTATUS NTAPI NtCreateSection(
    OUT PHANDLE SectionHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN PLARGE_INTEGER MaximumSize,
    IN ULONG SectionPageProtection,
    IN ULONG AllocationAttributes,
    IN HANDLE FileHandle)
{
    (void)DesiredAccess; (void)ObjectAttributes; (void)MaximumSize;
    (void)SectionPageProtection; (void)AllocationAttributes; (void)FileHandle;
    static ULONG_PTR fake_section = 0x4000;
    *SectionHandle = (HANDLE)(fake_section++);
    return STATUS_SUCCESS;
}

/* ------------------------------------------------------------------ */
/* LPC stubs                                                           */
/* ------------------------------------------------------------------ */

NTSTATUS NTAPI NtCreatePort(
    OUT PHANDLE PortHandle,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN ULONG MaxConnectionInfoLength,
    IN ULONG MaxMessageLength,
    IN ULONG MaxPoolUsage)
{
    (void)ObjectAttributes; (void)MaxConnectionInfoLength;
    (void)MaxMessageLength; (void)MaxPoolUsage;
    static ULONG_PTR fake_port = 0x5000;
    *PortHandle = (HANDLE)(fake_port++);
    return STATUS_SUCCESS;
}
