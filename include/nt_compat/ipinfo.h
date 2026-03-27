/*
 * ipinfo.h — IP Information Structures Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 */
#ifndef _NT_COMPAT_IPINFO_H_
#define _NT_COMPAT_IPINFO_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* IP address type */
typedef ULONG IPAddr;
typedef ULONG IPMask;

/* IP_OPT_HEADER */
typedef struct _IP_OPT_HEADER {
    UCHAR OptionType;
    UCHAR OptionLength;
} IP_OPT_HEADER, *PIP_OPT_HEADER;

#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_IPINFO_H_ */
