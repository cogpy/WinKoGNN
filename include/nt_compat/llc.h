/*++ BUILD Version: 0001
 * llc.h — IEEE 802.2 Logical Link Control Compatibility Shim
 *--*/
#ifndef _LLC_H_
#define _LLC_H_

#include "dlc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── LLC frame types ── */
#define LLC_I_FRAME                 0x00
#define LLC_S_FRAME                 0x01
#define LLC_U_FRAME                 0x03

/* ── LLC U-frame commands ── */
#define LLC_UI                      0x03
#define LLC_SABME                   0x6F
#define LLC_DISC                    0x43
#define LLC_UA                      0x63
#define LLC_DM                      0x0F
#define LLC_FRMR                    0x87
#define LLC_XID                     0xAF
#define LLC_TEST                    0xE3

/* ── LLC S-frame types ── */
#define LLC_RR                      0x01
#define LLC_RNR                     0x05
#define LLC_REJ                     0x09

/* ── SAP values ── */
#define LLC_SAP_NULL                0x00
#define LLC_SAP_SNAP                0xAA
#define LLC_SAP_NETBIOS             0xF0
#define LLC_SAP_SNA                 0x04
#define LLC_SAP_GLOBAL              0xFF

/* ── LLC header ── */
typedef struct _LLC_HEADER {
    UCHAR   dsap;
    UCHAR   ssap;
    union {
        USHORT  RawU16;
        struct {
            UCHAR   control;
        } UFrame;
        struct {
            UCHAR   control;
            UCHAR   nr_ns;
        } IFrame;
    };
} LLC_HEADER, *PLLC_HEADER;

/* ── LLC object ── */
typedef struct _LLC_OBJECT {
    LIST_ENTRY      Linkage;
    UCHAR           Sap;
    UCHAR           State;
    USHORT          StationId;
    ULONG           RefCount;
    PVOID           pAdapterContext;
} LLC_OBJECT, *PLLC_OBJECT;

#ifdef __cplusplus
}
#endif

#endif /* _LLC_H_ */
