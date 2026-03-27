/*
 * ntioapi.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 8 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTIOAPI_H_H_
#define _NT_COMPAT_NTIOAPI_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT I/O API */

#ifndef _NTIOAPI_H_COMPAT
#define _NTIOAPI_H_COMPAT

#define FILE_SUPERSEDE                                0x00000000
#define FILE_OPEN                                     0x00000001
#define FILE_CREATE                                   0x00000002
#define FILE_OPEN_IF                                  0x00000003
#define FILE_OVERWRITE                                0x00000004
#define FILE_OVERWRITE_IF                             0x00000005
#define FILE_DIRECTORY_FILE                           0x00000001
#define FILE_NON_DIRECTORY_FILE                       0x00000040
#define FILE_SYNCHRONOUS_IO_ALERT                     0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT                  0x00000020

#endif /* _NTIOAPI_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTIOAPI_H_H_ */
