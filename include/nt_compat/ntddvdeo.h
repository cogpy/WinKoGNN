/*
 * ntddvdeo.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 98 source files.
 *
 * IMPORTANT: On _WIN32, we define the Windows SDK guard macro
 * _NTDDVDEO_ to prevent the SDK's own ntddvdeo.h from being
 * included (which would cause 7k+ cascading errors due to
 * CTL_CODE macro conflicts).
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTDDVDEO_H_H_
#define _NT_COMPAT_NTDDVDEO_H_H_

/* Prevent the Windows SDK ntddvdeo.h from being included */
#ifndef _NTDDVDEO_
#define _NTDDVDEO_
#endif

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT Video Device I/O Control definitions */

#ifndef FILE_DEVICE_VIDEO
#define FILE_DEVICE_VIDEO   0x00000023
#endif

/* CTL_CODE macro if not defined */
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

#ifndef IOCTL_VIDEO_QUERY_AVAIL_MODES
#define IOCTL_VIDEO_QUERY_AVAIL_MODES       CTL_CODE(FILE_DEVICE_VIDEO, 0x00, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_SET_CURRENT_MODE        CTL_CODE(FILE_DEVICE_VIDEO, 0x01, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_RESET_DEVICE            CTL_CODE(FILE_DEVICE_VIDEO, 0x02, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_MAP_VIDEO_MEMORY        CTL_CODE(FILE_DEVICE_VIDEO, 0x03, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_UNMAP_VIDEO_MEMORY      CTL_CODE(FILE_DEVICE_VIDEO, 0x04, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_SHARE_VIDEO_MEMORY      CTL_CODE(FILE_DEVICE_VIDEO, 0x05, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_QUERY_NUM_AVAIL_MODES   CTL_CODE(FILE_DEVICE_VIDEO, 0x06, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_QUERY_CURRENT_MODE      CTL_CODE(FILE_DEVICE_VIDEO, 0x07, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_SET_COLOR_REGISTERS     CTL_CODE(FILE_DEVICE_VIDEO, 0x08, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_LOAD_AND_SET_FONT       CTL_CODE(FILE_DEVICE_VIDEO, 0x09, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_SET_PALETTE_REGISTERS   CTL_CODE(FILE_DEVICE_VIDEO, 0x0A, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_ENABLE_CURSOR           CTL_CODE(FILE_DEVICE_VIDEO, 0x0B, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_DISABLE_CURSOR          CTL_CODE(FILE_DEVICE_VIDEO, 0x0C, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_SET_CURSOR_ATTR         CTL_CODE(FILE_DEVICE_VIDEO, 0x0D, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_SET_CURSOR_POSITION     CTL_CODE(FILE_DEVICE_VIDEO, 0x0E, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_QUERY_CURSOR_POSITION   CTL_CODE(FILE_DEVICE_VIDEO, 0x0F, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_QUERY_POINTER_ATTR      CTL_CODE(FILE_DEVICE_VIDEO, 0x10, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_SET_POINTER_ATTR        CTL_CODE(FILE_DEVICE_VIDEO, 0x11, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_ENABLE_POINTER          CTL_CODE(FILE_DEVICE_VIDEO, 0x12, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_DISABLE_POINTER         CTL_CODE(FILE_DEVICE_VIDEO, 0x13, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_SET_POINTER_POSITION    CTL_CODE(FILE_DEVICE_VIDEO, 0x14, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_QUERY_POINTER_POSITION  CTL_CODE(FILE_DEVICE_VIDEO, 0x15, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_QUERY_POINTER_CAPABILITIES CTL_CODE(FILE_DEVICE_VIDEO, 0x16, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_FREE_PUBLIC_ACCESS_RANGES CTL_CODE(FILE_DEVICE_VIDEO, 0x17, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_QUERY_PUBLIC_ACCESS_RANGES CTL_CODE(FILE_DEVICE_VIDEO, 0x18, METHOD_BUFFERED, FILE_ANY_ACCESS)
#endif

#ifndef _VIDEO_MODE_INFORMATION_DEFINED
#define _VIDEO_MODE_INFORMATION_DEFINED
typedef struct _VIDEO_MODE_INFORMATION {
    ULONG Length;
    ULONG ModeIndex;
    ULONG VisScreenWidth;
    ULONG VisScreenHeight;
    ULONG ScreenStride;
    ULONG NumberOfPlanes;
    ULONG BitsPerPlane;
    ULONG Frequency;
    ULONG XMillimeter;
    ULONG YMillimeter;
    ULONG NumberRedBits;
    ULONG NumberGreenBits;
    ULONG NumberBlueBits;
    ULONG RedMask;
    ULONG GreenMask;
    ULONG BlueMask;
    ULONG AttributeFlags;
    ULONG VideoMemoryBitmapWidth;
    ULONG VideoMemoryBitmapHeight;
    ULONG DriverSpecificAttributeFlags;
} VIDEO_MODE_INFORMATION, *PVIDEO_MODE_INFORMATION;
#endif

#ifndef _VIDEO_NUM_MODES_DEFINED
#define _VIDEO_NUM_MODES_DEFINED
typedef struct _VIDEO_NUM_MODES {
    ULONG NumModes;
    ULONG ModeInformationLength;
} VIDEO_NUM_MODES, *PVIDEO_NUM_MODES;
#endif

#ifndef _VIDEO_MEMORY_DEFINED
#define _VIDEO_MEMORY_DEFINED
typedef struct _VIDEO_MEMORY {
    PVOID RequestedVirtualAddress;
} VIDEO_MEMORY, *PVIDEO_MEMORY;

typedef struct _VIDEO_MEMORY_INFORMATION {
    PVOID VideoRamBase;
    ULONG VideoRamLength;
    PVOID FrameBufferBase;
    ULONG FrameBufferLength;
} VIDEO_MEMORY_INFORMATION, *PVIDEO_MEMORY_INFORMATION;
#endif

/* ── VIDEO_BANK_TYPE — video bank switching mode ── */
#ifndef _VIDEO_BANK_TYPE_DEFINED
#define _VIDEO_BANK_TYPE_DEFINED
typedef enum _VIDEO_BANK_TYPE {
    VideoNotBanked = 0,
    VideoBanked1RW,
    VideoBanked1R1W,
    VideoBanked2RW,
    NumVideoBankTypes
} VIDEO_BANK_TYPE, *PVIDEO_BANK_TYPE;
#endif

/* ── VIDEO_CLUTDATA — color lookup table entry ── */
#ifndef _VIDEO_CLUTDATA_DEFINED
#define _VIDEO_CLUTDATA_DEFINED
typedef struct _VIDEO_CLUTDATA {
    UCHAR Red;
    UCHAR Green;
    UCHAR Blue;
    UCHAR Unused;
} VIDEO_CLUTDATA, *PVIDEO_CLUTDATA;
#endif

/* ── VIDEO_CLUT — color lookup table ── */
#ifndef _VIDEO_CLUT_DEFINED
#define _VIDEO_CLUT_DEFINED
typedef struct _VIDEO_CLUT {
    USHORT          NumEntries;
    USHORT          FirstEntry;
    union {
        VIDEO_CLUTDATA  RgbArray;
        ULONG           RgbLong;
    } LookupTable[1];
} VIDEO_CLUT, *PVIDEO_CLUT;
#endif

/* ── VIDEO_CURSOR_POSITION — cursor position ── */
#ifndef _VIDEO_CURSOR_POSITION_DEFINED
#define _VIDEO_CURSOR_POSITION_DEFINED
typedef struct _VIDEO_CURSOR_POSITION {
    SHORT Column;
    SHORT Row;
} VIDEO_CURSOR_POSITION, *PVIDEO_CURSOR_POSITION;
#endif

/* ── Video miniport function pointer types ── */
#ifndef _VIDEO_HW_TYPES_DEFINED
#define _VIDEO_HW_TYPES_DEFINED
struct _VIDEO_PORT_CONFIG_INFO;
typedef struct _VIDEO_PORT_CONFIG_INFO VIDEO_PORT_CONFIG_INFO, *PVIDEO_PORT_CONFIG_INFO;

typedef ULONG (*PVIDEO_HW_FIND_ADAPTER)(
    PVOID HwDeviceExtension,
    PVOID HwContext,
    PWSTR ArgumentString,
    PVIDEO_PORT_CONFIG_INFO ConfigInfo,
    PUCHAR Again);

typedef BOOLEAN (*PVIDEO_HW_INITIALIZE)(PVOID HwDeviceExtension);
typedef BOOLEAN (*PVIDEO_HW_INTERRUPT)(PVOID HwDeviceExtension);

typedef BOOLEAN (*PVIDEO_HW_START_IO)(
    PVOID HwDeviceExtension,
    PVOID RequestPacket);

typedef BOOLEAN (*PVIDEO_HW_RESET_HW)(
    PVOID HwDeviceExtension,
    ULONG Columns,
    ULONG Rows);

/* VIDEO_HW_INITIALIZATION_DATA — video miniport init structure */
#ifndef _VIDEO_HW_INITIALIZATION_DATA_DEFINED
#define _VIDEO_HW_INITIALIZATION_DATA_DEFINED
typedef struct _VIDEO_HW_INITIALIZATION_DATA {
    ULONG   HwInitDataSize;
    ULONG   AdapterInterfaceType;  /* INTERFACE_TYPE */
    PVIDEO_HW_FIND_ADAPTER  HwFindAdapter;
    PVIDEO_HW_INITIALIZE    HwInitialize;
    PVIDEO_HW_INTERRUPT     HwInterrupt;
    PVIDEO_HW_START_IO      HwStartIO;
    ULONG   HwDeviceExtensionSize;
    ULONG   StartingDeviceNumber;
    ULONG   HwLegacyResourceList;
    ULONG   HwLegacyResourceCount;
    PVIDEO_HW_RESET_HW      HwResetHw;
} VIDEO_HW_INITIALIZATION_DATA, *PVIDEO_HW_INITIALIZATION_DATA;
#endif /* _VIDEO_HW_INITIALIZATION_DATA_DEFINED */

/* VIDEOMODE — used by video drivers */
#ifndef _VIDEOMODE_DEFINED
#define _VIDEOMODE_DEFINED
typedef struct _VIDEOMODE {
    ULONG   Length;
    ULONG   ModeIndex;
    ULONG   VisScreenWidth;
    ULONG   VisScreenHeight;
    ULONG   ScreenStride;
    ULONG   NumberOfPlanes;
    ULONG   BitsPerPlane;
    ULONG   Frequency;
    ULONG   XMillimeter;
    ULONG   YMillimeter;
    ULONG   NumberRedBits;
    ULONG   NumberGreenBits;
    ULONG   NumberBlueBits;
    ULONG   RedMask;
    ULONG   GreenMask;
    ULONG   BlueMask;
    ULONG   AttributeFlags;
} VIDEOMODE, *PVIDEOMODE;
#endif /* _VIDEOMODE_DEFINED */

#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTDDVDEO_H_H_ */
