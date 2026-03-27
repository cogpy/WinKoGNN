/*
 * ntddnetd.h — NT DDK Network Device I/O Control Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Referenced by 9 source files.
 */
#ifndef _NT_COMPAT_NTDDNETD_H_
#define _NT_COMPAT_NTDDNETD_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Network device type */
#ifndef FILE_DEVICE_NETWORK
#define FILE_DEVICE_NETWORK 0x00000012
#endif

#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTDDNETD_H_ */
