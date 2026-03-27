/* HAL Private — internal to HAL subsystem */
#ifndef _HALP_H_COMPAT
#define _HALP_H_COMPAT
#include "nthal.h"
#include "ntddk.h"
/* HAL private types — these are normally defined in the HAL source tree */
typedef VOID (*PHAL_RESET_DISPLAY_PARAMETERS)(IN ULONG Columns, IN ULONG Rows);
typedef BOOLEAN (*PHAL_PRIVATE_DISPATCH_ROUTINE)(VOID);
VOID NTAPI HalpInitializeClock(VOID);
VOID NTAPI HalpInitializeStallExecution(IN ULONG ProcessorNumber);
BOOLEAN NTAPI HalpBusError(IN PVOID NMIInfo, IN PVOID ReservedInfo);
#endif
