/*
 * wdbgexts.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 12 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_WDBGEXTS_H_H_
#define _NT_COMPAT_WDBGEXTS_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* WinDbg Extensions */

#ifndef _WDBGEXTS_H_COMPAT
#define _WDBGEXTS_H_COMPAT

#define WDBGAPI                                       NTAPI
#define DECLARE_API(s)                                VOID WDBGAPI s(HANDLE hCurrentProcess, HANDLE hCurrentThread, ULONG dwCurrentPc, ULONG dwProcessor, PCSTR args)

#endif /* _WDBGEXTS_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_WDBGEXTS_H_H_ */
