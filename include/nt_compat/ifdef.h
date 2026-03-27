/*
 * ifdef.h — Interface Definition Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Prevents the Windows SDK ifdef.h from being included (405 errors).
 * The SDK ifdef.h defines NET_IF_* types that conflict with NT4 types.
 */
#ifndef _NT_COMPAT_IFDEF_H_
#define _NT_COMPAT_IFDEF_H_

/* Prevent the Windows SDK ifdef.h from being included */
#ifndef _IFDEF_
#define _IFDEF_
#endif

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NET_LUID — Network interface LUID (minimal stub) */
#ifndef _NET_LUID_DEFINED
#define _NET_LUID_DEFINED
typedef union _NET_LUID {
    ULONGLONG Value;
    struct {
        ULONGLONG Reserved : 24;
        ULONGLONG NetLuidIndex : 24;
        ULONGLONG IfType : 16;
    } Info;
} NET_LUID, *PNET_LUID;
#endif

/* IF_INDEX */
typedef ULONG NET_IFINDEX;
typedef NET_IFINDEX IF_INDEX;

#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_IFDEF_H_ */
