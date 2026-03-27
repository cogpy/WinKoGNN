/*
 * ntddcdrm.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 3 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTDDCDRM_H_H_
#define _NT_COMPAT_NTDDCDRM_H_H_

#include "ntdef.h"
#include "devioctl.h"
#ifdef __cplusplus
extern "C" {
#endif

/* NT CD-ROM Device IOCTL */

#ifndef _NTDDCDRM_H_COMPAT
#define _NTDDCDRM_H_COMPAT

#define FILE_DEVICE_CD_ROM                            0x00000002
#define IOCTL_CDROM_READ_TOC                          CTL_CODE(FILE_DEVICE_CD_ROM,0,METHOD_BUFFERED,FILE_READ_ACCESS)
#define IOCTL_CDROM_GET_LAST_SESSION                  CTL_CODE(FILE_DEVICE_CD_ROM,14,METHOD_BUFFERED,FILE_READ_ACCESS)

#endif /* _NTDDCDRM_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTDDCDRM_H_H_ */
