/*++ BUILD Version: 0001
 * types.h — Generic Types Compatibility Shim
 *--*/
#ifndef _TYPES_H_
#define _TYPES_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── BSD-style types used by some NT network code ── */
#ifndef _SYS_TYPES_COMPAT
#define _SYS_TYPES_COMPAT

typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;

typedef char *          caddr_t;
typedef long            daddr_t;
typedef long            off_t;
typedef long            key_t;
typedef unsigned int    size_t_compat;

#endif /* _SYS_TYPES_COMPAT */

#ifdef __cplusplus
}
#endif

#endif /* _TYPES_H_ */
