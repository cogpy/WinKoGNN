/*++ BUILD Version: 0001
 * ndis.h — Network Driver Interface Specification Compatibility Shim
 *--*/
#ifndef _NDIS_H_
#define _NDIS_H_

#include "ntddk.h"
#include "windef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── NDIS status codes ── */
typedef ULONG NDIS_STATUS;
typedef ULONG NDIS_OID;
typedef PVOID NDIS_HANDLE;

#define NDIS_STATUS_SUCCESS             ((NDIS_STATUS)0x00000000L)
#define NDIS_STATUS_PENDING             ((NDIS_STATUS)0x00000103L)
#define NDIS_STATUS_FAILURE             ((NDIS_STATUS)0xC0000001L)
#define NDIS_STATUS_RESOURCES           ((NDIS_STATUS)0xC000009AL)
#define NDIS_STATUS_NOT_ACCEPTED        ((NDIS_STATUS)0x00010003L)
#define NDIS_STATUS_CLOSING             ((NDIS_STATUS)0xC0010002L)
#define NDIS_STATUS_RESET_START         ((NDIS_STATUS)0x40010004L)
#define NDIS_STATUS_RESET_END           ((NDIS_STATUS)0x40010005L)
#define NDIS_STATUS_MEDIA_CONNECT       ((NDIS_STATUS)0x4001000BL)
#define NDIS_STATUS_MEDIA_DISCONNECT    ((NDIS_STATUS)0x4001000CL)

/* ── Medium types ── */
typedef enum _NDIS_MEDIUM {
    NdisMedium802_3 = 0,
    NdisMedium802_5,
    NdisMediumFddi,
    NdisMediumWan,
    NdisMediumLocalTalk,
    NdisMediumDix,
    NdisMediumArcnetRaw,
    NdisMediumArcnet878_2,
    NdisMediumAtm,
    NdisMediumWirelessWan,
    NdisMediumIrda,
    NdisMediumMax
} NDIS_MEDIUM, *PNDIS_MEDIUM;

/* ── Hardware status ── */
typedef enum _NDIS_HARDWARE_STATUS {
    NdisHardwareStatusReady = 0,
    NdisHardwareStatusInitializing,
    NdisHardwareStatusReset,
    NdisHardwareStatusClosing,
    NdisHardwareStatusNotReady
} NDIS_HARDWARE_STATUS, *PNDIS_HARDWARE_STATUS;

/* ── Physical address ── */
typedef LARGE_INTEGER NDIS_PHYSICAL_ADDRESS, *PNDIS_PHYSICAL_ADDRESS;
#define NDIS_PHYSICAL_ADDRESS_CONST(Low, High) { .QuadPart = ((LONGLONG)(High) << 32) | (Low) }

/* ── NDIS buffer / packet ── */
typedef struct _NDIS_BUFFER {
    struct _NDIS_BUFFER *Next;
    PVOID               VirtualAddress;
    ULONG               Length;
    PVOID               Process;
    ULONG               MappedSystemVa;
} NDIS_BUFFER, *PNDIS_BUFFER;

typedef struct _NDIS_PACKET {
    PNDIS_BUFFER        Head;
    PNDIS_BUFFER        Tail;
    PVOID               Pool;
    UINT                Count;
    ULONG               Flags;
    BOOLEAN             ValidCounts;
    UCHAR               NdisPacketFlags;
    USHORT              NdisPacketOobOffset;
} NDIS_PACKET, *PNDIS_PACKET;

/* ── NDIS_STRING is just UNICODE_STRING ── */
typedef UNICODE_STRING NDIS_STRING, *PNDIS_STRING;

/* ── Spin lock ── */
typedef KSPIN_LOCK NDIS_SPIN_LOCK, *PNDIS_SPIN_LOCK;

#define NdisAllocateSpinLock(SpinLock) KeInitializeSpinLock((PKSPIN_LOCK)(SpinLock))
#define NdisFreeSpinLock(SpinLock)     ((void)(SpinLock))
#define NdisAcquireSpinLock(SpinLock)  ((void)(SpinLock))
#define NdisReleaseSpinLock(SpinLock)  ((void)(SpinLock))

/* ── Memory allocation ── */
#define NdisAllocateMemory(Dest, Length, Flags, HighAddr) \
    (*(PVOID*)(Dest) = malloc(Length), (*(PVOID*)(Dest)) ? NDIS_STATUS_SUCCESS : NDIS_STATUS_RESOURCES)
#define NdisFreeMemory(Addr, Length, Flags) free(Addr)
#define NdisZeroMemory(Dest, Length) memset(Dest, 0, Length)
#define NdisMoveMemory(Dest, Src, Length) memmove(Dest, Src, Length)

/* ── Timer ── */
typedef struct _NDIS_MINIPORT_TIMER {
    KTIMER  Timer;
    KDPC    Dpc;
} NDIS_MINIPORT_TIMER, *PNDIS_MINIPORT_TIMER;

/* ── Wrapper registration stubs ── */
static inline void NdisMRegisterMiniport(NDIS_HANDLE WrapperHandle, PVOID MiniportChars, UINT Size) {
    (void)WrapperHandle; (void)MiniportChars; (void)Size;
}

static inline void NdisMInitializeWrapper(NDIS_HANDLE *WrapperHandle, PVOID SystemSpecific1,
    PVOID SystemSpecific2, PVOID SystemSpecific3) {
    (void)SystemSpecific1; (void)SystemSpecific2; (void)SystemSpecific3;
    *WrapperHandle = (NDIS_HANDLE)1;
}

static inline void NdisTerminateWrapper(NDIS_HANDLE WrapperHandle, PVOID SystemSpecific) {
    (void)WrapperHandle; (void)SystemSpecific;
}

/* ── OID constants ── */
#define OID_GEN_SUPPORTED_LIST          0x00010101
#define OID_GEN_HARDWARE_STATUS         0x00010102
#define OID_GEN_MEDIA_SUPPORTED         0x00010103
#define OID_GEN_MEDIA_IN_USE            0x00010104
#define OID_GEN_MAXIMUM_LOOKAHEAD       0x00010105
#define OID_GEN_MAXIMUM_FRAME_SIZE      0x00010106
#define OID_GEN_LINK_SPEED              0x00010107
#define OID_GEN_TRANSMIT_BUFFER_SPACE   0x00010108
#define OID_GEN_RECEIVE_BUFFER_SPACE    0x00010109
#define OID_GEN_TRANSMIT_BLOCK_SIZE     0x0001010A
#define OID_GEN_RECEIVE_BLOCK_SIZE      0x0001010B
#define OID_GEN_VENDOR_ID               0x0001010C
#define OID_GEN_VENDOR_DESCRIPTION      0x0001010D
#define OID_GEN_CURRENT_PACKET_FILTER   0x0001010E
#define OID_GEN_CURRENT_LOOKAHEAD       0x0001010F
#define OID_GEN_DRIVER_VERSION          0x00010110
#define OID_GEN_MAXIMUM_TOTAL_SIZE      0x00010111
#define OID_GEN_MAC_OPTIONS             0x00010113
#define OID_802_3_PERMANENT_ADDRESS     0x01010101
#define OID_802_3_CURRENT_ADDRESS       0x01010102

/* ── NDIS_TIMER — NDIS timer wrapper ── */
#ifndef _NDIS_TIMER_DEFINED
#define _NDIS_TIMER_DEFINED
typedef struct _NDIS_TIMER {
    KTIMER  Timer;
    KDPC    Dpc;
} NDIS_TIMER, *PNDIS_TIMER;
#endif

/* ── NDIS_INTERRUPT — NDIS interrupt object ── */
#ifndef _NDIS_INTERRUPT_DEFINED
#define _NDIS_INTERRUPT_DEFINED
typedef struct _NDIS_INTERRUPT {
    PVOID       InterruptObject;  /* PKINTERRUPT */
    KSPIN_LOCK  DpcCountLock;
    PVOID       MiniportIdField;
    PVOID       DpcRoutine;
    PVOID       SharedInterruptRoutine;
    PVOID       IsrRoutine;
    PVOID       InterruptContext;
    UCHAR       DpcCount;
    BOOLEAN     Removing;
} NDIS_INTERRUPT, *PNDIS_INTERRUPT;
typedef NDIS_INTERRUPT NDIS_MINIPORT_INTERRUPT, *PNDIS_MINIPORT_INTERRUPT;
#endif

/* ── NDIS_REQUEST_TYPE — request type enum ── */
#ifndef _NDIS_REQUEST_TYPE_DEFINED
#define _NDIS_REQUEST_TYPE_DEFINED
typedef enum _NDIS_REQUEST_TYPE {
    NdisRequestQueryInformation,
    NdisRequestSetInformation,
    NdisRequestQueryStatistics,
    NdisRequestOpen,
    NdisRequestClose,
    NdisRequestSend,
    NdisRequestTransferData,
    NdisRequestReset,
    NdisRequestGeneric1,
    NdisRequestGeneric2,
    NdisRequestGeneric3,
    NdisRequestGeneric4
} NDIS_REQUEST_TYPE, *PNDIS_REQUEST_TYPE;
#endif

/* ── NDIS_REQUEST — NDIS request structure ── */
#ifndef _NDIS_REQUEST_DEFINED
#define _NDIS_REQUEST_DEFINED
typedef struct _NDIS_REQUEST {
    UCHAR               MacReserved[16];
    NDIS_REQUEST_TYPE   RequestType;
    union {
        struct {
            NDIS_OID    Oid;
            PVOID       InformationBuffer;
            UINT        InformationBufferLength;
            UINT        BytesWritten;
            UINT        BytesNeeded;
        } QUERY_INFORMATION;
        struct {
            NDIS_OID    Oid;
            PVOID       InformationBuffer;
            UINT        InformationBufferLength;
            UINT        BytesRead;
            UINT        BytesNeeded;
        } SET_INFORMATION;
    } DATA;
} NDIS_REQUEST, *PNDIS_REQUEST;
#endif

/* ── NDIS_PACKET_PRIVATE — private packet data ── */
#ifndef _NDIS_PACKET_PRIVATE_DEFINED
#define _NDIS_PACKET_PRIVATE_DEFINED
typedef struct _NDIS_PACKET_PRIVATE {
    UINT        PhysicalCount;
    UINT        TotalLength;
    PNDIS_BUFFER Head;
    PNDIS_BUFFER Tail;
    PVOID       Pool;
    UINT        Count;
    ULONG       Flags;
    BOOLEAN     ValidCounts;
    UCHAR       NdisPacketFlags;
    USHORT      NdisPacketOobOffset;
} NDIS_PACKET_PRIVATE, *PNDIS_PACKET_PRIVATE;
#endif

/* ── NDIS_CONFIGURATION_PARAMETER — registry config parameter ── */
#ifndef _NDIS_CONFIGURATION_PARAMETER_DEFINED
#define _NDIS_CONFIGURATION_PARAMETER_DEFINED
typedef enum _NDIS_PARAMETER_TYPE {
    NdisParameterInteger,
    NdisParameterHexInteger,
    NdisParameterString,
    NdisParameterMultiString,
    NdisParameterBinary
} NDIS_PARAMETER_TYPE, *PNDIS_PARAMETER_TYPE;

typedef struct _NDIS_CONFIGURATION_PARAMETER {
    NDIS_PARAMETER_TYPE ParameterType;
    union {
        ULONG           IntegerData;
        NDIS_STRING     StringData;
        PVOID           BinaryData;
    } ParameterData;
} NDIS_CONFIGURATION_PARAMETER, *PNDIS_CONFIGURATION_PARAMETER;
#endif

/* ── NDIS_MCA_POS_DATA — MCA POS data ── */
#ifndef _NDIS_MCA_POS_DATA_DEFINED
#define _NDIS_MCA_POS_DATA_DEFINED
typedef struct _NDIS_MCA_POS_DATA {
    USHORT  AdapterId;
    UCHAR   PosData1;
    UCHAR   PosData2;
    UCHAR   PosData3;
    UCHAR   PosData4;
} NDIS_MCA_POS_DATA, *PNDIS_MCA_POS_DATA;
#endif

/* ── NDIS_MAC_CHARACTERISTICS — MAC driver characteristics ── */
#ifndef _NDIS_MAC_CHARACTERISTICS_DEFINED
#define _NDIS_MAC_CHARACTERISTICS_DEFINED
typedef struct _NDIS_MAC_CHARACTERISTICS {
    UCHAR       MajorNdisVersion;
    UCHAR       MinorNdisVersion;
    UINT        Reserved;
    PVOID       OpenAdapterHandler;
    PVOID       CloseAdapterHandler;
    PVOID       SendHandler;
    PVOID       TransferDataHandler;
    PVOID       ResetHandler;
    PVOID       RequestHandler;
    PVOID       QueryGlobalStatisticsHandler;
    PVOID       UnloadMacHandler;
    PVOID       AddAdapterHandler;
    PVOID       RemoveAdapterHandler;
    NDIS_STRING Name;
} NDIS_MAC_CHARACTERISTICS, *PNDIS_MAC_CHARACTERISTICS;
#endif

#ifdef __cplusplus
}
#endif

#endif /* _NDIS_H_ */
