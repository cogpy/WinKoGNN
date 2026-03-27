/*
 * ntddnfs.h — NT DDK NFS Device I/O Control Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Referenced by 30 source files (NFS redirector).
 */
#ifndef _NT_COMPAT_NTDDNFS_H_
#define _NT_COMPAT_NTDDNFS_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NFS file device type */
#ifndef FILE_DEVICE_NETWORK_FILE_SYSTEM
#define FILE_DEVICE_NETWORK_FILE_SYSTEM 0x00000014
#endif

/* NFS FSCTL codes (minimal stubs) */
#ifndef FSCTL_NFS_BASE
#define FSCTL_NFS_BASE FILE_DEVICE_NETWORK_FILE_SYSTEM
#endif

#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTDDNFS_H_ */
