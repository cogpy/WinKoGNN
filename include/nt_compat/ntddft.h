/*
 * ntddft.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 9 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTDDFT_H_H_
#define _NT_COMPAT_NTDDFT_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT Fault Tolerance IOCTL */

#ifndef _NTDDFT_H_COMPAT
#define _NTDDFT_H_COMPAT

#define FILE_DEVICE_DISK                              0x00000007
#define FT_MIRROR                                     0
#define FT_STRIPE                                     1
#define FT_STRIPE_WITH_PARITY                         2
#define FT_VOLUME_SET                                 3
#define FT_REDISTRIBUTION                             4

#endif /* _NTDDFT_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTDDFT_H_H_ */
