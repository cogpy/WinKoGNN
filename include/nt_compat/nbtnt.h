/* nbtnt.h — NetBIOS over TCP/IP NT-specific header stub */
#ifndef _NBTNT_H_COMPAT_
#define _NBTNT_H_COMPAT_
#include "ntdef.h"
#include "nbtprocs.h"
#define NBT_DEVICE_NAME L"\\Device\\NetBT_Tcpip"
typedef struct _NBT_WORK_ITEM_CONTEXT { PVOID pDeviceContext; PVOID pClientEle; PVOID pIrp; LIST_ENTRY Linkage; } NBT_WORK_ITEM_CONTEXT, *PNBT_WORK_ITEM_CONTEXT;
#endif
