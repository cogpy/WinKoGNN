/*++ BUILD Version: 0001
 * crt/excpt.h — CRT Exception Handling Compatibility Shim
 *--*/
#ifndef _CRT_EXCPT_H_
#define _CRT_EXCPT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ── Exception disposition ── */
#ifndef _EXCEPTION_DISPOSITION_DEFINED
#define _EXCEPTION_DISPOSITION_DEFINED
typedef enum _EXCEPTION_DISPOSITION {
    ExceptionContinueExecution = 0,
    ExceptionContinueSearch = 1,
    ExceptionNestedException = 2,
    ExceptionCollidedUnwind = 3
} EXCEPTION_DISPOSITION;
#endif

/* ── SEH filter return values ── */
#define EXCEPTION_EXECUTE_HANDLER       1
#define EXCEPTION_CONTINUE_SEARCH       0
#define EXCEPTION_CONTINUE_EXECUTION    (-1)

/* ── Exception record ── */
/* EXCEPTION_RECORD defined in ntrtl.h — just ensure the guard is set */
#ifndef _EXCEPTION_RECORD_DEFINED
#define _EXCEPTION_RECORD_DEFINED
typedef struct _EXCEPTION_RECORD {
    unsigned long   ExceptionCode;
    unsigned long   ExceptionFlags;
    struct _EXCEPTION_RECORD *ExceptionRecord;
    void           *ExceptionAddress;
    unsigned long   NumberParameters;
    unsigned long   ExceptionInformation[15];
} EXCEPTION_RECORD, *PEXCEPTION_RECORD;
#endif

/* ── Exception codes ── */
#define EXCEPTION_ACCESS_VIOLATION      0xC0000005L
#define EXCEPTION_ARRAY_BOUNDS_EXCEEDED 0xC000008CL
#define EXCEPTION_BREAKPOINT            0x80000003L
#define EXCEPTION_DATATYPE_MISALIGNMENT 0x80000002L
#define EXCEPTION_FLT_DENORMAL_OPERAND  0xC000008DL
#define EXCEPTION_FLT_DIVIDE_BY_ZERO    0xC000008EL
#define EXCEPTION_FLT_OVERFLOW          0xC0000091L
#define EXCEPTION_FLT_UNDERFLOW         0xC0000093L
#define EXCEPTION_INT_DIVIDE_BY_ZERO    0xC0000094L
#define EXCEPTION_INT_OVERFLOW          0xC0000095L
#define EXCEPTION_STACK_OVERFLOW        0xC00000FDL

/* ── SEH macros (no-op on non-MSVC) ── */
#ifndef _MSC_VER
#define __try       if(1)
#define __except(x) if(0)
#define __finally   if(1)
#define __leave     break
#define GetExceptionCode()          0
#define GetExceptionInformation()   ((PEXCEPTION_RECORD)0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _CRT_EXCPT_H_ */
