/* prefixp.h — MUP Prefix Table Private header stub */
#ifndef _PREFIXP_H_COMPAT_
#define _PREFIXP_H_COMPAT_
#include "ntdef.h"
#include "prefix.h"
typedef struct _DFS_PREFIX_TABLE_ENTRY { UNICODE_STRING Prefix; PVOID Data; struct _DFS_PREFIX_TABLE_ENTRY *Parent; struct _DFS_PREFIX_TABLE_ENTRY *Children; struct _DFS_PREFIX_TABLE_ENTRY *Sibling; } DFS_PREFIX_TABLE_ENTRY, *PDFS_PREFIX_TABLE_ENTRY;
#endif
