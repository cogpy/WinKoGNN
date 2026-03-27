/*
 * lmaccess.h — LAN Manager Access API Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Prevents the Windows SDK lmaccess.h from being included (6.5k+ errors
 * due to LPDWORD and other Win32 user-mode types being undefined in
 * kernel compilation context).
 */
#ifndef _NT_COMPAT_LMACCESS_H_
#define _NT_COMPAT_LMACCESS_H_

/* Prevent the Windows SDK lmaccess.h from being included */
#ifndef _LMACCESS_
#define _LMACCESS_
#endif

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Minimal LAN Manager type stubs for NT4 srv subsystem */
typedef DWORD NET_API_STATUS;

#ifndef NERR_Success
#define NERR_Success 0
#endif

/* User info level 0 — minimal stub */
#ifndef _USER_INFO_0_DEFINED
#define _USER_INFO_0_DEFINED
typedef struct _USER_INFO_0 {
    LPWSTR usri0_name;
} USER_INFO_0, *PUSER_INFO_0, *LPUSER_INFO_0;
#endif

#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_LMACCESS_H_ */
