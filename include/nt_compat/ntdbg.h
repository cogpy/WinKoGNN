/* NT Debug — kernel debugger interface */
#ifndef _NTDBG_H_COMPAT
#define _NTDBG_H_COMPAT
#include "ntdef.h"
#ifndef DBG_CONTINUE
#define DBG_CONTINUE                0x00010002L
#endif
#ifndef DBG_EXCEPTION_NOT_HANDLED
#define DBG_EXCEPTION_NOT_HANDLED   0x80010001L
#endif
#ifndef DBG_TERMINATE_THREAD
#define DBG_TERMINATE_THREAD        0x40010003L
#endif
#ifndef DBG_TERMINATE_PROCESS
#define DBG_TERMINATE_PROCESS       0x40010004L
#endif
NTSTATUS NTAPI DbgUiConnectToDbg(VOID);
NTSTATUS NTAPI DbgUiWaitStateChange(OUT PVOID StateChange, IN PLARGE_INTEGER Timeout OPTIONAL);
NTSTATUS NTAPI DbgUiContinue(IN PCLIENT_ID AppClientId, IN NTSTATUS ContinueStatus);
#endif
