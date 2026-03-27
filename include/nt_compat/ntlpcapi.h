/*++ BUILD Version: 0001
 * ntlpcapi.h — NT Local Procedure Call API Compatibility Shim
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * In NetCog, LPC is replaced by 9P2000.L (via diod/libnpfs).
 * This header provides the NT LPC types with a bridge to 9P.
 *
 * NT4 LPC → 9P mapping:
 *   NtCreatePort        → 9P server listen (Np9srv)
 *   NtConnectPort       → 9P Tversion + Tattach
 *   NtRequestPort       → 9P Twrite
 *   NtReplyPort         → 9P Rwrite
 *   NtRequestWaitReply  → 9P Twrite + Rread (synchronous)
 *   NtListenPort        → 9P accept connection
 *   NtAcceptConnectPort → 9P Rattach
 *   NtCompleteConnectPort → 9P connection established
 *--*/
#ifndef _NT_COMPAT_NTLPCAPI_H_
#define _NT_COMPAT_NTLPCAPI_H_

#include "ntdef.h"
#include "p9.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */
/* LPC Port Message — the fundamental IPC unit                         */
/* In NetCog, this maps to an Np9fcall (9P protocol message)           */
/* ------------------------------------------------------------------ */
#ifndef _PORT_MESSAGE_DEFINED
#define _PORT_MESSAGE_DEFINED
typedef struct _PORT_MESSAGE {
    union {
        struct {
            CSHORT DataLength;
            CSHORT TotalLength;
        } s1;
        ULONG Length;
    } u1;
    union {
        struct {
            CSHORT Type;
            CSHORT DataInfoOffset;
        } s2;
        ULONG ZeroInit;
    } u2;
    union {
        PVOID ClientId;     /* CLIENT_ID */
        double DoNotUseThisField;
    };
    ULONG MessageId;
    union {
        ULONG_PTR ClientViewSize;
        ULONG CallbackId;
    };
} PORT_MESSAGE, *PPORT_MESSAGE;
#endif

/* ------------------------------------------------------------------ */
/* LPC Port View — shared memory mapping for large messages            */
/* In NetCog, maps to 9P msize negotiation                             */
/* ------------------------------------------------------------------ */
#ifndef _PORT_VIEW_DEFINED
#define _PORT_VIEW_DEFINED
typedef struct _PORT_VIEW {
    ULONG       Length;
    PVOID       SectionHandle;
    ULONG       SectionOffset;
    ULONG       ViewSize;
    PVOID       ViewBase;
    PVOID       ViewRemoteBase;
} PORT_VIEW, *PPORT_VIEW;

typedef struct _REMOTE_PORT_VIEW {
    ULONG       Length;
    ULONG       ViewSize;
    PVOID       ViewBase;
} REMOTE_PORT_VIEW, *PREMOTE_PORT_VIEW;
#endif

/* ------------------------------------------------------------------ */
/* LPC message types                                                   */
/* ------------------------------------------------------------------ */
#define LPC_REQUEST             1
#define LPC_REPLY               2
#define LPC_DATAGRAM            3
#define LPC_LOST_REPLY          4
#define LPC_PORT_CLOSED         5
#define LPC_CLIENT_DIED         6
#define LPC_EXCEPTION           7
#define LPC_DEBUG_EVENT         8
#define LPC_ERROR_EVENT         9
#define LPC_CONNECTION_REQUEST  10

/* Maximum LPC message data size */
#ifndef PORT_MAXIMUM_MESSAGE_LENGTH
#define PORT_MAXIMUM_MESSAGE_LENGTH 256
#endif

/* ------------------------------------------------------------------ */
/* LPC API declarations — in NetCog these are implemented via 9P        */
/* The actual function prototypes are in nt.h; this header only         */
/* provides the PORT_MESSAGE and PORT_VIEW types needed by callers.     */
/* ------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTLPCAPI_H_ */
