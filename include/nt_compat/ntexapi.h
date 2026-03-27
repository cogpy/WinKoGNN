/*
 * ntexapi.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 7 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTEXAPI_H_H_
#define _NT_COMPAT_NTEXAPI_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT Executive API */

#ifndef _NTEXAPI_H_COMPAT
#define _NTEXAPI_H_COMPAT

#define NtCurrentProcess()                            ((HANDLE)(LONG_PTR)-1)
#define NtCurrentThread()                             ((HANDLE)(LONG_PTR)-2)
#ifndef PROCESS_TERMINATE
#define PROCESS_TERMINATE                             0x0001
#endif
#ifndef PROCESS_CREATE_THREAD
#define PROCESS_CREATE_THREAD                         0x0002
#endif
#ifndef PROCESS_VM_OPERATION
#define PROCESS_VM_OPERATION                          0x0008
#endif
#ifndef PROCESS_VM_READ
#define PROCESS_VM_READ                               0x0010
#endif
#ifndef PROCESS_VM_WRITE
#define PROCESS_VM_WRITE                              0x0020
#endif
#ifndef PROCESS_DUP_HANDLE
#define PROCESS_DUP_HANDLE                            0x0040
#endif
#ifndef PROCESS_CREATE_PROCESS
#define PROCESS_CREATE_PROCESS                        0x0080
#endif
#ifndef PROCESS_SET_QUOTA
#define PROCESS_SET_QUOTA                             0x0100
#endif
#ifndef PROCESS_SET_INFORMATION
#define PROCESS_SET_INFORMATION                       0x0200
#endif
#ifndef PROCESS_QUERY_INFORMATION
#define PROCESS_QUERY_INFORMATION                     0x0400
#endif
#ifndef PROCESS_ALL_ACCESS
#define PROCESS_ALL_ACCESS                            0x001F0FFF
#endif

#endif /* _NTEXAPI_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTEXAPI_H_H_ */
