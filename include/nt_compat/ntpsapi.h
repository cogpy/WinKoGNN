/*
 * ntpsapi.h — NT DDK/SDK Compatibility Header
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
#ifndef _NT_COMPAT_NTPSAPI_H_H_
#define _NT_COMPAT_NTPSAPI_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT Process/Thread API */

#ifndef _NTPSAPI_H_COMPAT
#define _NTPSAPI_H_COMPAT

#define PROCESS_BASIC_INFORMATION_CLASS               0
#define PROCESS_DEBUG_PORT                            7
#define PROCESS_WOW64_INFORMATION                     26
#define THREAD_BASIC_INFORMATION_CLASS                0

#endif /* _NTPSAPI_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTPSAPI_H_H_ */
