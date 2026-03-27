/*++ BUILD Version: 0001
 * prefix.h — Prefix Table Compatibility Shim
 *
 * Used by DFS and redirector for prefix resolution.
 *--*/
#ifndef _PREFIX_H_
#define _PREFIX_H_

#include "ntrtl.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── Prefix table entry ── */
typedef struct _PREFIX_TABLE_ENTRY {
    SHORT               NodeTypeCode;
    SHORT               NameLength;
    struct _PREFIX_TABLE_ENTRY *NextPrefixTree;
    RTL_SPLAY_LINKS     Links;
    PSTRING             Prefix;
} PREFIX_TABLE_ENTRY, *PPREFIX_TABLE_ENTRY;

/* ── Prefix table ── */
typedef struct _PREFIX_TABLE {
    SHORT               NodeTypeCode;
    SHORT               NameLength;
    PPREFIX_TABLE_ENTRY NextPrefixTree;
} PREFIX_TABLE, *PPREFIX_TABLE;

/* ── Unicode prefix table ── */
typedef struct _UNICODE_PREFIX_TABLE_ENTRY {
    SHORT               NodeTypeCode;
    SHORT               NameLength;
    struct _UNICODE_PREFIX_TABLE_ENTRY *NextPrefixTree;
    struct _UNICODE_PREFIX_TABLE_ENTRY *CaseMatch;
    RTL_SPLAY_LINKS     Links;
    PUNICODE_STRING     Prefix;
} UNICODE_PREFIX_TABLE_ENTRY, *PUNICODE_PREFIX_TABLE_ENTRY;

typedef struct _UNICODE_PREFIX_TABLE {
    SHORT               NodeTypeCode;
    SHORT               NameLength;
    PUNICODE_PREFIX_TABLE_ENTRY NextPrefixTree;
    PUNICODE_PREFIX_TABLE_ENTRY LastNextEntry;
} UNICODE_PREFIX_TABLE, *PUNICODE_PREFIX_TABLE;

/* RTL_SPLAY_LINKS defined in ntrtl.h */

static inline void RtlInitializePrefixTable(PPREFIX_TABLE Table) {
    Table->NodeTypeCode = 0; Table->NameLength = 0; Table->NextPrefixTree = NULL;
}

static inline void RtlInitializeUnicodePrefixTable(PUNICODE_PREFIX_TABLE Table) {
    Table->NodeTypeCode = 0; Table->NameLength = 0;
    Table->NextPrefixTree = NULL; Table->LastNextEntry = NULL;
}

#ifdef __cplusplus
}
#endif

#endif /* _PREFIX_H_ */
