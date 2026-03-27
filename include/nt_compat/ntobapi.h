/*
 * ntobapi.h — NT DDK/SDK Compatibility Header
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
#ifndef _NT_COMPAT_NTOBAPI_H_H_
#define _NT_COMPAT_NTOBAPI_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT Object Manager API */

#ifndef _NTOBAPI_H_COMPAT
#define _NTOBAPI_H_COMPAT

#ifndef OBJ_INHERIT
#ifndef OBJ_INHERIT
#define OBJ_INHERIT                                   0x00000002
#endif
#endif
#ifndef OBJ_PERMANENT
#ifndef OBJ_PERMANENT
#define OBJ_PERMANENT                                 0x00000010
#endif
#endif
#ifndef OBJ_EXCLUSIVE
#ifndef OBJ_EXCLUSIVE
#define OBJ_EXCLUSIVE                                 0x00000020
#endif
#endif
#ifndef OBJ_CASE_INSENSITIVE
#ifndef OBJ_CASE_INSENSITIVE
#define OBJ_CASE_INSENSITIVE                          0x00000040
#endif
#endif
#ifndef OBJ_OPENIF
#ifndef OBJ_OPENIF
#define OBJ_OPENIF                                    0x00000080
#endif
#endif
#ifndef OBJ_OPENLINK
#ifndef OBJ_OPENLINK
#define OBJ_OPENLINK                                  0x00000100
#endif
#endif
#ifndef OBJ_KERNEL_HANDLE
#ifndef OBJ_KERNEL_HANDLE
#define OBJ_KERNEL_HANDLE                             0x00000200
#endif
#endif
#ifndef OBJ_VALID_ATTRIBUTES
#ifndef OBJ_VALID_ATTRIBUTES
#define OBJ_VALID_ATTRIBUTES                          0x000003F2
#endif
#endif
#ifndef DUPLICATE_CLOSE_SOURCE
#define DUPLICATE_CLOSE_SOURCE                        0x00000001
#endif
#ifndef DUPLICATE_SAME_ACCESS
#define DUPLICATE_SAME_ACCESS                         0x00000002
#endif

#endif /* _NTOBAPI_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTOBAPI_H_H_ */
