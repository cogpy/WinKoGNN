/*
 * ntddbeep.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 6 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTDDBEEP_H_H_
#define _NT_COMPAT_NTDDBEEP_H_H_

#include "ntdef.h"
#include "devioctl.h"
#ifdef __cplusplus
extern "C" {
#endif

/* NT Beep Device IOCTL */

#ifndef _NTDDBEEP_H_COMPAT
#define _NTDDBEEP_H_COMPAT

#define FILE_DEVICE_BEEP                              0x00000001
#define IOCTL_BEEP_SET                                CTL_CODE(FILE_DEVICE_BEEP,0,METHOD_BUFFERED,FILE_ANY_ACCESS)

#endif /* _NTDDBEEP_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTDDBEEP_H_H_ */
