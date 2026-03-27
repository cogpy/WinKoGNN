/*
 * tdiinfo.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 23 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_TDIINFO_H_H_
#define _NT_COMPAT_TDIINFO_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif


/* TDI Information definitions */
#ifndef _TDIINFO_H_COMPAT
#define _TDIINFO_H_COMPAT
typedef struct TDIObjectID { ULONG toi_entity; ULONG toi_class; ULONG toi_type; ULONG toi_id; } TDIObjectID;
typedef struct TDIEntityID { ULONG tei_entity; ULONG tei_instance; } TDIEntityID;
#define INFO_CLASS_GENERIC      0x100
#define INFO_CLASS_PROTOCOL     0x200
#define INFO_CLASS_IMPLEMENTATION 0x300
#define INFO_TYPE_PROVIDER      0x100
#define INFO_TYPE_ADDRESS_OBJECT 0x200
#define INFO_TYPE_CONNECTION    0x300
#define ENTITY_LIST_ID          0
#define ENTITY_TYPE_ID          1
#define CL_TL_ENTITY            0x401
#define CO_TL_ENTITY            0x402
#define CL_NL_ENTITY            0x301
#define ER_ENTITY               0x380
#define IF_ENTITY               0x200
#define AT_ENTITY               0x280
#endif /* _TDIINFO_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_TDIINFO_H_H_ */
