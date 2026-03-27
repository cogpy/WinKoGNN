/* kdp.h — Kernel Debugger Private header stub */
#ifndef _KDP_H_COMPAT_
#define _KDP_H_COMPAT_
#include "ntdef.h"
#include "nt.h"
typedef BOOLEAN (*PKDEBUG_ROUTINE)(PVOID TrapFrame, PVOID ExceptionFrame, PVOID ExceptionRecord, PVOID Context, PVOID SecondChance);
typedef struct _BREAKPOINT_ENTRY { PVOID Address; ULONG Content; ULONG Flags; } BREAKPOINT_ENTRY;
#define BREAKPOINT_TABLE_SIZE 32
#define KDP_PACKET_RECEIVED 0
#define KDP_PACKET_TIMEOUT 1
#define KDP_PACKET_RESEND 2
VOID NTAPI KdpReboot(VOID);
#endif
