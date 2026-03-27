/*
 * minwinbase.h — Minimal Windows Base Definitions Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Prevents the Windows SDK minwinbase.h from being included (375 errors).
 */
#ifndef _NT_COMPAT_MINWINBASE_H_
#define _NT_COMPAT_MINWINBASE_H_

/* Prevent the Windows SDK minwinbase.h from being included */
#ifndef _MINWINBASE_
#define _MINWINBASE_
#endif

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* OVERLAPPED — used in async I/O */
#ifndef _OVERLAPPED_DEFINED
#define _OVERLAPPED_DEFINED
typedef struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union {
        struct {
            DWORD Offset;
            DWORD OffsetHigh;
        };
        PVOID Pointer;
    };
    HANDLE hEvent;
} OVERLAPPED, *LPOVERLAPPED;
#endif

/* SECURITY_ATTRIBUTES */
#ifndef _SECURITY_ATTRIBUTES_DEFINED
#define _SECURITY_ATTRIBUTES_DEFINED
typedef struct _SECURITY_ATTRIBUTES {
    DWORD  nLength;
    PVOID  lpSecurityDescriptor;
    BOOLEAN bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;
#endif

/* FILETIME */
#ifndef _FILETIME_DEFINED
#define _FILETIME_DEFINED
typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
#endif

/* SYSTEMTIME */
#ifndef _SYSTEMTIME_DEFINED
#define _SYSTEMTIME_DEFINED
typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;
#endif

#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_MINWINBASE_H_ */
