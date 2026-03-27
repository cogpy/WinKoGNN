/*
 * ntiologc.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 14 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTIOLOGC_H_H_
#define _NT_COMPAT_NTIOLOGC_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NT I/O Log Codes */

#ifndef _NTIOLOGC_H_COMPAT
#define _NTIOLOGC_H_COMPAT

#define NTIOLOGC_SEVERITY_SUCCESS                     0x00000000
#define NTIOLOGC_SEVERITY_INFORMATIONAL               0x40000000
#define NTIOLOGC_SEVERITY_WARNING                     0x80000000
#define NTIOLOGC_SEVERITY_ERROR                       0xC0000000
#define IO_ERR_INSUFFICIENT_RESOURCES                 0xC004000EL
#define IO_ERR_DRIVER_ERROR                           0xC0040004L
#define IO_ERR_CONFIGURATION_ERROR                    0xC0040003L
#define IO_ERR_TIMEOUT                                0xC0040019L
#define IO_ERR_BAD_FIRMWARE                           0xC004001CL
#define IO_ERR_INTERNAL_ERROR                         0xC0040006L
#define IO_ERR_OVERRUN_ERROR                          0xC004000BL
#define IO_ERR_RESET                                  0xC0040017L
#define IO_ERR_NOT_READY                              0xC004000FL
#define IO_WRN_FAILURE_PREDICTED                      0x80040032L

#endif /* _NTIOLOGC_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTIOLOGC_H_H_ */
