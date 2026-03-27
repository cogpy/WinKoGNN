/*
 * ntlsa.h — NT DDK/SDK Compatibility Header
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
#ifndef _NT_COMPAT_NTLSA_H_H_
#define _NT_COMPAT_NTLSA_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT Local Security Authority */

#ifndef _NTLSA_H_COMPAT
#define _NTLSA_H_COMPAT

#define POLICY_VIEW_LOCAL_INFORMATION                 0x00000001
#define POLICY_VIEW_AUDIT_INFORMATION                 0x00000002
#define POLICY_GET_PRIVATE_INFORMATION                0x00000004
#define POLICY_TRUST_ADMIN                            0x00000008
#define POLICY_CREATE_ACCOUNT                         0x00000010
#define POLICY_CREATE_SECRET                          0x00000020
#define POLICY_CREATE_PRIVILEGE                       0x00000040
#define POLICY_SET_DEFAULT_QUOTA_LIMITS               0x00000080
#define POLICY_SET_AUDIT_REQUIREMENTS                 0x00000100
#define POLICY_AUDIT_LOG_ADMIN                        0x00000200
#define POLICY_SERVER_ADMIN                           0x00000400
#define POLICY_LOOKUP_NAMES                           0x00000800

#endif /* _NTLSA_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTLSA_H_H_ */
