/*
 * ntddkbd.h — NT DDK/SDK Compatibility Header
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
#ifndef _NT_COMPAT_NTDDKBD_H_H_
#define _NT_COMPAT_NTDDKBD_H_H_

#include "ntdef.h"
#include "devioctl.h"
#ifdef __cplusplus
extern "C" {
#endif

/* NT Keyboard Device IOCTL */

#ifndef _NTDDKBD_H_COMPAT
#define _NTDDKBD_H_COMPAT

#define FILE_DEVICE_KEYBOARD                          0x0000000B
#define IOCTL_KEYBOARD_QUERY_ATTRIBUTES               CTL_CODE(FILE_DEVICE_KEYBOARD,0,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_KEYBOARD_QUERY_INDICATORS               CTL_CODE(FILE_DEVICE_KEYBOARD,0x10,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_KEYBOARD_SET_INDICATORS                 CTL_CODE(FILE_DEVICE_KEYBOARD,2,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define KEYBOARD_LED_NUM_LOCK                         0x01
#define KEYBOARD_LED_CAPS_LOCK                        0x02
#define KEYBOARD_LED_SCROLL_LOCK                      0x04

#endif /* _NTDDKBD_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTDDKBD_H_H_ */
