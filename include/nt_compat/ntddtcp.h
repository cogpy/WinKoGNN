/*
 * ntddtcp.h — NT DDK TCP Device I/O Control Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 */
#ifndef _NT_COMPAT_NTDDTCP_H_
#define _NT_COMPAT_NTDDTCP_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FILE_DEVICE_TRANSPORT
#define FILE_DEVICE_TRANSPORT 0x00000021
#endif

/* TCP device name */
#define DD_TCP_DEVICE_NAME L"\\Device\\Tcp"
#define DD_UDP_DEVICE_NAME L"\\Device\\Udp"
#define DD_RAW_IP_DEVICE_NAME L"\\Device\\RawIp"

#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTDDTCP_H_ */
