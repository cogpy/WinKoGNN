/*++ BUILD Version: 0001
 * dfsprocs.h — Distributed File System Procedures Compatibility Shim
 *--*/
#ifndef _DFSPROCS_H_
#define _DFSPROCS_H_

#include "ntifs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── DFS types ── */
typedef struct _DFS_VCB {
    USHORT          NodeTypeCode;
    USHORT          NodeByteSize;
    PVOID           VPB;
    PDEVICE_OBJECT  TargetDeviceObject;
    LIST_ENTRY      VcbLinks;
} DFS_VCB, *PDFS_VCB;

typedef struct _DFS_FCB {
    USHORT          NodeTypeCode;
    USHORT          NodeByteSize;
    UNICODE_STRING  FullFileName;
    UNICODE_STRING  AlternateFileName;
} DFS_FCB, *PDFS_FCB;

/* ── DFS node type codes ── */
#define DSFS_NTC_VCB                0x0901
#define DSFS_NTC_FCB                0x0902
#define DSFS_NTC_DATA_HEADER        0x0903

/* ── DFS function stubs ── */
static inline NTSTATUS DfsVolumePassThrough(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    (void)DeviceObject; (void)Irp;
    return STATUS_SUCCESS;
}

static inline NTSTATUS DfsFsdCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    (void)DeviceObject; (void)Irp;
    return STATUS_SUCCESS;
}

static inline NTSTATUS DfsFsdCleanup(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    (void)DeviceObject; (void)Irp;
    return STATUS_SUCCESS;
}

static inline NTSTATUS DfsFsdClose(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    (void)DeviceObject; (void)Irp;
    return STATUS_SUCCESS;
}

#ifdef __cplusplus
}
#endif

#endif /* _DFSPROCS_H_ */
