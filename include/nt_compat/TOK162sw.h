/* tok162sw.h / TOK162sw.h — IBM Token Ring 16/4 adapter header stub */
#ifndef _TOK162SW_H_COMPAT_
#define _TOK162SW_H_COMPAT_
#include "ntdef.h"
#include "ndis.h"
#define TOK162_NDIS_MAJOR_VERSION 3
#define TOK162_NDIS_MINOR_VERSION 0
typedef struct _TOK162_ADAPTER { NDIS_HANDLE MiniportAdapterHandle; ULONG InterruptLevel; ULONG IoBaseAddress; } TOK162_ADAPTER, *PTOK162_ADAPTER;
#endif
