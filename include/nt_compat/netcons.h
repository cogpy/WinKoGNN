/*
 * netcons.h — Network Constants Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 */
#ifndef _NT_COMPAT_NETCONS_H_
#define _NT_COMPAT_NETCONS_H_

/* Prevent the Windows SDK netcons.h from being included */
#ifndef _NETCONS_
#define _NETCONS_
#endif

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* LAN Manager path/name length constants */
#ifndef PATHLEN
#define PATHLEN     256
#endif
#ifndef UNCLEN
#define UNCLEN      (PATHLEN + 2)
#endif
#ifndef CNLEN
#define CNLEN       15
#endif
#ifndef UNLEN
#define UNLEN       256
#endif
#ifndef DNLEN
#define DNLEN       15
#endif

#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NETCONS_H_ */
