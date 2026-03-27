/*
 * ntseapi.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 5 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTSEAPI_H_H_
#define _NT_COMPAT_NTSEAPI_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT Security API */

#ifndef _NTSEAPI_H_COMPAT
#define _NTSEAPI_H_COMPAT

#ifndef TOKEN_ASSIGN_PRIMARY
#define TOKEN_ASSIGN_PRIMARY                          0x0001
#endif
#ifndef TOKEN_DUPLICATE
#define TOKEN_DUPLICATE                               0x0002
#endif
#ifndef TOKEN_IMPERSONATE
#define TOKEN_IMPERSONATE                             0x0004
#endif
#ifndef TOKEN_QUERY
#define TOKEN_QUERY                                   0x0008
#endif
#ifndef TOKEN_QUERY_SOURCE
#define TOKEN_QUERY_SOURCE                            0x0010
#endif
#ifndef TOKEN_ADJUST_PRIVILEGES
#define TOKEN_ADJUST_PRIVILEGES                       0x0020
#endif
#ifndef TOKEN_ADJUST_GROUPS
#define TOKEN_ADJUST_GROUPS                           0x0040
#endif
#ifndef TOKEN_ADJUST_DEFAULT
#define TOKEN_ADJUST_DEFAULT                          0x0080
#endif
#ifndef TOKEN_ALL_ACCESS
#define TOKEN_ALL_ACCESS                              0xF00FF
#endif
#ifndef SE_PRIVILEGE_ENABLED_BY_DEFAULT
#define SE_PRIVILEGE_ENABLED_BY_DEFAULT               0x00000001
#endif
#ifndef SE_PRIVILEGE_ENABLED
#define SE_PRIVILEGE_ENABLED                          0x00000002
#endif
#ifndef SE_PRIVILEGE_USED_FOR_ACCESS
#define SE_PRIVILEGE_USED_FOR_ACCESS                  0x80000000
#endif

#endif /* _NTSEAPI_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTSEAPI_H_H_ */
