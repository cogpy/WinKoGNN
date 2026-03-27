/*
 * nturtl.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 152 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTURTL_H_H_
#define _NT_COMPAT_NTURTL_H_H_

#include "ntdef.h"
#include "ntrtl.h"
#ifdef __cplusplus
extern "C" {
#endif


/* NT User-mode Runtime Library — supplements ntrtl.h for user-mode components */

#ifndef _NTURTL_H_COMPAT
#define _NTURTL_H_COMPAT

/* Process parameters */
#ifndef _RTL_USER_PROCESS_PARAMETERS_DEFINED
#define _RTL_USER_PROCESS_PARAMETERS_DEFINED
typedef struct _RTL_USER_PROCESS_PARAMETERS {
    ULONG MaximumLength;
    ULONG Length;
    ULONG Flags;
    ULONG DebugFlags;
    HANDLE ConsoleHandle;
    ULONG ConsoleFlags;
    HANDLE StandardInput;
    HANDLE StandardOutput;
    HANDLE StandardError;
    UNICODE_STRING CurrentDirectory_DosPath;
    HANDLE CurrentDirectory_Handle;
    UNICODE_STRING DllPath;
    UNICODE_STRING ImagePathName;
    UNICODE_STRING CommandLine;
    PVOID Environment;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;
#endif

/* PEB — Process Environment Block */
#ifndef _PEB_DEFINED
#define _PEB_DEFINED
typedef struct _PEB {
    BOOLEAN InheritedAddressSpace;
    BOOLEAN ReadImageFileExecOptions;
    BOOLEAN BeingDebugged;
    BOOLEAN SpareBool;
    HANDLE Mutant;
    PVOID ImageBaseAddress;
    PVOID Ldr;
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
    PVOID SubSystemData;
    PVOID ProcessHeap;
} PEB, *PPEB;
#endif

/* TEB — Thread Environment Block */
#ifndef _TEB_DEFINED
#define _TEB_DEFINED
typedef struct _TEB {
    NT_TIB NtTib;
    PVOID EnvironmentPointer;
    CLIENT_ID ClientId;
    PVOID ActiveRpcHandle;
    PVOID ThreadLocalStoragePointer;
    PPEB ProcessEnvironmentBlock;
} TEB, *PTEB;
#endif

/* Heap */
PVOID NTAPI RtlCreateHeap(IN ULONG Flags, IN PVOID HeapBase OPTIONAL,
    IN SIZE_T ReserveSize, IN SIZE_T CommitSize, IN PVOID Lock OPTIONAL, IN PVOID Parameters OPTIONAL);
PVOID NTAPI RtlDestroyHeap(IN PVOID HeapHandle);
PVOID NTAPI RtlAllocateHeap(IN PVOID HeapHandle, IN ULONG Flags, IN SIZE_T Size);
BOOLEAN NTAPI RtlFreeHeap(IN PVOID HeapHandle, IN ULONG Flags, IN PVOID BaseAddress);

/* Process creation */
NTSTATUS NTAPI RtlCreateProcessParameters(
    OUT PRTL_USER_PROCESS_PARAMETERS *ProcessParameters,
    IN PUNICODE_STRING ImagePathName, IN PUNICODE_STRING DllPath OPTIONAL,
    IN PUNICODE_STRING CurrentDirectory OPTIONAL, IN PUNICODE_STRING CommandLine OPTIONAL,
    IN PVOID Environment OPTIONAL, IN PUNICODE_STRING WindowTitle OPTIONAL,
    IN PUNICODE_STRING DesktopInfo OPTIONAL, IN PUNICODE_STRING ShellInfo OPTIONAL,
    IN PUNICODE_STRING RuntimeData OPTIONAL);
NTSTATUS NTAPI RtlDestroyProcessParameters(IN PRTL_USER_PROCESS_PARAMETERS ProcessParameters);

/* Critical sections */
#ifndef _RTL_CRITICAL_SECTION_DEFINED
#define _RTL_CRITICAL_SECTION_DEFINED
typedef struct _RTL_CRITICAL_SECTION_DEBUG {
    USHORT Type;
    USHORT CreatorBackTraceIndex;
    struct _RTL_CRITICAL_SECTION *CriticalSection;
    LIST_ENTRY ProcessLocksList;
    ULONG EntryCount;
    ULONG ContentionCount;
    ULONG Spare[2];
} RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG;

typedef struct _RTL_CRITICAL_SECTION {
    PRTL_CRITICAL_SECTION_DEBUG DebugInfo;
    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;
#endif

NTSTATUS NTAPI RtlInitializeCriticalSection(IN PRTL_CRITICAL_SECTION CriticalSection);
NTSTATUS NTAPI RtlEnterCriticalSection(IN PRTL_CRITICAL_SECTION CriticalSection);
NTSTATUS NTAPI RtlLeaveCriticalSection(IN PRTL_CRITICAL_SECTION CriticalSection);
NTSTATUS NTAPI RtlDeleteCriticalSection(IN PRTL_CRITICAL_SECTION CriticalSection);

#endif /* _NTURTL_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTURTL_H_H_ */
