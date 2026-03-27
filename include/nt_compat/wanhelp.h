/*++ BUILD Version: 0001
 * wanhelp.h — WAN Helper Compatibility Shim
 *--*/
#ifndef _WANHELP_H_
#define _WANHELP_H_

#include "ndis.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── WAN medium subtypes ── */
#define NdisWanMediumHub        0
#define NdisWanMediumX_25       1
#define NdisWanMediumIsdn       2
#define NdisWanMediumSerial     3
#define NdisWanMediumFrameRelay 4
#define NdisWanMediumAtm        5
#define NdisWanMediumSonet      6
#define NdisWanMediumSW56K      7
#define NdisWanMediumPPTP       8
#define NdisWanMediumL2TP       9

/* ── WAN link info ── */
typedef struct _NDIS_WAN_INFO {
    ULONG   MaxFrameSize;
    ULONG   MaxTransmit;
    ULONG   HeaderPadding;
    ULONG   TailPadding;
    ULONG   Endpoints;
    NDIS_MEDIUM MemoryFlags;
    NDIS_PHYSICAL_ADDRESS HighestAcceptableAddress;
    ULONG   FramingBits;
    ULONG   DesiredACCM;
} NDIS_WAN_INFO, *PNDIS_WAN_INFO;

typedef struct _NDIS_WAN_GET_LINK_INFO {
    NDIS_HANDLE NdisLinkHandle;
    ULONG       MaxSendFrameSize;
    ULONG       MaxRecvFrameSize;
    ULONG       SendFramingBits;
    ULONG       RecvFramingBits;
    ULONG       SendCompressionBits;
    ULONG       RecvCompressionBits;
    ULONG       SendACCM;
    ULONG       RecvACCM;
} NDIS_WAN_GET_LINK_INFO, *PNDIS_WAN_GET_LINK_INFO;

#ifdef __cplusplus
}
#endif

#endif /* _WANHELP_H_ */
