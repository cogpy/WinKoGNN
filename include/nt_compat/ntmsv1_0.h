/*
 * ntmsv1_0.h — NT DDK/SDK Compatibility Header
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
#ifndef _NT_COMPAT_NTMSV1_0_H_H_
#define _NT_COMPAT_NTMSV1_0_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT MSV1_0 Authentication */

#ifndef _NTMSV1_0_H_COMPAT
#define _NTMSV1_0_H_COMPAT

#define MSV1_0_PACKAGE_NAME                           "MICROSOFT_AUTHENTICATION_PACKAGE_V1_0"
#define MSV1_0_SUBAUTHENTICATION_KEY                  "SYSTEM\\CurrentControlSet\\Control\\Lsa\\MSV1_0"

#endif /* _NTMSV1_0_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTMSV1_0_H_H_ */
