/*
 * ntldr.h — NT DDK/SDK Compatibility Header
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
#ifndef _NT_COMPAT_NTLDR_H_H_
#define _NT_COMPAT_NTLDR_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT Loader */

#ifndef _NTLDR_H_COMPAT
#define _NTLDR_H_COMPAT

#define LDR_DLL_NOTIFICATION_REASON_LOADED            1
#define LDR_DLL_NOTIFICATION_REASON_UNLOADED          2

#endif /* _NTLDR_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTLDR_H_H_ */
