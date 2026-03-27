/*
 * ntsam.h — NT DDK/SDK Compatibility Header
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
#ifndef _NT_COMPAT_NTSAM_H_H_
#define _NT_COMPAT_NTSAM_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT Security Account Manager */

#ifndef _NTSAM_H_COMPAT
#define _NTSAM_H_COMPAT

#define SAM_SERVER_CONNECT                            0x0001
#define SAM_SERVER_SHUTDOWN                           0x0002
#define SAM_SERVER_INITIALIZE                         0x0004
#define SAM_SERVER_CREATE_DOMAIN                      0x0008
#define SAM_SERVER_ENUMERATE_DOMAINS                  0x0010
#define SAM_SERVER_LOOKUP_DOMAIN                      0x0020
#define SAM_SERVER_ALL_ACCESS                         0x000F003F

#endif /* _NTSAM_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTSAM_H_H_ */
