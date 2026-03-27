/*
 * ntiodump.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 2 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTIODUMP_H_H_
#define _NT_COMPAT_NTIODUMP_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT I/O Dump definitions */

#ifndef _NTIODUMP_H_COMPAT
#define _NTIODUMP_H_COMPAT

#define DUMP_DRIVER_NAME_LENGTH                       15

#endif /* _NTIODUMP_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTIODUMP_H_H_ */
