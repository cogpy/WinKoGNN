/*++ BUILD Version: 0001
 * ntifs.h — NT Installable File System Kit Compatibility Shim
 *
 * Provides file system driver types: FCB, CCB, VCB, file objects,
 * cache manager, and Fsrtl stubs.
 *--*/
#ifndef _NTIFS_H_
#define _NTIFS_H_

#include "ntddk.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── File system control flags ── */
#define SL_CASE_SENSITIVE           0x0080
#define SL_OPEN_PAGING_FILE         0x0002
#define SL_FORCE_ACCESS_CHECK       0x0001

/* ── File information classes ── */
typedef enum _FILE_INFORMATION_CLASS {
    FileDirectoryInformation = 1,
    FileFullDirectoryInformation,
    FileBothDirectoryInformation,
    FileBasicInformation,
    FileStandardInformation,
    FileInternalInformation,
    FileEaInformation,
    FileAccessInformation,
    FileNameInformation,
    FileRenameInformation,
    FileLinkInformation,
    FileNamesInformation,
    FileDispositionInformation,
    FilePositionInformation,
    FileFullEaInformation,
    FileModeInformation,
    FileAlignmentInformation,
    FileAllInformation,
    FileAllocationInformation,
    FileEndOfFileInformation,
    FileAlternateNameInformation,
    FileStreamInformation,
    FilePipeInformation,
    FilePipeLocalInformation,
    FilePipeRemoteInformation,
    FileMailslotQueryInformation,
    FileMailslotSetInformation,
    FileCompressionInformation,
    FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

/* ── FS information classes ── */
typedef enum _FS_INFORMATION_CLASS {
    FileFsVolumeInformation = 1,
    FileFsLabelInformation,
    FileFsSizeInformation,
    FileFsDeviceInformation,
    FileFsAttributeInformation,
    FileFsControlInformation,
    FileFsFullSizeInformation,
    FileFsObjectIdInformation,
    FileFsMaximumInformation
} FS_INFORMATION_CLASS, *PFS_INFORMATION_CLASS;

/* ── File Object ── */
typedef struct _FILE_OBJECT {
    SHORT               Type;
    SHORT               Size;
    PDEVICE_OBJECT      DeviceObject;
    PVOID               FsContext;
    PVOID               FsContext2;
    PVOID               SectionObjectPointer;
    PVOID               PrivateCacheMap;
    NTSTATUS            FinalStatus;
    PVOID               RelatedFileObject;
    BOOLEAN             LockOperation;
    BOOLEAN             DeletePending;
    BOOLEAN             ReadAccess;
    BOOLEAN             WriteAccess;
    BOOLEAN             DeleteAccess;
    BOOLEAN             SharedRead;
    BOOLEAN             SharedWrite;
    BOOLEAN             SharedDelete;
    ULONG               Flags;
    UNICODE_STRING      FileName;
    LARGE_INTEGER       CurrentByteOffset;
} FILE_OBJECT, *PFILE_OBJECT;

/* ── Section Object Pointers ── */
typedef struct _SECTION_OBJECT_POINTERS {
    PVOID DataSectionObject;
    PVOID SharedCacheMap;
    PVOID ImageSectionObject;
} SECTION_OBJECT_POINTERS, *PSECTION_OBJECT_POINTERS;

/* ── ERESOURCE (executive resource lock) ── */
#ifndef _ERESOURCE_DEFINED
#define _ERESOURCE_DEFINED
typedef struct _ERESOURCE {
    LIST_ENTRY          SystemResourcesList;
    PVOID               OwnerTable;
    SHORT               ActiveCount;
    USHORT              Flag;
    PVOID               SharedWaiters;
    PVOID               ExclusiveWaiters;
    PVOID               OwnerThreads[2];
    ULONG               ContentionCount;
    ULONG               NumberOfSharedWaiters;
    ULONG               NumberOfExclusiveWaiters;
    PVOID               Address;
    KSPIN_LOCK          SpinLock;
} ERESOURCE, *PERESOURCE;
#endif /* _ERESOURCE_DEFINED */

/* ── FSRTL stubs ── */
#define FsRtlEnterFileSystem()    ((void)0)
#define FsRtlExitFileSystem()     ((void)0)

static inline BOOLEAN FsRtlDoesNameContainWildCards(PUNICODE_STRING Name) {
    (void)Name; return FALSE;
}

static inline BOOLEAN FsRtlIsNameInExpression(
    PUNICODE_STRING Expression, PUNICODE_STRING Name,
    BOOLEAN IgnoreCase, PWCHAR UpcaseTable) {
    (void)Expression; (void)Name; (void)IgnoreCase; (void)UpcaseTable;
    return FALSE;
}

/* ── Cache manager stubs ── */
static inline BOOLEAN CcInitializeCacheMap(
    PFILE_OBJECT FileObject, PVOID FileSizes,
    BOOLEAN PinAccess, PVOID Callbacks, PVOID LazyWriteContext) {
    (void)FileObject; (void)FileSizes; (void)PinAccess;
    (void)Callbacks; (void)LazyWriteContext;
    return TRUE;
}

static inline BOOLEAN CcUninitializeCacheMap(
    PFILE_OBJECT FileObject, PVOID TruncateSize, PVOID UninitializeCompleteEvent) {
    (void)FileObject; (void)TruncateSize; (void)UninitializeCompleteEvent;
    return TRUE;
}

/* ── ERESOURCE stubs ── */
static inline NTSTATUS ExInitializeResourceLite(PERESOURCE Resource) {
    (void)Resource; return STATUS_SUCCESS;
}

static inline NTSTATUS ExDeleteResourceLite(PERESOURCE Resource) {
    (void)Resource; return STATUS_SUCCESS;
}

static inline BOOLEAN ExAcquireResourceExclusiveLite(PERESOURCE Resource, BOOLEAN Wait) {
    (void)Resource; (void)Wait; return TRUE;
}

static inline BOOLEAN ExAcquireResourceSharedLite(PERESOURCE Resource, BOOLEAN Wait) {
    (void)Resource; (void)Wait; return TRUE;
}

static inline void ExReleaseResourceLite(PERESOURCE Resource) {
    (void)Resource;
}

/* ── Notify change directory ── */
static inline void FsRtlNotifyFullChangeDirectory(
    PVOID NotifySync, PLIST_ENTRY NotifyList, PVOID FsContext,
    PVOID FullDirectoryName, BOOLEAN WatchTree, BOOLEAN IgnoreBuffer,
    ULONG CompletionFilter, PIRP Irp, PVOID TraverseCallback,
    PVOID SubjectContext) {
    (void)NotifySync; (void)NotifyList; (void)FsContext;
    (void)FullDirectoryName; (void)WatchTree; (void)IgnoreBuffer;
    (void)CompletionFilter; (void)Irp; (void)TraverseCallback;
    (void)SubjectContext;
}

#ifdef __cplusplus
}
#endif

#endif /* _NTIFS_H_ */
