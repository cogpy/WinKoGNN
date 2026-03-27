/*
 * ntddmou.h — NT DDK/SDK Compatibility Header
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
#ifndef _NT_COMPAT_NTDDMOU_H_H_
#define _NT_COMPAT_NTDDMOU_H_H_

#include "ntdef.h"
#include "devioctl.h"
#ifdef __cplusplus
extern "C" {
#endif

/* NT Mouse Device IOCTL */

#ifndef _NTDDMOU_H_COMPAT
#define _NTDDMOU_H_COMPAT

#define FILE_DEVICE_MOUSE                             0x0000000F
#define IOCTL_MOUSE_QUERY_ATTRIBUTES                  CTL_CODE(FILE_DEVICE_MOUSE,0,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define MOUSE_LEFT_BUTTON_DOWN                        0x0001
#define MOUSE_LEFT_BUTTON_UP                          0x0002
#define MOUSE_RIGHT_BUTTON_DOWN                       0x0004
#define MOUSE_RIGHT_BUTTON_UP                         0x0008
#define MOUSE_MIDDLE_BUTTON_DOWN                      0x0010
#define MOUSE_MIDDLE_BUTTON_UP                        0x0020

#endif /* _NTDDMOU_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTDDMOU_H_H_ */
