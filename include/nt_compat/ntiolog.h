/*
 * ntiolog.h — NT DDK/SDK Compatibility Header
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
#ifndef _NT_COMPAT_NTIOLOG_H_H_
#define _NT_COMPAT_NTIOLOG_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT I/O Log definitions */

#ifndef _NTIOLOG_H_COMPAT
#define _NTIOLOG_H_COMPAT

#define IO_TYPE_ERROR_LOG_MESSAGE                     0x0B
#define ERROR_LOG_LIMIT_SIZE                          240
#define IO_ERROR_LOG_MESSAGE_HEADER_LENGTH            18

#endif /* _NTIOLOG_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTIOLOG_H_H_ */
