/*
 * ntddvdm.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 1 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTDDVDM_H_H_
#define _NT_COMPAT_NTDDVDM_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT VDM Device IOCTL */

#ifndef _NTDDVDM_H_COMPAT
#define _NTDDVDM_H_COMPAT

#define FILE_DEVICE_VDM                               0x0000002C

#endif /* _NTDDVDM_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTDDVDM_H_H_ */
