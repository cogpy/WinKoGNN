/*++ BUILD Version: 0001
 * dlc.h — Data Link Control Protocol Compatibility Shim
 *--*/
#ifndef _DLC_H_
#define _DLC_H_

#include "ntddk.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── DLC command codes ── */
#define LLC_DIR_OPEN_ADAPTER        0x03
#define LLC_DIR_CLOSE_ADAPTER       0x04
#define LLC_DIR_SET_MULTICAST_ADDRESS 0x06
#define LLC_DIR_OPEN_DIRECT         0x08
#define LLC_DIR_CLOSE_DIRECT        0x09
#define LLC_DLC_OPEN_SAP            0x15
#define LLC_DLC_CLOSE_SAP           0x16
#define LLC_DLC_OPEN_STATION        0x19
#define LLC_DLC_CLOSE_STATION       0x1A
#define LLC_DLC_CONNECT_STATION     0x1B
#define LLC_DLC_FLOW_CONTROL        0x1D
#define LLC_TRANSMIT_FRAMES         0x0A
#define LLC_TRANSMIT_DIR_FRAME      0x0E
#define LLC_RECEIVE                 0x21
#define LLC_RECEIVE_CANCEL          0x22
#define LLC_BUFFER_GET              0x26
#define LLC_BUFFER_FREE             0x27
#define LLC_READ                    0x31
#define LLC_READ_CANCEL             0x32

/* ── DLC return codes ── */
#define LLC_STATUS_SUCCESS              0x0000
#define LLC_STATUS_INVALID_COMMAND      0x0001
#define LLC_STATUS_DUPLICATE_COMMAND    0x0002
#define LLC_STATUS_ADAPTER_OPEN         0x0003
#define LLC_STATUS_ADAPTER_CLOSED       0x0004
#define LLC_STATUS_PARAMETER_MISSING    0x0005
#define LLC_STATUS_INVALID_OPTION       0x0006
#define LLC_STATUS_COMMAND_CANCELLED    0x0007
#define LLC_STATUS_ACCESS_DENIED        0x0008
#define LLC_STATUS_ADAPTER_NOT_INITIALIZED 0x0009
#define LLC_STATUS_CANCELLED_BY_USER    0x000A
#define LLC_STATUS_RESOURCES            0x000C

/* ── SAP types ── */
#define LLC_INDIVIDUAL_SAP          0x04
#define LLC_GROUP_SAP               0x08
#define LLC_MEMBER_OF_GROUP_SAP     0x10

/* ── CCB (Command Control Block) ── */
typedef struct _LLC_CCB {
    UCHAR       uchAdapterNumber;
    UCHAR       uchDlcCommand;
    UCHAR       uchDlcStatus;
    UCHAR       uchReserved1;
    struct _LLC_CCB *pNext;
    ULONG       ulCompletionFlag;
    PVOID       pCcbParms;
    PVOID       hCompletionEvent;
    UCHAR       uchReserved2;
    UCHAR       uchReadFlag;
    USHORT      usReserved3;
} LLC_CCB, *PLLC_CCB;

#ifdef __cplusplus
}
#endif

#endif /* _DLC_H_ */
