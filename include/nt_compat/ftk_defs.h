/*++ BUILD Version: 0001
 * ftk_defs.h — FDDI/Token Ring Kit Definitions Compatibility Shim
 *--*/
#ifndef _FTK_DEFS_H_
#define _FTK_DEFS_H_

#include "ndis.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── FTK types ── */
typedef UCHAR   BYTE;
typedef USHORT  WORD;
typedef ULONG   DWORD;

#ifndef LOBYTE
#define LOBYTE(w)   ((BYTE)((DWORD)(w) & 0xFF))
#define HIBYTE(w)   ((BYTE)(((DWORD)(w) >> 8) & 0xFF))
#define LOWORD(l)   ((WORD)((DWORD)(l) & 0xFFFF))
#define HIWORD(l)   ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define MAKEWORD(a, b) ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b) ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#endif

/* ── Adapter types ── */
#define ADAPTER_CARD_TYPE_16_4_AT       1
#define ADAPTER_CARD_TYPE_16_4_MC       2
#define ADAPTER_CARD_TYPE_16_4_PC       3
#define ADAPTER_CARD_TYPE_16_4_MAXY     4
#define ADAPTER_CARD_TYPE_SMART_16_4_AT 5
#define ADAPTER_CARD_TYPE_SMART_16_4_PC 6
#define ADAPTER_CARD_TYPE_16_4_EISA     7
#define ADAPTER_CARD_TYPE_16_4_PCI      8

/* ── Ring speeds ── */
#define RING_SPEED_4                4
#define RING_SPEED_16               16

/* ── Frame types ── */
#define FRAME_TYPE_MAC              0x00
#define FRAME_TYPE_LLC              0x40
#define FRAME_TYPE_FIRST_RESERVED   0x80

/* ── SRB commands ── */
#define SRB_OPEN_ADAPTER            0x03
#define SRB_CLOSE_ADAPTER           0x04
#define SRB_SET_MULTICAST_ADDRESS   0x06
#define SRB_SET_BRIDGE_PARMS        0x09
#define SRB_CONFIG_BRIDGE_RAM       0x0C
#define SRB_TRANSMIT_DIR_FRAME      0x0A
#define SRB_READ_LOG                0x08
#define SRB_MODIFY_OPEN_PARMS       0x01

/* ── Return codes ── */
#define SRB_E_SUCCESS               0x00
#define SRB_E_INVALID_COMMAND       0x01
#define SRB_E_ADAPTER_CLOSED        0x03
#define SRB_E_ADAPTER_OPEN          0x04
#define SRB_E_OPEN_FAILED           0x07

#ifdef __cplusplus
}
#endif

#endif /* _FTK_DEFS_H_ */
