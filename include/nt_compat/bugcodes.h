/*++
 * bugcodes.h — NT Bugcheck Codes (Cross-Platform Compatibility Shim)
 *--*/
#ifndef _BUGCODES_H_COMPAT_
#define _BUGCODES_H_COMPAT_

#define APC_INDEX_MISMATCH              0x00000001
#define DEVICE_QUEUE_NOT_BUSY           0x00000002
#define INVALID_AFFINITY_SET            0x00000003
#define INVALID_DATA_ACCESS_TRAP        0x00000004
#define INVALID_PROCESS_ATTACH_ATTEMPT  0x00000005
#define INVALID_PROCESS_DETACH_ATTEMPT  0x00000006
#define INVALID_SOFTWARE_INTERRUPT      0x00000007
#define IRQL_NOT_DISPATCH_LEVEL         0x00000008
#define IRQL_NOT_GREATER_OR_EQUAL       0x00000009
#define IRQL_NOT_LESS_OR_EQUAL          0x0000000A
#define NO_EXCEPTION_HANDLING_SUPPORT   0x0000000B
#define MAXIMUM_WAIT_OBJECTS_EXCEEDED   0x0000000C
#define MUTEX_LEVEL_NUMBER_VIOLATION    0x0000000D
#define NO_USER_MODE_CONTEXT            0x0000000E
#define SPIN_LOCK_ALREADY_OWNED         0x0000000F
#define SPIN_LOCK_NOT_OWNED             0x00000010
#define THREAD_NOT_MUTEX_OWNER          0x00000011
#define TRAP_CAUSE_UNKNOWN              0x00000012
#define KMODE_EXCEPTION_NOT_HANDLED     0x0000001E
#define KERNEL_MODE_EXCEPTION_NOT_HANDLED 0x0000008E
#define PAGE_FAULT_IN_NONPAGED_AREA     0x00000050
#define KERNEL_STACK_INPAGE_ERROR       0x00000077
#define KERNEL_DATA_INPAGE_ERROR        0x0000007A
#define INACCESSIBLE_BOOT_DEVICE        0x0000007B
#define UNEXPECTED_KERNEL_MODE_TRAP     0x0000007F
#define DRIVER_IRQL_NOT_LESS_OR_EQUAL   0x000000D1

/* Stub for KeBugCheck / KeBugCheckEx */
#include <stdio.h>
#include <stdlib.h>

static inline void KeBugCheck(unsigned long BugCheckCode) {
    fprintf(stderr, "*** STOP: 0x%08lX\n", BugCheckCode);
    /* In AGI-OS context, log to cognitive telemetry instead of halting */
}

static inline void KeBugCheckEx(
    unsigned long BugCheckCode,
    unsigned long BugCheckParameter1,
    unsigned long BugCheckParameter2,
    unsigned long BugCheckParameter3,
    unsigned long BugCheckParameter4)
{
    fprintf(stderr, "*** STOP: 0x%08lX (0x%08lX, 0x%08lX, 0x%08lX, 0x%08lX)\n",
            BugCheckCode, BugCheckParameter1, BugCheckParameter2,
            BugCheckParameter3, BugCheckParameter4);
}

#endif /* _BUGCODES_H_COMPAT_ */
