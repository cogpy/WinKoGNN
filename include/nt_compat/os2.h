/*++ BUILD Version: 0001
 * os2.h — OS/2 Subsystem Compatibility Shim
 *--*/
#ifndef _OS2_H_
#define _OS2_H_

#include "ntddk.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── OS/2 basic types ── */
typedef USHORT  SHANDLE;
typedef PVOID   LHANDLE;
typedef ULONG   APIRET;
typedef ULONG   HFILE;
#ifndef _HMODULE_DEFINED
typedef ULONG   HMODULE;
#define _HMODULE_DEFINED
#endif
typedef ULONG   PID;
typedef ULONG   TID;

/* ── OS/2 return codes ── */
#define NO_ERROR_OS2                0
#define ERROR_FILE_NOT_FOUND_OS2    2
#define ERROR_PATH_NOT_FOUND_OS2    3
#define ERROR_ACCESS_DENIED_OS2     5
#define ERROR_NOT_ENOUGH_MEMORY_OS2 8
#define ERROR_INVALID_PARAMETER_OS2 87

/* ── OS/2 file open modes ── */
#define OPEN_ACCESS_READONLY        0x0000
#define OPEN_ACCESS_WRITEONLY       0x0001
#define OPEN_ACCESS_READWRITE       0x0002
#define OPEN_SHARE_DENYREADWRITE    0x0010
#define OPEN_SHARE_DENYWRITE        0x0020
#define OPEN_SHARE_DENYREAD         0x0030
#define OPEN_SHARE_DENYNONE         0x0040

#ifdef __cplusplus
}
#endif

#endif /* _OS2_H_ */
