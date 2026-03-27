/*
 * ntddscsi.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 17 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTDDSCSI_H_H_
#define _NT_COMPAT_NTDDSCSI_H_H_

#include "ntdef.h"
#include "devioctl.h"
#ifdef __cplusplus
extern "C" {
#endif

/* NT SCSI Device IOCTL */

#ifndef _NTDDSCSI_H_COMPAT
#define _NTDDSCSI_H_COMPAT

#define IOCTL_SCSI_PASS_THROUGH                       CTL_CODE(FILE_DEVICE_CONTROLLER,0x0401,METHOD_BUFFERED,FILE_READ_ACCESS|FILE_WRITE_ACCESS)
#define IOCTL_SCSI_MINIPORT                           CTL_CODE(FILE_DEVICE_CONTROLLER,0x0402,METHOD_BUFFERED,FILE_READ_ACCESS|FILE_WRITE_ACCESS)
#define IOCTL_SCSI_GET_INQUIRY_DATA                   CTL_CODE(FILE_DEVICE_CONTROLLER,0x0403,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SCSI_GET_CAPABILITIES                   CTL_CODE(FILE_DEVICE_CONTROLLER,0x0404,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SCSI_PASS_THROUGH_DIRECT                CTL_CODE(FILE_DEVICE_CONTROLLER,0x0405,METHOD_BUFFERED,FILE_READ_ACCESS|FILE_WRITE_ACCESS)
#define IOCTL_SCSI_GET_ADDRESS                        CTL_CODE(FILE_DEVICE_CONTROLLER,0x0406,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SCSI_RESCAN_BUS                         CTL_CODE(FILE_DEVICE_CONTROLLER,0x0407,METHOD_BUFFERED,FILE_ANY_ACCESS)

#endif /* _NTDDSCSI_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTDDSCSI_H_H_ */
