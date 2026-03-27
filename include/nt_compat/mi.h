/* Memory Manager Internal — private Mm types */
#ifndef _MI_H_COMPAT
#define _MI_H_COMPAT
#include "ntdef.h"
typedef struct _MMPTE { ULONG_PTR Long; } MMPTE, *PMMPTE;
typedef struct _MMPFN { ULONG_PTR Flags; } MMPFN, *PMMPFN;
NTSTATUS NTAPI MiMapViewOfSection(IN PVOID Section, IN PVOID Process, IN OUT PVOID *BaseAddress, IN ULONG_PTR ZeroBits, IN SIZE_T CommitSize, IN OUT PLARGE_INTEGER SectionOffset, IN OUT PSIZE_T ViewSize);
#endif
