/*++ BUILD Version: 0001
 * ntsrv.h — NT Server (SRV/SMB) Compatibility Shim
 *
 * Types for the LAN Manager server (srv.sys) driver.
 *--*/
#ifndef _NTSRV_H_
#define _NTSRV_H_

#include "ntddk.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── Server block types ── */
typedef enum _BLOCK_TYPE {
    BlockTypeGarbage = 0,
    BlockTypeBuffer,
    BlockTypeConnection,
    BlockTypeEndpoint,
    BlockTypeLfcb,
    BlockTypeMfcb,
    BlockTypeRfcb,
    BlockTypeSearch,
    BlockTypeSearchCore,
    BlockTypeSession,
    BlockTypeShare,
    BlockTypeTransaction,
    BlockTypeTreeConnect,
    BlockTypeWaitForOplockBreak,
    BlockTypeCommDevice,
    BlockTypeWorkContext,
    BlockTypeMaximum
} BLOCK_TYPE;

/* ── Block header ── */
typedef struct _BLOCK_HEADER {
    USHORT      Type;
    USHORT      State;
    LONG        ReferenceCount;
} BLOCK_HEADER, *PBLOCK_HEADER;

/* ── Server statistics ── */
typedef struct _SRV_STATISTICS {
    LARGE_INTEGER TotalBytesReceived;
    LARGE_INTEGER TotalBytesSent;
    ULONG         SessionsTimedOut;
    ULONG         SessionsErroredOut;
    ULONG         SessionsLoggedOff;
    ULONG         SessionsForcedLogOff;
    ULONG         LogonErrors;
    ULONG         AccessPermissionErrors;
    ULONG         GrantedAccessErrors;
    ULONG         SystemErrors;
    ULONG         BlockingSmbsRejected;
    ULONG         WorkItemShortages;
} SRV_STATISTICS, *PSRV_STATISTICS;

/* ── SMB header ── */
typedef struct _SMB_HEADER {
    UCHAR       Protocol[4];
    UCHAR       Command;
    NTSTATUS    Status;
    UCHAR       Flags;
    USHORT      Flags2;
    USHORT      PidHigh;
    UCHAR       SecuritySignature[8];
    USHORT      Reserved;
    USHORT      Tid;
    USHORT      Pid;
    USHORT      Uid;
    USHORT      Mid;
} SMB_HEADER, *PSMB_HEADER;

/* ── Common SRV macros ── */
#define SMB_COM_NEGOTIATE           0x72
#define SMB_COM_SESSION_SETUP_ANDX  0x73
#define SMB_COM_TREE_CONNECT_ANDX   0x75
#define SMB_COM_TRANSACTION         0x25
#define SMB_COM_TRANSACTION2        0x32
#define SMB_COM_READ_ANDX           0x2E
#define SMB_COM_WRITE_ANDX          0x2F
#define SMB_COM_CLOSE               0x04

#ifdef __cplusplus
}
#endif

#endif /* _NTSRV_H_ */
