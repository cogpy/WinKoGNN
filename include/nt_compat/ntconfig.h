/*
 * ntconfig.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 7 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTCONFIG_H_H_
#define _NT_COMPAT_NTCONFIG_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT Configuration Manager */

#ifndef _NTCONFIG_H_COMPAT
#define _NTCONFIG_H_COMPAT

#ifndef KEY_QUERY_VALUE
#define KEY_QUERY_VALUE                               0x0001
#endif
#ifndef KEY_SET_VALUE
#define KEY_SET_VALUE                                 0x0002
#endif
#ifndef KEY_CREATE_SUB_KEY
#define KEY_CREATE_SUB_KEY                            0x0004
#endif
#ifndef KEY_ENUMERATE_SUB_KEYS
#define KEY_ENUMERATE_SUB_KEYS                        0x0008
#endif
#ifndef KEY_NOTIFY
#define KEY_NOTIFY                                    0x0010
#endif
#ifndef KEY_CREATE_LINK
#define KEY_CREATE_LINK                               0x0020
#endif
#ifndef KEY_READ
#define KEY_READ                                      0x20019
#endif
#ifndef KEY_WRITE
#define KEY_WRITE                                     0x20006
#endif
#ifndef KEY_ALL_ACCESS
#define KEY_ALL_ACCESS                                0xF003F
#endif
#ifndef REG_NONE
#define REG_NONE                                      0
#endif
#ifndef REG_SZ
#define REG_SZ                                        1
#endif
#ifndef REG_EXPAND_SZ
#define REG_EXPAND_SZ                                 2
#endif
#ifndef REG_BINARY
#define REG_BINARY                                    3
#endif
#ifndef REG_DWORD
#define REG_DWORD                                     4
#endif
#ifndef REG_MULTI_SZ
#define REG_MULTI_SZ                                  7
#endif

#endif /* _NTCONFIG_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTCONFIG_H_H_ */
