/*
 * ntddstor.h — NT DDK Storage Device I/O Control Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Prevents the Windows SDK ntddstor.h from being included (1.2k+ errors).
 */
#ifndef _NT_COMPAT_NTDDSTOR_H_
#define _NT_COMPAT_NTDDSTOR_H_

/* Prevent the Windows SDK ntddstor.h from being included */
#ifndef _NTDDSTOR_H_
#define _NTDDSTOR_H_
#endif

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* DEVICE_TYPE — used in storage device descriptors */
#ifndef _DEVICE_TYPE_DEFINED
#define _DEVICE_TYPE_DEFINED
typedef ULONG DEVICE_TYPE;
#endif

/* FILE_DEVICE_DISK */
#ifndef FILE_DEVICE_DISK
#define FILE_DEVICE_DISK 0x00000007
#endif

/* IOCTL_STORAGE codes (minimal stubs) */
#ifndef IOCTL_STORAGE_BASE
#define IOCTL_STORAGE_BASE FILE_DEVICE_DISK
#endif

#ifndef _STORAGE_DEVICE_NUMBER_DEFINED
#define _STORAGE_DEVICE_NUMBER_DEFINED
typedef struct _STORAGE_DEVICE_NUMBER {
    DEVICE_TYPE DeviceType;
    ULONG       DeviceNumber;
    ULONG       PartitionNumber;
} STORAGE_DEVICE_NUMBER, *PSTORAGE_DEVICE_NUMBER;
#endif

#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTDDSTOR_H_ */
