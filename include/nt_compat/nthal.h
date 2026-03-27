/*
 * nthal.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 65 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTHAL_H_H_
#define _NT_COMPAT_NTHAL_H_H_

#include "ntdef.h"
#include "nt.h"
#ifdef __cplusplus
extern "C" {
#endif


/* NT Hardware Abstraction Layer interface */

#ifndef _NTHAL_H_COMPAT
#define _NTHAL_H_COMPAT

/* HAL dispatch table version */
#define HAL_DISPATCH_VERSION 3

/* Bus types */
typedef enum _INTERFACE_TYPE {
    InterfaceTypeUndefined = -1,
    Internal, Isa, Eisa, MicroChannel, TurboChannel,
    PCIBus, VMEBus, NuBus, PCMCIABus, CBus,
    MPIBus, MPSABus, ProcessorInternal, InternalPowerBus,
    PNPISABus, PNPBus, MaximumInterfaceType
} INTERFACE_TYPE, *PINTERFACE_TYPE;

typedef enum _BUS_DATA_TYPE {
    ConfigurationSpaceUndefined = -1,
    Cmos, EisaConfiguration, Pos, CbusConfiguration,
    PCIConfiguration, VMEConfiguration, NuBusConfiguration,
    PCMCIAConfiguration, MPIConfiguration, MPSAConfiguration,
    PNPISAConfiguration, MaximumBusDataType
} BUS_DATA_TYPE, *PBUS_DATA_TYPE;

typedef enum _DMA_WIDTH { Width8Bits, Width16Bits, Width32Bits, MaximumDmaWidth } DMA_WIDTH, *PDMA_WIDTH;
typedef enum _DMA_SPEED { Compatible, TypeA, TypeB, TypeC, TypeF, MaximumDmaSpeed } DMA_SPEED, *PDMA_SPEED;

/* DMA adapter */
typedef struct _DEVICE_DESCRIPTION {
    ULONG Version;
    BOOLEAN Master;
    BOOLEAN ScatterGather;
    BOOLEAN DemandMode;
    BOOLEAN AutoInitialize;
    BOOLEAN Dma32BitAddresses;
    BOOLEAN IgnoreCount;
    BOOLEAN Reserved1;
    BOOLEAN Reserved2;
    ULONG BusNumber;
    ULONG DmaChannel;
    INTERFACE_TYPE InterfaceType;
    DMA_WIDTH DmaWidth;
    DMA_SPEED DmaSpeed;
    ULONG MaximumLength;
    ULONG DmaPort;
} DEVICE_DESCRIPTION, *PDEVICE_DESCRIPTION;

/* HAL functions */
ULONG NTAPI HalGetBusData(IN BUS_DATA_TYPE BusDataType, IN ULONG BusNumber,
    IN ULONG SlotNumber, IN PVOID Buffer, IN ULONG Length);
ULONG NTAPI HalSetBusData(IN BUS_DATA_TYPE BusDataType, IN ULONG BusNumber,
    IN ULONG SlotNumber, IN PVOID Buffer, IN ULONG Length);
ULONG NTAPI HalGetBusDataByOffset(IN BUS_DATA_TYPE BusDataType, IN ULONG BusNumber,
    IN ULONG SlotNumber, IN PVOID Buffer, IN ULONG Offset, IN ULONG Length);
BOOLEAN NTAPI HalTranslateBusAddress(IN INTERFACE_TYPE InterfaceType, IN ULONG BusNumber,
    IN PHYSICAL_ADDRESS BusAddress, IN OUT PULONG AddressSpace, OUT PPHYSICAL_ADDRESS TranslatedAddress);
PVOID NTAPI HalAllocateCommonBuffer(IN PVOID AdapterObject, IN ULONG Length,
    OUT PPHYSICAL_ADDRESS LogicalAddress, IN BOOLEAN CacheEnabled);
VOID NTAPI HalFreeCommonBuffer(IN PVOID AdapterObject, IN ULONG Length,
    IN PHYSICAL_ADDRESS LogicalAddress, IN PVOID VirtualAddress, IN BOOLEAN CacheEnabled);

#endif /* _NTHAL_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTHAL_H_H_ */
