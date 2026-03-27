/*
 * ntddpar.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 4 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTDDPAR_H_H_
#define _NT_COMPAT_NTDDPAR_H_H_

#include "ntdef.h"
#include "devioctl.h"
#ifdef __cplusplus
extern "C" {
#endif

/* NT Parallel Port IOCTL */

#ifndef _NTDDPAR_H_COMPAT
#define _NTDDPAR_H_COMPAT

#define FILE_DEVICE_PARALLEL_PORT                     0x00000016
#define IOCTL_PAR_QUERY_INFORMATION                   CTL_CODE(FILE_DEVICE_PARALLEL_PORT,1,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PAR_SET_INFORMATION                     CTL_CODE(FILE_DEVICE_PARALLEL_PORT,2,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PAR_QUERY_DEVICE_ID                     CTL_CODE(FILE_DEVICE_PARALLEL_PORT,3,METHOD_BUFFERED,FILE_ANY_ACCESS)

#endif /* _NTDDPAR_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTDDPAR_H_H_ */
