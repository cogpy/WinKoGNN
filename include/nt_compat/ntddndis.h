/*
 * ntddndis.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 11 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTDDNDIS_H_H_
#define _NT_COMPAT_NTDDNDIS_H_H_

#include "ntdef.h"
#include "devioctl.h"
#ifdef __cplusplus
extern "C" {
#endif

/* NT NDIS Device IOCTL */

#ifndef _NTDDNDIS_H_COMPAT
#define _NTDDNDIS_H_COMPAT

#define FILE_DEVICE_NETWORK                           0x00000012
#define IOCTL_NDIS_QUERY_GLOBAL_STATS                 CTL_CODE(FILE_DEVICE_NETWORK,0,METHOD_OUT_DIRECT,FILE_ANY_ACCESS)
#define IOCTL_NDIS_QUERY_ALL_STATS                    CTL_CODE(FILE_DEVICE_NETWORK,1,METHOD_OUT_DIRECT,FILE_ANY_ACCESS)
#define IOCTL_NDIS_ADD_DEVICE                         CTL_CODE(FILE_DEVICE_NETWORK,2,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NDIS_DELETE_DEVICE                      CTL_CODE(FILE_DEVICE_NETWORK,3,METHOD_BUFFERED,FILE_ANY_ACCESS)

#endif /* _NTDDNDIS_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTDDNDIS_H_H_ */
