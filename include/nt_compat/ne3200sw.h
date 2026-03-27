/*++ BUILD Version: 0001
 * ne3200sw.h — NE3200 EISA Network Adapter Compatibility Shim
 *--*/
#ifndef _NE3200SW_H_
#define _NE3200SW_H_

#include "ndis.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── NE3200 adapter constants ── */
#define NE3200_MAXIMUM_BLOCKS_PER_PACKET    6
#define NE3200_MAXIMUM_PACKET_SIZE          1514
#define NE3200_HEADER_SIZE                  14
#define NE3200_MAXIMUM_MULTICAST            16
#define NE3200_LENGTH_OF_ADDRESS            6

/* ── NE3200 command codes ── */
#define NE3200_COMMAND_SET_STATION_ADDRESS   0x01
#define NE3200_COMMAND_SET_MULTICAST_ADDRESS 0x02
#define NE3200_COMMAND_CLEAR_STATISTICS      0x04
#define NE3200_COMMAND_CONFIGURE_82586       0x05
#define NE3200_COMMAND_SET_FILTER            0x06

/* ── NE3200 status codes ── */
#define NE3200_STATUS_COMPLETE              0x0001
#define NE3200_STATUS_ERROR                 0x0002
#define NE3200_STATUS_ABORT                 0x0004

/* ── NE3200 command block ── */
typedef struct _NE3200_COMMAND_BLOCK {
    USHORT  Status;
    USHORT  Command;
    ULONG   NextPending;
    UCHAR   Data[64];
} NE3200_COMMAND_BLOCK, *PNE3200_COMMAND_BLOCK;

#ifdef __cplusplus
}
#endif

#endif /* _NE3200SW_H_ */
