/*
 * tdistat.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 43 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_TDISTAT_H_H_
#define _NT_COMPAT_TDISTAT_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif


/* TDI Status codes for network transport drivers */
#ifndef _TDISTAT_H_COMPAT
#define _TDISTAT_H_COMPAT
#define TDI_SUCCESS             0x00000000L
#define TDI_NO_RESOURCES        0xC000009AL
#define TDI_ADDR_IN_USE         0xC0000154L
#define TDI_BAD_ADDR            0xC0000155L
#define TDI_NO_FREE_ADDR        0xC0000156L
#define TDI_ADDR_INVALID        0xC0000157L
#define TDI_ADDR_DELETED        0xC0000158L
#define TDI_BUFFER_OVERFLOW     0x80000005L
#define TDI_BAD_EVENT_TYPE      0xC00000BBL
#define TDI_BAD_OPTION          0xC0000159L
#define TDI_CONN_REFUSED        0xC0000236L
#define TDI_CONN_ABORTED        0xC000023FL
#define TDI_CONN_RESET          0xC000020DL
#define TDI_TIMED_OUT           0xC00000B5L
#define TDI_GRACEFUL_DISC       0xC0000237L
#define TDI_NOT_ACCEPTED        0xC0000238L
#define TDI_MORE_PROCESSING     0xC0000016L
#define TDI_INVALID_STATE       0xC0000184L
#define TDI_INVALID_PARAMETER   0xC000000DL
#define TDI_DEST_NET_UNREACH    0xC000023CL
#define TDI_DEST_HOST_UNREACH   0xC000023DL
#define TDI_DEST_UNREACHABLE    0xC000023EL
#define TDI_PENDING             STATUS_PENDING
#endif /* _TDISTAT_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_TDISTAT_H_H_ */
