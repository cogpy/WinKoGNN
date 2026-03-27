/*++ BUILD Version: 0001
 * miniport.h — Video Miniport Driver Compatibility Shim
 *--*/
#ifndef _MINIPORT_H_
#define _MINIPORT_H_

#include "ntddk.h"
#include "dderror.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── Video port types ── */
typedef LONG VP_STATUS;
typedef PVOID PVIDEO_PORT_HANDLE;

/* ── Interface type ── */
#ifndef _INTERFACE_TYPE_DEFINED
#define _INTERFACE_TYPE_DEFINED
typedef enum _INTERFACE_TYPE {
    InterfaceTypeUndefined = -1,
    Internal = 0,
    Isa,
    Eisa,
    MicroChannel,
    TurboChannel,
    PCIBus,
    VMEBus,
    NuBus,
    PCMCIABus,
    CBus,
    MPIBus,
    MPSABus,
    MaximumInterfaceType
} INTERFACE_TYPE, *PINTERFACE_TYPE;
#endif

/* ── Bus data type ── */
#ifndef _BUS_DATA_TYPE_DEFINED
#define _BUS_DATA_TYPE_DEFINED
typedef enum _BUS_DATA_TYPE {
    ConfigurationSpaceUndefined = -1,
    Cmos,
    EisaConfiguration,
    Pos,
    CbusConfiguration,
    PCIConfiguration,
    VMEConfiguration,
    NuBusConfiguration,
    PCMCIAConfiguration,
    MaximumBusDataType
} BUS_DATA_TYPE, *PBUS_DATA_TYPE;
#endif

/* ── Video access range ── */
#ifndef _VIDEO_ACCESS_RANGE_DEFINED
#define _VIDEO_ACCESS_RANGE_DEFINED
typedef struct _VIDEO_ACCESS_RANGE {
    LARGE_INTEGER   RangeStart;
    ULONG           RangeLength;
    UCHAR           RangeInIoSpace;
    UCHAR           RangeVisible;
    UCHAR           RangeShareable;
} VIDEO_ACCESS_RANGE, *PVIDEO_ACCESS_RANGE;
#endif

/* ── Video request packet ── */
#ifndef _VIDEO_REQUEST_PACKET_DEFINED
#define _VIDEO_REQUEST_PACKET_DEFINED
typedef struct _VIDEO_REQUEST_PACKET {
    ULONG           IoControlCode;
    IO_STATUS_BLOCK StatusBlock;
    PVOID           InputBuffer;
    ULONG           InputBufferLength;
    PVOID           OutputBuffer;
    ULONG           OutputBufferLength;
} VIDEO_REQUEST_PACKET, *PVIDEO_REQUEST_PACKET;
#endif

/* ── PCI common config ── */
#ifndef _PCI_COMMON_CONFIG_DEFINED
#define _PCI_COMMON_CONFIG_DEFINED
typedef struct _PCI_COMMON_CONFIG {
    USHORT  VendorID;
    USHORT  DeviceID;
    USHORT  Command;
    USHORT  Status;
    UCHAR   RevisionID;
    UCHAR   ProgIf;
    UCHAR   SubClass;
    UCHAR   BaseClass;
    UCHAR   CacheLineSize;
    UCHAR   LatencyTimer;
    UCHAR   HeaderType;
    UCHAR   BIST;
    ULONG   BaseAddresses[6];
    ULONG   Reserved1[2];
    ULONG   ROMBaseAddress;
    ULONG   Reserved2[2];
    UCHAR   InterruptLine;
    UCHAR   InterruptPin;
    UCHAR   MinimumGrant;
    UCHAR   MaximumLatency;
} PCI_COMMON_CONFIG, *PPCI_COMMON_CONFIG;
#endif

#ifndef PCI_COMMON_HDR_LENGTH
#define PCI_COMMON_HDR_LENGTH (sizeof(PCI_COMMON_CONFIG))
#endif

/* ── Video port function stubs ── */
static inline VP_STATUS VideoPortVerifyAccessRanges(
    PVOID HwDeviceExtension, ULONG NumAccessRanges,
    PVIDEO_ACCESS_RANGE AccessRanges) {
    (void)HwDeviceExtension; (void)NumAccessRanges; (void)AccessRanges;
    return NO_ERROR;
}

static inline PVOID VideoPortGetDeviceBase(
    PVOID HwDeviceExtension, LARGE_INTEGER IoAddress,
    ULONG NumberOfUchars, UCHAR InIoSpace) {
    (void)HwDeviceExtension; (void)IoAddress; (void)NumberOfUchars; (void)InIoSpace;
    return NULL;
}

static inline void VideoPortFreeDeviceBase(PVOID HwDeviceExtension, PVOID MappedAddress) {
    (void)HwDeviceExtension; (void)MappedAddress;
}

static inline ULONG VideoPortGetBusData(
    PVOID HwDeviceExtension, BUS_DATA_TYPE BusDataType,
    ULONG SlotNumber, PVOID Buffer, ULONG Offset, ULONG Length) {
    (void)HwDeviceExtension; (void)BusDataType; (void)SlotNumber;
    (void)Buffer; (void)Offset; (void)Length;
    return 0;
}

/* ── Port I/O macros ── */
#define VideoPortReadPortUchar(Port)          (0)
#define VideoPortReadPortUshort(Port)         (0)
#define VideoPortReadPortUlong(Port)          (0)
#define VideoPortWritePortUchar(Port, Value)  ((void)0)
#define VideoPortWritePortUshort(Port, Value) ((void)0)
#define VideoPortWritePortUlong(Port, Value)  ((void)0)

/* ── Register I/O macros ── */
#define VideoPortReadRegisterUchar(Reg)          (0)
#define VideoPortReadRegisterUshort(Reg)         (0)
#define VideoPortReadRegisterUlong(Reg)          (0)
#define VideoPortWriteRegisterUchar(Reg, Value)  ((void)0)
#define VideoPortWriteRegisterUshort(Reg, Value) ((void)0)
#define VideoPortWriteRegisterUlong(Reg, Value)  ((void)0)

#ifdef __cplusplus
}
#endif

#endif /* _MINIPORT_H_ */
