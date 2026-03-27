/*++ BUILD Version: 0001
 * oscfg.h — OS Configuration for Network Stack Compatibility Shim
 *
 * Provides OS-specific type mappings used by TDI/NBT/network components.
 *--*/
#ifndef _OSCFG_H_
#define _OSCFG_H_

#include "ntddk.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── TDI types ── */
typedef LONG TDI_STATUS;
typedef PVOID PTDI_CONNECTION_INFORMATION;

#define TDI_SUCCESS                 0x00000000
#define TDI_PENDING                 STATUS_PENDING
#define TDI_BUFFER_OVERFLOW         STATUS_BUFFER_OVERFLOW
#define TDI_NO_RESOURCES            STATUS_INSUFFICIENT_RESOURCES
#define TDI_ADDR_IN_USE             0xC0000154L
#define TDI_NOT_FOUND               0xC0000225L
#define TDI_TIMED_OUT               0xC00000B5L

/* ── Transport address ── */
#define TDI_ADDRESS_TYPE_IP         2
#define TDI_ADDRESS_TYPE_NETBIOS    17
#define TDI_ADDRESS_LENGTH_IP       14
#define TDI_ADDRESS_LENGTH_NETBIOS  20

typedef struct _TDI_ADDRESS_IP {
    USHORT  sin_port;
    ULONG   in_addr;
    UCHAR   sin_zero[8];
} TDI_ADDRESS_IP, *PTDI_ADDRESS_IP;

typedef struct _TDI_ADDRESS_NETBIOS {
    USHORT  NetbiosNameType;
    UCHAR   NetbiosName[16];
} TDI_ADDRESS_NETBIOS, *PTDI_ADDRESS_NETBIOS;

typedef struct _TRANSPORT_ADDRESS {
    LONG    TAAddressCount;
    struct {
        USHORT  AddressLength;
        USHORT  AddressType;
        UCHAR   Address[1];
    } Address[1];
} TRANSPORT_ADDRESS, *PTRANSPORT_ADDRESS;

/* ── Event types ── */
#define TDI_EVENT_CONNECT           0
#define TDI_EVENT_DISCONNECT        1
#define TDI_EVENT_ERROR             2
#define TDI_EVENT_RECEIVE           3
#define TDI_EVENT_RECEIVE_DATAGRAM  4
#define TDI_EVENT_RECEIVE_EXPEDITED 5
#define TDI_EVENT_SEND_POSSIBLE     6

/* ── OS configuration macros ── */
#define CTEInitLock(Lock)       KeInitializeSpinLock((PKSPIN_LOCK)(Lock))
#define CTEGetLock(Lock, Irql)  (*(KIRQL*)(Irql) = 0, (void)(Lock))
#define CTEFreeLock(Lock, Irql) ((void)(Lock), (void)(Irql))

typedef KSPIN_LOCK CTELock;
typedef KIRQL CTELockHandle;

/* ── CTE timer ── */
typedef struct _CTETimer {
    KTIMER  Timer;
    KDPC    Dpc;
} CTETimer;

static inline void CTEInitTimer(CTETimer *Timer) { (void)Timer; }
static inline NTSTATUS CTEStartTimer(CTETimer *Timer, ULONG DueTime,
    PVOID CompletionRoutine, PVOID Context) {
    (void)Timer; (void)DueTime; (void)CompletionRoutine; (void)Context;
    return STATUS_SUCCESS;
}

/* ── CTE event ── */
typedef KEVENT CTEEvent;
#define CTEInitEvent(Event, Type) ((void)(Event), (void)(Type))
#define CTESignal(Event, Status)  ((void)(Event), (void)(Status))
#define CTEBlock(Event)           ((void)(Event), STATUS_SUCCESS)

/* ── CTE memory ── */
#define CTEAllocMem(Size)         ExAllocatePool(NonPagedPool, Size)
#define CTEFreeMem(Ptr)           ExFreePool(Ptr)

/* ── Blocking routines ── */
typedef struct _CTEBlockStruc {
    KEVENT  Event;
    NTSTATUS Status;
} CTEBlockStruc;

static inline void CTEInitBlockStruc(CTEBlockStruc *Block) { (void)Block; }

#ifdef __cplusplus
}
#endif

#endif /* _OSCFG_H_ */
