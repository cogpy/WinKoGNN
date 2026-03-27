/*
 * devioctl.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 75 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_DEVIOCTL_H_H_
#define _NT_COMPAT_DEVIOCTL_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Device I/O Control definitions — CTL_CODE macro and FILE_DEVICE constants */

#ifndef _DEVIOCTL_H_COMPAT
#define _DEVIOCTL_H_COMPAT

#ifndef CTL_CODE
#define METHOD_BUFFERED     0
#define METHOD_IN_DIRECT    1
#define METHOD_OUT_DIRECT   2
#define METHOD_NEITHER      3
#define FILE_ANY_ACCESS     0
#define FILE_READ_ACCESS    1
#define FILE_WRITE_ACCESS   2
#define CTL_CODE(DeviceType, Function, Method, Access) \
    (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))
#endif

#define FILE_DEVICE_BEEP                0x00000001
#define FILE_DEVICE_CD_ROM              0x00000002
#define FILE_DEVICE_CD_ROM_FILE_SYSTEM  0x00000003
#define FILE_DEVICE_CONTROLLER          0x00000004
#define FILE_DEVICE_DATALINK            0x00000005
#define FILE_DEVICE_DFS                 0x00000006
#define FILE_DEVICE_DISK                0x00000007
#define FILE_DEVICE_DISK_FILE_SYSTEM    0x00000008
#define FILE_DEVICE_FILE_SYSTEM         0x00000009
#define FILE_DEVICE_INPORT_PORT         0x0000000A
#define FILE_DEVICE_KEYBOARD            0x0000000B
#define FILE_DEVICE_MAILSLOT            0x0000000C
#define FILE_DEVICE_MIDI_IN             0x0000000D
#define FILE_DEVICE_MIDI_OUT            0x0000000E
#define FILE_DEVICE_MOUSE               0x0000000F
#define FILE_DEVICE_MULTI_UNC_PROVIDER  0x00000010
#define FILE_DEVICE_NAMED_PIPE          0x00000011
#define FILE_DEVICE_NETWORK             0x00000012
#define FILE_DEVICE_NETWORK_BROWSER     0x00000013
#define FILE_DEVICE_NETWORK_FILE_SYSTEM 0x00000014
#define FILE_DEVICE_NULL                0x00000015
#define FILE_DEVICE_PARALLEL_PORT       0x00000016
#define FILE_DEVICE_PHYSICAL_NETCARD    0x00000017
#define FILE_DEVICE_PRINTER             0x00000018
#define FILE_DEVICE_SCANNER             0x00000019
#define FILE_DEVICE_SERIAL_MOUSE_PORT   0x0000001A
#define FILE_DEVICE_SERIAL_PORT         0x0000001B
#define FILE_DEVICE_SCREEN              0x0000001C
#define FILE_DEVICE_SOUND               0x0000001D
#define FILE_DEVICE_STREAMS             0x0000001E
#define FILE_DEVICE_TAPE                0x0000001F
#define FILE_DEVICE_TAPE_FILE_SYSTEM    0x00000020
#define FILE_DEVICE_TRANSPORT           0x00000021
#define FILE_DEVICE_UNKNOWN             0x00000022
#define FILE_DEVICE_VIDEO               0x00000023
#define FILE_DEVICE_VIRTUAL_DISK        0x00000024
#define FILE_DEVICE_WAVE_IN             0x00000025
#define FILE_DEVICE_WAVE_OUT            0x00000026
#define FILE_DEVICE_8042_PORT           0x00000027
#define FILE_DEVICE_NETWORK_REDIRECTOR  0x00000028
#define FILE_DEVICE_BATTERY             0x00000029
#define FILE_DEVICE_BUS_EXTENDER        0x0000002A
#define FILE_DEVICE_MODEM               0x0000002B
#define FILE_DEVICE_VDM                 0x0000002C
#define FILE_DEVICE_MASS_STORAGE        0x0000002D
#define FILE_DEVICE_CHANGER             0x00000030
#define FILE_DEVICE_ACPI                0x00000032
#define FILE_DEVICE_FULLSCREEN_VIDEO    0x00000034

#endif /* _DEVIOCTL_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_DEVIOCTL_H_H_ */
