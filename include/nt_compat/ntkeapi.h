/*
 * ntkeapi.h — NT DDK/SDK Compatibility Header
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
#ifndef _NT_COMPAT_NTKEAPI_H_H_
#define _NT_COMPAT_NTKEAPI_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT Kernel Executive API */

#ifndef _NTKEAPI_H_COMPAT
#define _NTKEAPI_H_COMPAT

#ifndef LOW_PRIORITY
#define LOW_PRIORITY                                  0
#endif
#ifndef LOW_REALTIME_PRIORITY
#define LOW_REALTIME_PRIORITY                         16
#endif
#ifndef HIGH_PRIORITY
#define HIGH_PRIORITY                                 31
#endif
#ifndef MAXIMUM_PRIORITY
#define MAXIMUM_PRIORITY                              32
#endif
#ifndef THREAD_TERMINATE
#define THREAD_TERMINATE                              0x0001
#endif
#ifndef THREAD_SET_INFORMATION
#define THREAD_SET_INFORMATION                        0x0020
#endif
#ifndef THREAD_QUERY_INFORMATION
#define THREAD_QUERY_INFORMATION                      0x0040
#endif
#ifndef THREAD_ALL_ACCESS
#define THREAD_ALL_ACCESS                             0x001F03FF
#endif

#endif /* _NTKEAPI_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTKEAPI_H_H_ */
