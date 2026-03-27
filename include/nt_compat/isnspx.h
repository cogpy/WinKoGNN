/* isnspx.h — IPX/SPX protocol header stub */
#ifndef _ISNSPX_H_COMPAT_
#define _ISNSPX_H_COMPAT_
#include "ntdef.h"
#define SPX_DEVICE_NAME L"\\Device\\NwlnkSpx"
#define SOCKET_CONNECTION_ORIENTED 1
#define SOCKET_DATAGRAM 2
typedef struct _SPX_CONN_STATS { ULONG DataFramesSent; ULONG DataFramesReceived; ULONG DataBytesSent; ULONG DataBytesReceived; } SPX_CONN_STATS, *PSPX_CONN_STATS;
#endif
