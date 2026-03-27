/*++
 * ntrtl.h — NT Runtime Library (Cross-Platform Compatibility Shim)
 *
 * Part of WinKoGNN Unified AGI-OS Build
 *
 * Provides the RTL (Runtime Library) functions used throughout the
 * NT kernel source tree.  These are the utility functions that sit
 * between the kernel proper and the HAL.
 *--*/

#ifndef _NTRTL_H_COMPAT_
#define _NTRTL_H_COMPAT_

#include "ntdef.h"
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */
/* Unicode string operations                                           */
/* ------------------------------------------------------------------ */
static inline void RtlInitUnicodeString(
    PUNICODE_STRING DestinationString,
    PCWSTR SourceString)
{
    if (SourceString) {
        USHORT len = 0;
        const WCHAR *p = SourceString;
        while (*p++) len++;
        DestinationString->Length = (USHORT)(len * sizeof(WCHAR));
        DestinationString->MaximumLength = (USHORT)((len + 1) * sizeof(WCHAR));
        DestinationString->Buffer = (PWSTR)SourceString;
    } else {
        DestinationString->Length = 0;
        DestinationString->MaximumLength = 0;
        DestinationString->Buffer = NULL;
    }
}

static inline BOOLEAN RtlEqualUnicodeString(
    const UNICODE_STRING *String1,
    const UNICODE_STRING *String2,
    BOOLEAN CaseInSensitive)
{
    (void)CaseInSensitive;
    if (String1->Length != String2->Length) return FALSE;
    return memcmp(String1->Buffer, String2->Buffer, String1->Length) == 0;
}

static inline NTSTATUS RtlUnicodeStringToAnsiString(
    PANSI_STRING DestinationString,
    PCUNICODE_STRING SourceString,
    BOOLEAN AllocateDestinationString)
{
    USHORT len = SourceString->Length / sizeof(WCHAR);
    if (AllocateDestinationString) {
        DestinationString->Buffer = (PCHAR)malloc(len + 1);
        if (!DestinationString->Buffer) return STATUS_NO_MEMORY;
        DestinationString->MaximumLength = len + 1;
    }
    for (USHORT i = 0; i < len && i < DestinationString->MaximumLength - 1; i++) {
        DestinationString->Buffer[i] = (CHAR)(SourceString->Buffer[i] & 0xFF);
    }
    DestinationString->Buffer[len] = '\0';
    DestinationString->Length = len;
    return STATUS_SUCCESS;
}

static inline void RtlFreeUnicodeString(PUNICODE_STRING UnicodeString) {
    if (UnicodeString->Buffer) {
        free(UnicodeString->Buffer);
        UnicodeString->Buffer = NULL;
        UnicodeString->Length = 0;
        UnicodeString->MaximumLength = 0;
    }
}

static inline void RtlFreeAnsiString(PANSI_STRING AnsiString) {
    if (AnsiString->Buffer) {
        free(AnsiString->Buffer);
        AnsiString->Buffer = NULL;
        AnsiString->Length = 0;
        AnsiString->MaximumLength = 0;
    }
}

static inline NTSTATUS RtlAppendUnicodeStringToString(
    PUNICODE_STRING Destination,
    const UNICODE_STRING *Source)
{
    if (Destination->Length + Source->Length > Destination->MaximumLength)
        return STATUS_BUFFER_TOO_SMALL;
    memcpy((PCHAR)Destination->Buffer + Destination->Length,
           Source->Buffer, Source->Length);
    Destination->Length += Source->Length;
    return STATUS_SUCCESS;
}

/* ------------------------------------------------------------------ */
/* ANSI string operations                                              */
/* ------------------------------------------------------------------ */
static inline void RtlInitString(PSTRING DestinationString, PCSTR SourceString) {
    if (SourceString) {
        USHORT len = (USHORT)strlen(SourceString);
        DestinationString->Length = len;
        DestinationString->MaximumLength = len + 1;
        DestinationString->Buffer = (PCHAR)SourceString;
    } else {
        DestinationString->Length = 0;
        DestinationString->MaximumLength = 0;
        DestinationString->Buffer = NULL;
    }
}

#define RtlInitAnsiString RtlInitString

/* ------------------------------------------------------------------ */
/* Memory operations                                                   */
/* ------------------------------------------------------------------ */
#define RtlCopyMemory(Dest, Src, Len)   memcpy((Dest), (Src), (Len))
#define RtlMoveMemory(Dest, Src, Len)   memmove((Dest), (Src), (Len))
#define RtlZeroMemory(Dest, Len)        memset((Dest), 0, (Len))
#define RtlFillMemory(Dest, Len, Fill)  memset((Dest), (Fill), (Len))
#define RtlCompareMemory(S1, S2, Len)   memcmp((S1), (S2), (Len))

#define RtlEqualMemory(Dest, Src, Len)  (memcmp((Dest), (Src), (Len)) == 0)

/* ------------------------------------------------------------------ */
/* Bitmap operations                                                   */
/* ------------------------------------------------------------------ */
typedef struct _RTL_BITMAP {
    ULONG   SizeOfBitMap;
    PULONG  Buffer;
} RTL_BITMAP, *PRTL_BITMAP;

static inline void RtlInitializeBitMap(
    PRTL_BITMAP BitMapHeader,
    PULONG BitMapBuffer,
    ULONG SizeOfBitMap)
{
    BitMapHeader->SizeOfBitMap = SizeOfBitMap;
    BitMapHeader->Buffer = BitMapBuffer;
}

static inline void RtlSetBit(PRTL_BITMAP BitMapHeader, ULONG BitNumber) {
    BitMapHeader->Buffer[BitNumber / 32] |= (1UL << (BitNumber % 32));
}

static inline void RtlClearBit(PRTL_BITMAP BitMapHeader, ULONG BitNumber) {
    BitMapHeader->Buffer[BitNumber / 32] &= ~(1UL << (BitNumber % 32));
}

static inline BOOLEAN RtlTestBit(PRTL_BITMAP BitMapHeader, ULONG BitNumber) {
    return (BitMapHeader->Buffer[BitNumber / 32] & (1UL << (BitNumber % 32))) != 0;
}

static inline void RtlClearAllBits(PRTL_BITMAP BitMapHeader) {
    RtlZeroMemory(BitMapHeader->Buffer,
                   (BitMapHeader->SizeOfBitMap + 31) / 32 * sizeof(ULONG));
}

static inline void RtlSetAllBits(PRTL_BITMAP BitMapHeader) {
    RtlFillMemory(BitMapHeader->Buffer,
                   (BitMapHeader->SizeOfBitMap + 31) / 32 * sizeof(ULONG), 0xFF);
}

/* ------------------------------------------------------------------ */
/* Splay tree (used by NT memory manager and object manager)           */
/* ------------------------------------------------------------------ */
typedef struct _RTL_SPLAY_LINKS {
    struct _RTL_SPLAY_LINKS *Parent;
    struct _RTL_SPLAY_LINKS *LeftChild;
    struct _RTL_SPLAY_LINKS *RightChild;
} RTL_SPLAY_LINKS, *PRTL_SPLAY_LINKS;

static inline void RtlInitializeSplayLinks(PRTL_SPLAY_LINKS Links) {
    Links->Parent = Links;
    Links->LeftChild = NULL;
    Links->RightChild = NULL;
}

/* ------------------------------------------------------------------ */
/* AVL tree                                                            */
/* ------------------------------------------------------------------ */
typedef struct _RTL_BALANCED_LINKS {
    struct _RTL_BALANCED_LINKS *Parent;
    struct _RTL_BALANCED_LINKS *LeftChild;
    struct _RTL_BALANCED_LINKS *RightChild;
    CHAR Balance;
    UCHAR Reserved[3];
} RTL_BALANCED_LINKS, *PRTL_BALANCED_LINKS;

typedef struct _RTL_AVL_TABLE {
    RTL_BALANCED_LINKS BalancedRoot;
    PVOID OrderedPointer;
    ULONG WhichOrderedElement;
    ULONG NumberGenericTableElements;
    ULONG DepthOfTree;
    PRTL_BALANCED_LINKS RestartKey;
    ULONG DeleteCount;
    PVOID CompareRoutine;
    PVOID AllocateRoutine;
    PVOID FreeRoutine;
    PVOID TableContext;
} RTL_AVL_TABLE, *PRTL_AVL_TABLE;

/* ------------------------------------------------------------------ */
/* Generic table                                                       */
/* ------------------------------------------------------------------ */
typedef struct _RTL_GENERIC_TABLE {
    PRTL_SPLAY_LINKS TableRoot;
    LIST_ENTRY InsertOrderList;
    PLIST_ENTRY OrderedPointer;
    ULONG WhichOrderedElement;
    ULONG NumberGenericTableElements;
    PVOID CompareRoutine;
    PVOID AllocateRoutine;
    PVOID FreeRoutine;
    PVOID TableContext;
} RTL_GENERIC_TABLE, *PRTL_GENERIC_TABLE;

/* ------------------------------------------------------------------ */
/* Heap management                                                     */
/* ------------------------------------------------------------------ */
static inline PVOID RtlAllocateHeap(PVOID HeapHandle, ULONG Flags, SIZE_T Size) {
    (void)HeapHandle; (void)Flags;
    return malloc(Size);
}

static inline BOOLEAN RtlFreeHeap(PVOID HeapHandle, ULONG Flags, PVOID BaseAddress) {
    (void)HeapHandle; (void)Flags;
    free(BaseAddress);
    return TRUE;
}

/* ------------------------------------------------------------------ */
/* Exception handling (stubs)                                          */
/* ------------------------------------------------------------------ */
#ifndef _EXCEPTION_RECORD_DEFINED
#define _EXCEPTION_RECORD_DEFINED
typedef struct _EXCEPTION_RECORD {
    NTSTATUS    ExceptionCode;
    ULONG       ExceptionFlags;
    struct _EXCEPTION_RECORD *ExceptionRecord;
    PVOID       ExceptionAddress;
    ULONG       NumberParameters;
    ULONG_PTR   ExceptionInformation[15];
} EXCEPTION_RECORD, *PEXCEPTION_RECORD;
#endif

/* ------------------------------------------------------------------ */
/* Registry (stubs for compilation)                                    */
/* ------------------------------------------------------------------ */
typedef struct _KEY_VALUE_PARTIAL_INFORMATION {
    ULONG   TitleIndex;
    ULONG   Type;
    ULONG   DataLength;
    UCHAR   Data[1];
} KEY_VALUE_PARTIAL_INFORMATION, *PKEY_VALUE_PARTIAL_INFORMATION;

/* Registry value types */
#define REG_NONE                    0
#define REG_SZ                      1
#define REG_EXPAND_SZ               2
#define REG_BINARY                  3
#define REG_DWORD                   4
#define REG_DWORD_LITTLE_ENDIAN     4
#define REG_DWORD_BIG_ENDIAN        5
#define REG_LINK                    6
#define REG_MULTI_SZ                7
#define REG_RESOURCE_LIST           8

/* ------------------------------------------------------------------ */
/* Conversion macros                                                   */
/* ------------------------------------------------------------------ */
#define RtlUlongByteSwap(x) \
    ((((x) & 0xFF000000) >> 24) | (((x) & 0x00FF0000) >> 8) | \
     (((x) & 0x0000FF00) << 8)  | (((x) & 0x000000FF) << 24))

#define RtlUshortByteSwap(x) \
    ((USHORT)((((x) & 0xFF00) >> 8) | (((x) & 0x00FF) << 8)))

/* ------------------------------------------------------------------ */
/* Interlocked operations (GCC/Clang builtins)                         */
/* ------------------------------------------------------------------ */
#ifndef _WIN32
#define InterlockedIncrement(p)     __sync_add_and_fetch((volatile LONG*)(p), 1)
#define InterlockedDecrement(p)     __sync_sub_and_fetch((volatile LONG*)(p), 1)
#define InterlockedExchange(p, v)   __sync_lock_test_and_set((volatile LONG*)(p), (v))
#define InterlockedCompareExchange(p, e, c) \
    __sync_val_compare_and_swap((volatile LONG*)(p), (c), (e))
#define InterlockedExchangeAdd(p, v) __sync_fetch_and_add((volatile LONG*)(p), (v))
#endif

#ifdef __cplusplus
}
#endif

#endif /* _NTRTL_H_COMPAT_ */
