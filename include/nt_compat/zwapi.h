/*
 * zwapi.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 67 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_ZWAPI_H_H_
#define _NT_COMPAT_ZWAPI_H_H_

#include "ntdef.h"
#include "nt.h"
#ifdef __cplusplus
extern "C" {
#endif


/* Zw* routines are kernel-mode entry points for Nt* system services.
 * They skip access checks since the caller is already in kernel mode. */

#ifndef _ZWAPI_DEFINED
#define _ZWAPI_DEFINED

/* APC routine callback for async I/O */
#ifndef _IO_APC_ROUTINE_DEFINED
#define _IO_APC_ROUTINE_DEFINED
typedef VOID (NTAPI *PIO_APC_ROUTINE)(IN PVOID ApcContext, IN PIO_STATUS_BLOCK IoStatusBlock, IN ULONG Reserved);
#endif

NTSTATUS NTAPI ZwClose(IN HANDLE Handle);
NTSTATUS NTAPI ZwCreateFile(OUT PHANDLE FileHandle, IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes, OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PLARGE_INTEGER AllocationSize OPTIONAL, IN ULONG FileAttributes,
    IN ULONG ShareAccess, IN ULONG CreateDisposition, IN ULONG CreateOptions,
    IN PVOID EaBuffer OPTIONAL, IN ULONG EaLength);
NTSTATUS NTAPI ZwOpenFile(OUT PHANDLE FileHandle, IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes, OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG ShareAccess, IN ULONG OpenOptions);
NTSTATUS NTAPI ZwReadFile(IN HANDLE FileHandle, IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL, IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock, OUT PVOID Buffer, IN ULONG Length,
    IN PLARGE_INTEGER ByteOffset OPTIONAL, IN PULONG Key OPTIONAL);
NTSTATUS NTAPI ZwWriteFile(IN HANDLE FileHandle, IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL, IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock, IN PVOID Buffer, IN ULONG Length,
    IN PLARGE_INTEGER ByteOffset OPTIONAL, IN PULONG Key OPTIONAL);
NTSTATUS NTAPI ZwQueryInformationFile(IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock, OUT PVOID FileInformation,
    IN ULONG Length, IN ULONG FileInformationClass);
NTSTATUS NTAPI ZwSetInformationFile(IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock, IN PVOID FileInformation,
    IN ULONG Length, IN ULONG FileInformationClass);
NTSTATUS NTAPI ZwQueryVolumeInformationFile(IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock, OUT PVOID FsInformation,
    IN ULONG Length, IN ULONG FsInformationClass);
NTSTATUS NTAPI ZwDeviceIoControlFile(IN HANDLE FileHandle, IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL, IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock, IN ULONG IoControlCode,
    IN PVOID InputBuffer OPTIONAL, IN ULONG InputBufferLength,
    OUT PVOID OutputBuffer OPTIONAL, IN ULONG OutputBufferLength);
NTSTATUS NTAPI ZwFsControlFile(IN HANDLE FileHandle, IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL, IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock, IN ULONG FsControlCode,
    IN PVOID InputBuffer OPTIONAL, IN ULONG InputBufferLength,
    OUT PVOID OutputBuffer OPTIONAL, IN ULONG OutputBufferLength);
NTSTATUS NTAPI ZwFlushBuffersFile(IN HANDLE FileHandle, OUT PIO_STATUS_BLOCK IoStatusBlock);
NTSTATUS NTAPI ZwQueryDirectoryFile(IN HANDLE FileHandle, IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL, IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock, OUT PVOID FileInformation,
    IN ULONG Length, IN ULONG FileInformationClass, IN BOOLEAN ReturnSingleEntry,
    IN PUNICODE_STRING FileName OPTIONAL, IN BOOLEAN RestartScan);

/* Registry */
NTSTATUS NTAPI ZwOpenKey(OUT PHANDLE KeyHandle, IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes);
NTSTATUS NTAPI ZwCreateKey(OUT PHANDLE KeyHandle, IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes, IN ULONG TitleIndex,
    IN PUNICODE_STRING Class OPTIONAL, IN ULONG CreateOptions, OUT PULONG Disposition OPTIONAL);
NTSTATUS NTAPI ZwQueryValueKey(IN HANDLE KeyHandle, IN PUNICODE_STRING ValueName,
    IN ULONG KeyValueInformationClass, OUT PVOID KeyValueInformation,
    IN ULONG Length, OUT PULONG ResultLength);
NTSTATUS NTAPI ZwSetValueKey(IN HANDLE KeyHandle, IN PUNICODE_STRING ValueName,
    IN ULONG TitleIndex, IN ULONG Type, IN PVOID Data, IN ULONG DataSize);
NTSTATUS NTAPI ZwDeleteKey(IN HANDLE KeyHandle);
NTSTATUS NTAPI ZwEnumerateKey(IN HANDLE KeyHandle, IN ULONG Index,
    IN ULONG KeyInformationClass, OUT PVOID KeyInformation,
    IN ULONG Length, OUT PULONG ResultLength);
NTSTATUS NTAPI ZwEnumerateValueKey(IN HANDLE KeyHandle, IN ULONG Index,
    IN ULONG KeyValueInformationClass, OUT PVOID KeyValueInformation,
    IN ULONG Length, OUT PULONG ResultLength);

/* Process/Thread */
NTSTATUS NTAPI ZwCreateProcess(OUT PHANDLE ProcessHandle, IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL, IN HANDLE ParentProcess,
    IN BOOLEAN InheritObjectTable, IN HANDLE SectionHandle OPTIONAL,
    IN HANDLE DebugPort OPTIONAL, IN HANDLE ExceptionPort OPTIONAL);
NTSTATUS NTAPI ZwOpenProcess(OUT PHANDLE ProcessHandle, IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes, IN PCLIENT_ID ClientId);
NTSTATUS NTAPI ZwTerminateProcess(IN HANDLE ProcessHandle OPTIONAL, IN NTSTATUS ExitStatus);
NTSTATUS NTAPI ZwQueryInformationProcess(IN HANDLE ProcessHandle,
    IN ULONG ProcessInformationClass, OUT PVOID ProcessInformation,
    IN ULONG ProcessInformationLength, OUT PULONG ReturnLength OPTIONAL);

/* Memory */
NTSTATUS NTAPI ZwAllocateVirtualMemory(IN HANDLE ProcessHandle, IN OUT PVOID *BaseAddress,
    IN ULONG_PTR ZeroBits, IN OUT PSIZE_T RegionSize, IN ULONG AllocationType, IN ULONG Protect);
NTSTATUS NTAPI ZwFreeVirtualMemory(IN HANDLE ProcessHandle, IN OUT PVOID *BaseAddress,
    IN OUT PSIZE_T RegionSize, IN ULONG FreeType);

/* Section */
NTSTATUS NTAPI ZwCreateSection(OUT PHANDLE SectionHandle, IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL, IN PLARGE_INTEGER MaximumSize OPTIONAL,
    IN ULONG SectionPageProtection, IN ULONG AllocationAttributes, IN HANDLE FileHandle OPTIONAL);
NTSTATUS NTAPI ZwMapViewOfSection(IN HANDLE SectionHandle, IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress, IN ULONG_PTR ZeroBits, IN SIZE_T CommitSize,
    IN OUT PLARGE_INTEGER SectionOffset OPTIONAL, IN OUT PSIZE_T ViewSize,
    IN ULONG InheritDisposition, IN ULONG AllocationType, IN ULONG Win32Protect);
NTSTATUS NTAPI ZwUnmapViewOfSection(IN HANDLE ProcessHandle, IN PVOID BaseAddress);

/* Events/Synchronization */
NTSTATUS NTAPI ZwCreateEvent(OUT PHANDLE EventHandle, IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL, IN ULONG EventType,
    IN BOOLEAN InitialState);
NTSTATUS NTAPI ZwSetEvent(IN HANDLE EventHandle, OUT PLONG PreviousState OPTIONAL);
NTSTATUS NTAPI ZwWaitForSingleObject(IN HANDLE Handle, IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL);
NTSTATUS NTAPI ZwWaitForMultipleObjects(IN ULONG Count, IN HANDLE Handles[],
    IN ULONG WaitType, IN BOOLEAN Alertable, IN PLARGE_INTEGER Timeout OPTIONAL);

/* Misc */
NTSTATUS NTAPI ZwMakeTemporaryObject(IN HANDLE Handle);
NTSTATUS NTAPI ZwDuplicateObject(IN HANDLE SourceProcessHandle, IN HANDLE SourceHandle,
    IN HANDLE TargetProcessHandle OPTIONAL, OUT PHANDLE TargetHandle OPTIONAL,
    IN ACCESS_MASK DesiredAccess, IN ULONG HandleAttributes, IN ULONG Options);
NTSTATUS NTAPI ZwLoadDriver(IN PUNICODE_STRING DriverServiceName);
NTSTATUS NTAPI ZwUnloadDriver(IN PUNICODE_STRING DriverServiceName);

#endif /* _ZWAPI_DEFINED */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_ZWAPI_H_H_ */
