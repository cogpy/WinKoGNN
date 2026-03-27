/* Kernel Internal — private kernel scheduler/dispatcher types */
#ifndef _KI_H_COMPAT
#define _KI_H_COMPAT
#include "ntdef.h"
VOID NTAPI KiDispatchInterrupt(VOID);
VOID NTAPI KiQuantumEnd(VOID);
VOID NTAPI KiIdleLoop(VOID);
VOID NTAPI KiDeliverApc(IN KPROCESSOR_MODE PreviousMode, IN PVOID ExceptionFrame, IN PVOID TrapFrame);
VOID NTAPI KiUnexpectedInterrupt(VOID);
VOID NTAPI KiDispatchException(IN PVOID ExceptionRecord, IN PVOID ExceptionFrame, IN PVOID TrapFrame, IN KPROCESSOR_MODE PreviousMode, IN BOOLEAN FirstChance);
#endif
