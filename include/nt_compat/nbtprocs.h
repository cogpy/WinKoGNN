/*++ BUILD Version: 0001
 * nbtprocs.h — NetBIOS over TCP/IP Procedures Compatibility Shim
 *--*/
#ifndef _NBTPROCS_H_
#define _NBTPROCS_H_

#include "oscfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── NetBT state machine states ── */
typedef enum _NBT_STATE {
    NBT_IDLE = 0,
    NBT_ASSOCIATED,
    NBT_CONNECTING,
    NBT_SESSION_INBOUND,
    NBT_SESSION_OUTBOUND,
    NBT_SESSION_UP,
    NBT_DISCONNECTING,
    NBT_DISCONNECTED
} NBT_STATE;

/* ── Name types ── */
#define NBT_UNIQUE      0x0000
#define NBT_GROUP       0x0001
#define NBT_NAME_SIZE   16

/* ── Connection element ── */
typedef struct _tCONNECTELE {
    LIST_ENTRY      Linkage;
    ULONG           Verify;
    NBT_STATE       state;
    ULONG           RefCount;
    PVOID           pLowerConnId;
    PVOID           pClientEle;
    PVOID           pDeviceContext;
} tCONNECTELE, *ptCONNECTELE;

/* ── Address element ── */
typedef struct _tADDRESSELE {
    LIST_ENTRY      Linkage;
    ULONG           Verify;
    ULONG           RefCount;
    LIST_ENTRY      ClientHead;
    PVOID           pNameAddr;
    PVOID           pDeviceContext;
    PVOID           SecurityDescriptor;
} tADDRESSELE, *ptADDRESSELE;

/* ── Device context ── */
typedef struct _tDEVICECONTEXT {
    LIST_ENTRY      Linkage;
    ULONG           Verify;
    DEVICE_OBJECT   DeviceObject;
    UNICODE_STRING  ExportName;
    UNICODE_STRING  BindName;
    ULONG           IpAddress;
    ULONG           SubnetMask;
    ULONG           BroadcastAddress;
    ULONG           lNameServerAddress;
    ULONG           lBackupServer;
} tDEVICECONTEXT, *ptDEVICECONTEXT;

/* ── Function stubs ── */
static inline NTSTATUS NbtOpenConnection(PVOID pDeviceContext, PVOID pIrp, PVOID pHandle) {
    (void)pDeviceContext; (void)pIrp; (void)pHandle; return STATUS_SUCCESS;
}

static inline NTSTATUS NbtCloseConnection(PVOID pConnEle, PVOID pIrp) {
    (void)pConnEle; (void)pIrp; return STATUS_SUCCESS;
}

static inline NTSTATUS NbtAssociateAddress(PVOID pConnEle, PVOID pAddrEle) {
    (void)pConnEle; (void)pAddrEle; return STATUS_SUCCESS;
}

static inline NTSTATUS NbtDisassociateAddress(PVOID pConnEle) {
    (void)pConnEle; return STATUS_SUCCESS;
}

static inline NTSTATUS NbtConnect(PVOID pConnEle, PVOID pTimeout, PVOID pCallInfo, PVOID pIrp) {
    (void)pConnEle; (void)pTimeout; (void)pCallInfo; (void)pIrp;
    return STATUS_SUCCESS;
}

#ifdef __cplusplus
}
#endif

#endif /* _NBTPROCS_H_ */
