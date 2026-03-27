/*++ BUILD Version: 0001
 * irda.h — Infrared Data Association Compatibility Shim
 *--*/
#ifndef _IRDA_H_
#define _IRDA_H_

#include "ndis.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── IrDA address family ── */
#define AF_IRDA                     26

/* ── IrDA service types ── */
#define IRDA_SERVICE_IAS            0x01
#define IRDA_SERVICE_IRLMP          0x02
#define IRDA_SERVICE_IRLPT          0x04
#define IRDA_SERVICE_IRCOMM         0x08
#define IRDA_SERVICE_IRTRANP        0x10
#define IRDA_SERVICE_IROBEX         0x20

/* ── IrLAP speeds ── */
#define IRLAP_SPEED_2400            0x0001
#define IRLAP_SPEED_9600            0x0002
#define IRLAP_SPEED_19200           0x0004
#define IRLAP_SPEED_38400           0x0008
#define IRLAP_SPEED_57600           0x0010
#define IRLAP_SPEED_115200          0x0020
#define IRLAP_SPEED_576000          0x0040
#define IRLAP_SPEED_1152000         0x0080
#define IRLAP_SPEED_4000000         0x0100

/* ── IrDA device info ── */
typedef struct _IRDA_DEVICE_INFO {
    UCHAR   irdaDeviceID[4];
    CHAR    irdaDeviceName[22];
    UCHAR   irdaDeviceHints1;
    UCHAR   irdaDeviceHints2;
    UCHAR   irdaCharSet;
} IRDA_DEVICE_INFO, *PIRDA_DEVICE_INFO;

/* ── IrDA message ── */
typedef struct _IRDA_MSG {
    ULONG   Prim;
    union {
        struct { ULONG DevAddr; } IRLAP_DISCOVERY;
        struct { PVOID pQos; } IRLAP_CONNECT;
        struct { PVOID pData; ULONG DataLen; } IRDA_DATA;
    };
} IRDA_MSG, *PIRDA_MSG;

/* ── IrDA primitives ── */
#define IRLAP_DISCOVERY_REQ         0x0001
#define IRLAP_DISCOVERY_CONF        0x0002
#define IRLAP_CONNECT_REQ           0x0003
#define IRLAP_CONNECT_IND           0x0004
#define IRLAP_CONNECT_RESP          0x0005
#define IRLAP_CONNECT_CONF          0x0006
#define IRLAP_DISCONNECT_REQ        0x0007
#define IRLAP_DISCONNECT_IND        0x0008
#define IRLAP_DATA_REQ              0x0009
#define IRLAP_DATA_IND              0x000A
#define IRLAP_DATA_CONF             0x000B

#ifdef __cplusplus
}
#endif

#endif /* _IRDA_H_ */
