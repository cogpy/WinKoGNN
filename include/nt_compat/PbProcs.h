/*++ BUILD Version: 0001
 * PbProcs.h — Pinball (HPFS/NTFS) Procedures Compatibility Shim
 *
 * "Pinball" was the codename for the HPFS file system in NT.
 *--*/
#ifndef _PBPROCS_H_
#define _PBPROCS_H_

#include "ntifs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── Pinball node type codes ── */
#define PB_NTC_VCB                  0x0501
#define PB_NTC_FCB                  0x0502
#define PB_NTC_CCB                  0x0503
#define PB_NTC_ROOT_DCB             0x0504
#define PB_NTC_DCB                  0x0505
#define PB_NTC_IRP_CONTEXT          0x0506

/* ── Volume Control Block ── */
typedef struct _PB_VCB {
    USHORT          NodeTypeCode;
    USHORT          NodeByteSize;
    PVOID           Vpb;
    PDEVICE_OBJECT  TargetDeviceObject;
    LIST_ENTRY      VcbLinks;
    ERESOURCE       Resource;
    ULONG           VcbState;
    ULONG           VcbCondition;
} PB_VCB, *PPB_VCB;

/* ── File Control Block ── */
typedef struct _PB_FCB {
    USHORT          NodeTypeCode;
    USHORT          NodeByteSize;
    LIST_ENTRY      ParentDcbLinks;
    PVOID           ParentDcb;
    ERESOURCE       Resource;
    ERESOURCE       PagingIoResource;
    ULONG           FcbState;
    UNICODE_STRING  FullFileName;
    UNICODE_STRING  FinalName;
    LARGE_INTEGER   CreationTime;
    LARGE_INTEGER   LastAccessTime;
    LARGE_INTEGER   LastWriteTime;
} PB_FCB, *PPB_FCB;

/* ── Context Control Block ── */
typedef struct _PB_CCB {
    USHORT          NodeTypeCode;
    USHORT          NodeByteSize;
    ULONG           Flags;
    UNICODE_STRING  QueryTemplate;
} PB_CCB, *PPB_CCB;

#ifdef __cplusplus
}
#endif

#endif /* _PBPROCS_H_ */
