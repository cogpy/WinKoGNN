/*
 * tdikrnl.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 39 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_TDIKRNL_H_H_
#define _NT_COMPAT_TDIKRNL_H_H_

#include "ntdef.h"
#include "tdistat.h"
#include "ntddk.h"
#ifdef __cplusplus
extern "C" {
#endif


/* TDI Kernel-mode interface for transport drivers */
#ifndef _TDIKRNL_H_COMPAT
#define _TDIKRNL_H_COMPAT
typedef LONG TDI_STATUS;
typedef PVOID CONNECTION_CONTEXT;
typedef struct _TDI_CONNECTION_INFORMATION {
    LONG UserDataLength; PVOID UserData;
    LONG OptionsLength; PVOID Options;
    LONG RemoteAddressLength; PVOID RemoteAddress;
} TDI_CONNECTION_INFORMATION, *PTDI_CONNECTION_INFORMATION;
typedef struct _TDI_REQUEST {
    union { HANDLE AddressHandle; CONNECTION_CONTEXT ConnectionContext; HANDLE ControlChannel; } Handle;
    PVOID RequestNotifyObject; PVOID RequestContext;
    TDI_STATUS TdiStatus;
} TDI_REQUEST, *PTDI_REQUEST;
typedef struct _TDI_REQUEST_KERNEL {
    ULONG RequestFlags; PTDI_CONNECTION_INFORMATION RequestConnectionInformation;
    PTDI_CONNECTION_INFORMATION ReturnConnectionInformation; PVOID RequestSpecific;
} TDI_REQUEST_KERNEL, *PTDI_REQUEST_KERNEL;
#define TDI_TRANSPORT_ADDRESS_FILE  1
#define TDI_CONNECTION_FILE         2
#define TDI_CONTROL_CHANNEL_FILE    3
#define TDI_SEND_EXPEDITED          0x0020
#define TDI_SEND_PARTIAL            0x0040
#define TDI_RECEIVE_NORMAL          0x0000
#define TDI_RECEIVE_EXPEDITED       0x0001
#define TDI_RECEIVE_PEEK            0x0002
#endif /* _TDIKRNL_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_TDIKRNL_H_H_ */
