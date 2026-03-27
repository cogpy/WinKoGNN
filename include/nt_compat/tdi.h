/*
 * tdi.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 36 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_TDI_H_H_
#define _NT_COMPAT_TDI_H_H_

#include "ntdef.h"
#include "tdistat.h"
#ifdef __cplusplus
extern "C" {
#endif


/* Transport Driver Interface base definitions */
#ifndef _TDI_H_COMPAT
#define _TDI_H_COMPAT
#include "tdikrnl.h"
typedef struct _TA_ADDRESS { USHORT AddressLength; USHORT AddressType; UCHAR Address[1]; } TA_ADDRESS, *PTA_ADDRESS;
typedef struct _TRANSPORT_ADDRESS { LONG TAAddressCount; TA_ADDRESS Address[1]; } TRANSPORT_ADDRESS, *PTRANSPORT_ADDRESS;
typedef struct _TDI_ADDRESS_IP { USHORT sin_port; ULONG in_addr; UCHAR sin_zero[8]; } TDI_ADDRESS_IP, *PTDI_ADDRESS_IP;
typedef struct _TDI_ADDRESS_NETBIOS { USHORT NetbiosNameType; UCHAR NetbiosName[16]; } TDI_ADDRESS_NETBIOS, *PTDI_ADDRESS_NETBIOS;
typedef struct _TDI_ADDRESS_IPX { ULONG NetworkAddress; UCHAR NodeAddress[6]; USHORT Socket; } TDI_ADDRESS_IPX, *PTDI_ADDRESS_IPX;
#define TDI_ADDRESS_TYPE_IP         2
#define TDI_ADDRESS_TYPE_NETBIOS    17
#define TDI_ADDRESS_TYPE_IPX        6
#define TDI_ADDRESS_LENGTH_IP       sizeof(TDI_ADDRESS_IP)
#define TDI_ADDRESS_LENGTH_NETBIOS  sizeof(TDI_ADDRESS_NETBIOS)
#define TDI_ADDRESS_LENGTH_IPX      sizeof(TDI_ADDRESS_IPX)
#endif /* _TDI_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_TDI_H_H_ */
