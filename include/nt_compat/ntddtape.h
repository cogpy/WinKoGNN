/*
 * ntddtape.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 32 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTDDTAPE_H_H_
#define _NT_COMPAT_NTDDTAPE_H_H_

#include "ntdef.h"
#include "devioctl.h"
#ifdef __cplusplus
extern "C" {
#endif

/* NT Tape Device IOCTL */

#ifndef _NTDDTAPE_H_COMPAT
#define _NTDDTAPE_H_COMPAT

#define FILE_DEVICE_TAPE                              0x0000001F
#define IOCTL_TAPE_ERASE                              CTL_CODE(FILE_DEVICE_TAPE,0x00,METHOD_BUFFERED,FILE_READ_ACCESS)
#define IOCTL_TAPE_PREPARE                            CTL_CODE(FILE_DEVICE_TAPE,0x01,METHOD_BUFFERED,FILE_READ_ACCESS)
#define IOCTL_TAPE_WRITE_MARKS                        CTL_CODE(FILE_DEVICE_TAPE,0x02,METHOD_BUFFERED,FILE_READ_ACCESS)
#define IOCTL_TAPE_GET_POSITION                       CTL_CODE(FILE_DEVICE_TAPE,0x03,METHOD_BUFFERED,FILE_READ_ACCESS)
#define IOCTL_TAPE_SET_POSITION                       CTL_CODE(FILE_DEVICE_TAPE,0x04,METHOD_BUFFERED,FILE_READ_ACCESS)
#define IOCTL_TAPE_GET_DRIVE_PARAMS                   CTL_CODE(FILE_DEVICE_TAPE,0x05,METHOD_BUFFERED,FILE_READ_ACCESS)
#define IOCTL_TAPE_SET_DRIVE_PARAMS                   CTL_CODE(FILE_DEVICE_TAPE,0x06,METHOD_BUFFERED,FILE_READ_ACCESS|FILE_WRITE_ACCESS)
#define IOCTL_TAPE_GET_MEDIA_PARAMS                   CTL_CODE(FILE_DEVICE_TAPE,0x07,METHOD_BUFFERED,FILE_READ_ACCESS)
#define IOCTL_TAPE_SET_MEDIA_PARAMS                   CTL_CODE(FILE_DEVICE_TAPE,0x08,METHOD_BUFFERED,FILE_READ_ACCESS)
#define IOCTL_TAPE_GET_STATUS                         CTL_CODE(FILE_DEVICE_TAPE,0x09,METHOD_BUFFERED,FILE_READ_ACCESS)
#define TAPE_ERASE_SHORT                              0
#define TAPE_ERASE_LONG                               1

#endif /* _NTDDTAPE_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTDDTAPE_H_H_ */
