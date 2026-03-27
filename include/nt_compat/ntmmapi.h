/*
 * ntmmapi.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 4 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTMMAPI_H_H_
#define _NT_COMPAT_NTMMAPI_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT Memory Manager API */

#ifndef _NTMMAPI_H_COMPAT
#define _NTMMAPI_H_COMPAT

#ifndef MEM_COMMIT
#define MEM_COMMIT                                    0x1000
#endif
#ifndef MEM_RESERVE
#define MEM_RESERVE                                   0x2000
#endif
#ifndef MEM_DECOMMIT
#define MEM_DECOMMIT                                  0x4000
#endif
#ifndef MEM_RELEASE
#define MEM_RELEASE                                   0x8000
#endif
#ifndef MEM_FREE
#define MEM_FREE                                      0x10000
#endif
#ifndef MEM_RESET
#define MEM_RESET                                     0x80000
#endif
#ifndef MEM_TOP_DOWN
#define MEM_TOP_DOWN                                  0x100000
#endif
#ifndef MEM_LARGE_PAGES
#define MEM_LARGE_PAGES                               0x20000000
#endif
#ifndef PAGE_NOACCESS
#define PAGE_NOACCESS                                 0x01
#endif
#ifndef PAGE_READONLY
#define PAGE_READONLY                                 0x02
#endif
#ifndef PAGE_READWRITE
#define PAGE_READWRITE                                0x04
#endif
#ifndef PAGE_WRITECOPY
#define PAGE_WRITECOPY                                0x08
#endif
#ifndef PAGE_EXECUTE
#define PAGE_EXECUTE                                  0x10
#endif
#ifndef PAGE_EXECUTE_READ
#define PAGE_EXECUTE_READ                             0x20
#endif
#ifndef PAGE_EXECUTE_READWRITE
#define PAGE_EXECUTE_READWRITE                        0x40
#endif
#ifndef PAGE_GUARD
#define PAGE_GUARD                                    0x100
#endif
#ifndef PAGE_NOCACHE
#define PAGE_NOCACHE                                  0x200
#endif

#endif /* _NTMMAPI_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTMMAPI_H_H_ */
