/*++
 * ntdef.h — NT Base Type Definitions (Cross-Platform Compatibility Shim)
 *
 * Part of WinKoGNN Unified AGI-OS Build
 *
 * IMPORTANT: On Windows/MSVC, this header must NOT include <windows.h>
 * because our nt_compat/ directory is the first -I path and would
 * intercept SDK headers (windef.h, etc.) creating circular failures.
 *
 * Design: Provide the complete NT4 DDK type system from scratch on ALL
 * platforms using only <stdint.h>/<stddef.h>/<stdlib.h>.
 *
 * All composite types are guarded with #ifndef _TYPENAME_DEFINED so
 * that the NT4 source tree's own internal headers (ntosdef.h, io.h,
 * ke.h, etc.) can safely redefine them without C2011 errors.
 *--*/

#ifndef _NTDEF_H_COMPAT_
#define _NTDEF_H_COMPAT_

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */
/* Standard library includes                                           */
/* ------------------------------------------------------------------ */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>   /* malloc/free — prevents C2375 linkage conflict */
#include <string.h>   /* memset/memcpy/memmove */

/* ------------------------------------------------------------------ */
/* Fundamental scalar types                                            */
/* ------------------------------------------------------------------ */
#ifndef _NTDEF_SCALARS_DEFINED_
#define _NTDEF_SCALARS_DEFINED_
typedef unsigned char       UCHAR;
typedef unsigned char       BYTE;
typedef unsigned char       BOOLEAN;
typedef char                CHAR;
typedef signed char         SCHAR;      /* NT signed char */
typedef signed char         CCHAR;      /* NT compact char */
typedef int16_t             SHORT;
typedef int16_t             CSHORT;     /* NT compact short */
typedef uint16_t            USHORT;
typedef uint16_t            WORD;
typedef int32_t             LONG;
typedef uint32_t            ULONG;
typedef uint32_t            DWORD;
typedef int64_t             LONGLONG;
typedef uint64_t            ULONGLONG;
typedef uint64_t            DWORD64;
typedef void               *PVOID;
typedef void               *HANDLE;
typedef HANDLE             *PHANDLE;
typedef ULONG              *PULONG;
typedef USHORT             *PUSHORT;
typedef UCHAR              *PUCHAR;
typedef CHAR               *PCHAR;
typedef CCHAR              *PCCHAR;
typedef BYTE               *PBYTE;
typedef BOOLEAN            *PBOOLEAN;
typedef LONG               *PLONG;
typedef LONGLONG           *PLONGLONG;
typedef ULONGLONG          *PULONGLONG;
typedef const char         *PCSTR;
typedef const char         *LPCSTR;
typedef void                VOID;
typedef int                 INT;
typedef unsigned int        UINT;
#endif /* _NTDEF_SCALARS_DEFINED_ */

/* Pointer-sized types */
#ifndef _NTDEF_PTR_TYPES_DEFINED_
#define _NTDEF_PTR_TYPES_DEFINED_
#if defined(__LP64__) || defined(_WIN64)
typedef int64_t             LONG_PTR;
typedef uint64_t            ULONG_PTR;
typedef uint64_t            SIZE_T;
typedef int64_t             SSIZE_T;
#else
typedef int32_t             LONG_PTR;
typedef uint32_t            ULONG_PTR;
typedef uint32_t            SIZE_T;
typedef int32_t             SSIZE_T;
#endif
typedef ULONG_PTR           DWORD_PTR;
typedef SIZE_T             *PSIZE_T;
#endif /* _NTDEF_PTR_TYPES_DEFINED_ */

/* Wide string pointer types */
typedef uint16_t            WCHAR;
typedef WCHAR              *PWCHAR;
typedef WCHAR              *PWSTR;
typedef WCHAR              *LPWSTR;
typedef const WCHAR        *PCWSTR;
typedef const WCHAR        *LPCWSTR;

/* ------------------------------------------------------------------ */
/* NT Status type                                                      */
/* ------------------------------------------------------------------ */
typedef LONG                NTSTATUS;
typedef NTSTATUS           *PNTSTATUS;

#define NT_SUCCESS(Status)          ((NTSTATUS)(Status) >= 0)
#define NT_INFORMATION(Status)      ((ULONG)(Status) >> 30 == 1)
#define NT_WARNING(Status)          ((ULONG)(Status) >> 30 == 2)
#define NT_ERROR(Status)            ((ULONG)(Status) >> 30 == 3)

/* Common status codes */
#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS              ((NTSTATUS)0x00000000L)
#define STATUS_UNSUCCESSFUL         ((NTSTATUS)0xC0000001L)
#define STATUS_NOT_IMPLEMENTED      ((NTSTATUS)0xC0000002L)
#define STATUS_INVALID_PARAMETER    ((NTSTATUS)0xC000000DL)
#define STATUS_NO_MEMORY            ((NTSTATUS)0xC0000017L)
#define STATUS_ACCESS_DENIED        ((NTSTATUS)0xC0000022L)
#define STATUS_OBJECT_NAME_NOT_FOUND ((NTSTATUS)0xC0000034L)
#define STATUS_BUFFER_TOO_SMALL     ((NTSTATUS)0xC0000023L)
#define STATUS_PENDING              ((NTSTATUS)0x00000103L)
#define STATUS_TIMEOUT              ((NTSTATUS)0x00000102L)
#define STATUS_BUFFER_OVERFLOW      ((NTSTATUS)0x80000005L)
#define STATUS_NO_MORE_ENTRIES      ((NTSTATUS)0x8000001AL)
#endif

/* ------------------------------------------------------------------ */
/* Boolean constants                                                   */
/* ------------------------------------------------------------------ */
#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif
#ifndef NULL
#define NULL    ((void *)0)
#endif

/* ------------------------------------------------------------------ */
/* Calling conventions                                                 */
/* ------------------------------------------------------------------ */
#ifdef _WIN32
  #ifndef NTAPI
    #define NTAPI       __stdcall
  #endif
  #ifndef WINAPI
    #define WINAPI      __stdcall
  #endif
  #ifndef CALLBACK
    #define CALLBACK    __stdcall
  #endif
  #ifndef APIENTRY
    #define APIENTRY    __stdcall
  #endif
  #ifndef DECLSPEC_IMPORT
    #define DECLSPEC_IMPORT __declspec(dllimport)
  #endif
  #ifndef DECLSPEC_NORETURN
    #define DECLSPEC_NORETURN __declspec(noreturn)
  #endif
  #ifndef FORCEINLINE
    #define FORCEINLINE __forceinline
  #endif
  #ifndef NTSYSCALLAPI
    #define NTSYSCALLAPI
  #endif
  #ifndef NTKERNELAPI
    #if (defined(_NTDRIVER_) || defined(_NTDDK_) || defined(_NTIFS_) || defined(_NTHAL_)) && !defined(_BLDR_)
      #define NTKERNELAPI DECLSPEC_IMPORT
    #else
      #define NTKERNELAPI
    #endif
  #endif
  #ifndef NTHALAPI
    #if !defined(_NTHAL_) && !defined(_BLDR_)
      #define NTHALAPI DECLSPEC_IMPORT
    #else
      #define NTHALAPI
    #endif
  #endif
  #ifndef FASTCALL
    #define FASTCALL __fastcall
  #endif
  /* Force IN/OUT/OPTIONAL to be empty (NT4 convention) */
  #ifdef IN
    #undef IN
  #endif
  #define IN
  #ifdef OUT
    #undef OUT
  #endif
  #define OUT
  #ifdef OPTIONAL
    #undef OPTIONAL
  #endif
  #define OPTIONAL
  #ifndef CONST
    #define CONST       const
  #endif
  #ifndef UNALIGNED
    #if defined(_M_X64) || defined(_M_AMD64)
      #define UNALIGNED __unaligned
    #else
      #define UNALIGNED
    #endif
  #endif
#else
  /* Non-Windows: all calling conventions are no-ops */
  #define NTAPI
  #define WINAPI
  #define CALLBACK
  #define APIENTRY
  #ifndef __stdcall
    #define __stdcall
  #endif
  #ifndef __cdecl
    #define __cdecl
  #endif
  #ifndef __fastcall
    #define __fastcall
  #endif
  #define DECLSPEC_IMPORT
  #define DECLSPEC_NORETURN
  #define FORCEINLINE         static inline
  #define NTSYSCALLAPI
  #define NTKERNELAPI
  #define NTHALAPI
  #define FASTCALL
  #define IN
  #define OUT
  #define OPTIONAL
  #define CONST               const
  #define UNALIGNED
#endif

/* ------------------------------------------------------------------ */
/* Unicode / ANSI string types                                         */
/* ------------------------------------------------------------------ */
#ifndef _UNICODE_STRING_DEFINED
#define _UNICODE_STRING_DEFINED
typedef struct _UNICODE_STRING {
    USHORT  Length;
    USHORT  MaximumLength;
    PWSTR   Buffer;
} UNICODE_STRING, *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;
#endif

#ifndef _STRING_DEFINED
#define _STRING_DEFINED
typedef struct _STRING {
    USHORT  Length;
    USHORT  MaximumLength;
    PCHAR   Buffer;
} STRING, *PSTRING, ANSI_STRING, *PANSI_STRING, OEM_STRING, *POEM_STRING;
#endif

/* ------------------------------------------------------------------ */
/* LARGE_INTEGER (guarded — ntosdef.h may redefine)                    */
/* ------------------------------------------------------------------ */
#ifndef _LARGE_INTEGER_DEFINED
#define _LARGE_INTEGER_DEFINED
/*
 * NT4 source accesses .LowPart / .HighPart directly (no .u. prefix).
 * We use an anonymous struct so both .LowPart and .u.LowPart work.
 * MSVC supports anonymous structs natively; GCC/Clang need the
 * pragma or -fms-extensions.  C11 also allows anonymous structs.
 */
#if defined(__GNUC__) || defined(__clang__)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpedantic"
#endif
typedef union _LARGE_INTEGER {
    struct {
        ULONG LowPart;
        LONG  HighPart;
    };              /* anonymous — direct .LowPart access */
    struct {
        ULONG LowPart;
        LONG  HighPart;
    } u;            /* named — .u.LowPart also works */
    LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;
#if defined(__GNUC__) || defined(__clang__)
  #pragma GCC diagnostic pop
#endif
#endif

#ifndef _ULARGE_INTEGER_DEFINED
#define _ULARGE_INTEGER_DEFINED
#if defined(__GNUC__) || defined(__clang__)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpedantic"
#endif
typedef union _ULARGE_INTEGER {
    struct {
        ULONG LowPart;
        ULONG HighPart;
    };              /* anonymous — direct .LowPart access */
    struct {
        ULONG LowPart;
        ULONG HighPart;
    } u;            /* named — .u.LowPart also works */
    ULONGLONG QuadPart;
} ULARGE_INTEGER, *PULARGE_INTEGER;
#if defined(__GNUC__) || defined(__clang__)
  #pragma GCC diagnostic pop
#endif
#endif

/* ------------------------------------------------------------------ */
/* Doubly-linked list (guarded — many NT headers redefine)             */
/* ------------------------------------------------------------------ */
#ifndef _LIST_ENTRY_DEFINED
#define _LIST_ENTRY_DEFINED
typedef struct _LIST_ENTRY {
    struct _LIST_ENTRY *Flink;
    struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY;
#endif

#ifndef _SINGLE_LIST_ENTRY_DEFINED
#define _SINGLE_LIST_ENTRY_DEFINED
typedef struct _SINGLE_LIST_ENTRY {
    struct _SINGLE_LIST_ENTRY *Next;
} SINGLE_LIST_ENTRY, *PSINGLE_LIST_ENTRY;
#endif

/* List manipulation macros */
#ifndef InitializeListHead
#define InitializeListHead(ListHead) \
    ((ListHead)->Flink = (ListHead)->Blink = (ListHead))
#endif

#ifndef IsListEmpty
#define IsListEmpty(ListHead) \
    ((ListHead)->Flink == (ListHead))
#endif

#define RemoveHeadList(ListHead) \
    (ListHead)->Flink; \
    { PLIST_ENTRY _f = (ListHead)->Flink; \
      _f->Flink->Blink = (ListHead); \
      (ListHead)->Flink = _f->Flink; }

#define RemoveTailList(ListHead) \
    (ListHead)->Blink; \
    { PLIST_ENTRY _b = (ListHead)->Blink; \
      _b->Blink->Flink = (ListHead); \
      (ListHead)->Blink = _b->Blink; }

#define InsertTailList(ListHead, Entry) \
    { PLIST_ENTRY _b = (ListHead)->Blink; \
      (Entry)->Flink = (ListHead); \
      (Entry)->Blink = _b; \
      _b->Flink = (Entry); \
      (ListHead)->Blink = (Entry); }

#define InsertHeadList(ListHead, Entry) \
    { PLIST_ENTRY _f = (ListHead)->Flink; \
      (Entry)->Flink = _f; \
      (Entry)->Blink = (ListHead); \
      _f->Blink = (Entry); \
      (ListHead)->Flink = (Entry); }

#define RemoveEntryList(Entry) \
    { PLIST_ENTRY _f = (Entry)->Flink; \
      PLIST_ENTRY _b = (Entry)->Blink; \
      _b->Flink = _f; \
      _f->Blink = _b; }

/* CONTAINING_RECORD — the NT kernel's equivalent of container_of */
#ifndef CONTAINING_RECORD
#define CONTAINING_RECORD(address, type, field) \
    ((type *)((PCHAR)(address) - (ULONG_PTR)(&((type *)0)->field)))
#endif

/* FIELD_OFFSET */
#ifndef FIELD_OFFSET
#define FIELD_OFFSET(type, field) ((LONG_PTR)(&((type *)0)->field))
#endif

/* ------------------------------------------------------------------ */
/* Object attributes                                                   */
/* ------------------------------------------------------------------ */
typedef struct _OBJECT_ATTRIBUTES {
    ULONG           Length;
    HANDLE          RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG           Attributes;
    PVOID           SecurityDescriptor;
    PVOID           SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

#define OBJ_INHERIT             0x00000002L
#define OBJ_PERMANENT           0x00000010L
#define OBJ_EXCLUSIVE           0x00000020L
#define OBJ_CASE_INSENSITIVE    0x00000040L
#define OBJ_OPENIF              0x00000080L
#define OBJ_OPENLINK            0x00000100L
#define OBJ_KERNEL_HANDLE       0x00000200L

#define InitializeObjectAttributes(p, n, a, r, s) { \
    (p)->Length = sizeof(OBJECT_ATTRIBUTES); \
    (p)->RootDirectory = r; \
    (p)->Attributes = a; \
    (p)->ObjectName = n; \
    (p)->SecurityDescriptor = s; \
    (p)->SecurityQualityOfService = NULL; }

/* ------------------------------------------------------------------ */
/* I/O Status Block                                                    */
/* ------------------------------------------------------------------ */
typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID    Pointer;
    };
    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

/* ------------------------------------------------------------------ */
/* Client ID (process + thread identification)                         */
/* ------------------------------------------------------------------ */
typedef struct _CLIENT_ID {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

/* ------------------------------------------------------------------ */
/* IRQL and synchronization primitives (guarded)                       */
/* ------------------------------------------------------------------ */
typedef UCHAR               KIRQL;
typedef KIRQL              *PKIRQL;

#define PASSIVE_LEVEL       0
#define LOW_LEVEL           0
#define APC_LEVEL           1
#define DISPATCH_LEVEL      2
#define PROFILE_LEVEL       27
#define HIGH_LEVEL          31

#ifndef _KSPIN_LOCK_DEFINED
#define _KSPIN_LOCK_DEFINED
typedef struct _KSPIN_LOCK {
    volatile ULONG_PTR Lock;
} KSPIN_LOCK, *PKSPIN_LOCK;
#endif

/* ------------------------------------------------------------------ */
/* KDPC (guarded — ntosdef.h redefines)                                */
/* ------------------------------------------------------------------ */
#ifndef _KDPC_DEFINED
#define _KDPC_DEFINED
typedef struct _KDPC {
    SHORT           Type;
    UCHAR           Importance;
    UCHAR           Number;
    LIST_ENTRY      DpcListEntry;
    PVOID           DeferredRoutine;
    PVOID           DeferredContext;
    PVOID           SystemArgument1;
    PVOID           SystemArgument2;
    PVOID           DpcData;
} KDPC, *PKDPC, *PRKDPC;
#endif

/* ------------------------------------------------------------------ */
/* Dispatcher objects — FORWARD DECLARATIONS ONLY                      */
/* Full definitions are in private/ntos/inc/ke.h which does NOT use    */
/* _DEFINED guards.  We must NOT define them here to avoid C2011.      */
/* ------------------------------------------------------------------ */

/* Minimal DISPATCHER_HEADER — needed by many headers before ke.h.     */
/* ke.h will redefine it; MSVC C allows identical struct redefinition  */
/* only if the struct body is EXACTLY identical.  So we provide the    */
/* exact same layout ke.h uses.                                       */
#ifndef _DISPATCHER_HEADER_DEFINED
#define _DISPATCHER_HEADER_DEFINED
typedef struct _DISPATCHER_HEADER {
    UCHAR       Type;
    UCHAR       Absolute;
    UCHAR       Size;
    UCHAR       Inserted;
    LONG        SignalState;
    LIST_ENTRY  WaitListHead;
} DISPATCHER_HEADER;
#endif

/* KEVENT — guarded; ke.h also defines with same guard.                */
#ifndef _KEVENT_DEFINED
#define _KEVENT_DEFINED
typedef struct _KEVENT {
    DISPATCHER_HEADER Header;
} KEVENT, *PKEVENT, *PRKEVENT;
#endif

/* KMUTANT — guarded; ke.h also defines with same guard.               */
#ifndef _KMUTANT_DEFINED
#define _KMUTANT_DEFINED
typedef struct _KMUTANT {
    DISPATCHER_HEADER Header;
    LIST_ENTRY        MutantListEntry;
    PVOID             OwnerThread;
    BOOLEAN           Abandoned;
    UCHAR             ApcDisable;
} KMUTANT, *PKMUTANT, *PRKMUTANT, KMUTEX, *PKMUTEX;
#endif

/* KSEMAPHORE — guarded; ke.h also defines with same guard.            */
#ifndef _KSEMAPHORE_DEFINED
#define _KSEMAPHORE_DEFINED
typedef struct _KSEMAPHORE {
    DISPATCHER_HEADER Header;
    LONG              Limit;
} KSEMAPHORE, *PKSEMAPHORE, *PRKSEMAPHORE;
#endif

/* KTIMER — guarded; ke.h also defines with same guard.                */
#ifndef _KTIMER_DEFINED
#define _KTIMER_DEFINED
typedef struct _KTIMER {
    DISPATCHER_HEADER Header;
    ULARGE_INTEGER    DueTime;
    LIST_ENTRY        TimerListEntry;
    PKDPC             Dpc;
    LONG              Period;
} KTIMER, *PKTIMER, *PRKTIMER;
#endif

/* ------------------------------------------------------------------ */
/* Object type forward declarations                                    */
/* ------------------------------------------------------------------ */
typedef struct _OBJECT_TYPE *POBJECT_TYPE;

/* ------------------------------------------------------------------ */
/* Memory descriptor list (guarded — io.h may redefine)                */
/* ------------------------------------------------------------------ */
#ifndef _MDL_DEFINED
#define _MDL_DEFINED
typedef struct _MDL {
    struct _MDL    *Next;
    SHORT           Size;
    SHORT           MdlFlags;
    PVOID           Process;
    PVOID           MappedSystemVa;
    PVOID           StartVa;
    ULONG           ByteCount;
    ULONG           ByteOffset;
} MDL, *PMDL;
#endif

/* ------------------------------------------------------------------ */
/* Access mask                                                         */
/* ------------------------------------------------------------------ */
typedef ULONG ACCESS_MASK;
typedef ACCESS_MASK *PACCESS_MASK;

#ifndef DELETE
#define DELETE                  0x00010000L
#endif
#define READ_CONTROL            0x00020000L
#define WRITE_DAC               0x00040000L
#define WRITE_OWNER             0x00080000L
#define SYNCHRONIZE             0x00100000L
#define GENERIC_READ            0x80000000L
#define GENERIC_WRITE           0x40000000L
#define GENERIC_EXECUTE         0x20000000L
#define GENERIC_ALL             0x10000000L

/* ------------------------------------------------------------------ */
/* Processor modes (guarded — ntosdef.h redefines _MODE)               */
/* ------------------------------------------------------------------ */
#ifndef _MODE_DEFINED
#define _MODE_DEFINED
typedef enum _MODE {
    KernelMode,
    UserMode,
    MaximumMode
} MODE;
#endif

/* KPROCESSOR_MODE — used throughout the kernel */
#ifndef _KPROCESSOR_MODE_DEFINED
#define _KPROCESSOR_MODE_DEFINED
typedef CCHAR KPROCESSOR_MODE;
#endif

/* KWAIT_REASON — guarded; ke.h also defines with same guard.           */
#ifndef _KWAIT_REASON_DEFINED
#define _KWAIT_REASON_DEFINED
typedef enum _KWAIT_REASON {
    Executive, FreePage, PageIn, PoolAllocation, DelayExecution,
    Suspended, UserRequest, WrExecutive, WrFreePage, WrPageIn,
    WrPoolAllocation, WrDelayExecution, WrSuspended, WrUserRequest,
    WrEventPair, WrQueue, WrLpcReceive, WrLpcReply, WrVirtualMemory,
    WrPageOut, WrRendezvous, Spare2, Spare3, Spare4, Spare5, Spare6,
    WrKernel, MaximumWaitReason
} KWAIT_REASON;
#endif

/* ------------------------------------------------------------------ */
/* Security types                                                      */
/* ------------------------------------------------------------------ */
typedef PVOID PSECURITY_DESCRIPTOR;
typedef PVOID PACCESS_STATE;
typedef PVOID PSID;
typedef PVOID PACL;

/* SECURITY_QUALITY_OF_SERVICE — used in ntosdef.h SECURITY_CLIENT_CONTEXT */
#ifndef _SECURITY_QUALITY_OF_SERVICE_DEFINED
#define _SECURITY_QUALITY_OF_SERVICE_DEFINED
typedef enum _SECURITY_IMPERSONATION_LEVEL {
    SecurityAnonymous,
    SecurityIdentification,
    SecurityImpersonation,
    SecurityDelegation
} SECURITY_IMPERSONATION_LEVEL, *PSECURITY_IMPERSONATION_LEVEL;

typedef struct _SECURITY_QUALITY_OF_SERVICE {
    ULONG                           Length;
    SECURITY_IMPERSONATION_LEVEL    ImpersonationLevel;
    BOOLEAN                         ContextTrackingMode;
    BOOLEAN                         EffectiveOnly;
} SECURITY_QUALITY_OF_SERVICE, *PSECURITY_QUALITY_OF_SERVICE;
#endif

typedef struct _SECURITY_SUBJECT_CONTEXT {
    PVOID ClientToken;
    ULONG ImpersonationLevel;
    PVOID PrimaryToken;
    PVOID ProcessAuditId;
} SECURITY_SUBJECT_CONTEXT, *PSECURITY_SUBJECT_CONTEXT;

typedef struct _GENERIC_MAPPING {
    ACCESS_MASK GenericRead;
    ACCESS_MASK GenericWrite;
    ACCESS_MASK GenericExecute;
    ACCESS_MASK GenericAll;
} GENERIC_MAPPING, *PGENERIC_MAPPING;

/* ------------------------------------------------------------------ */
/* GUID — 128-bit identifier (used in NTFS, PnP, etc.)                */
/* ------------------------------------------------------------------ */
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {
    ULONG   Data1;
    USHORT  Data2;
    USHORT  Data3;
    UCHAR   Data4[8];
} GUID, *PGUID, *LPGUID;
#endif

/* ------------------------------------------------------------------ */
/* SEH types (used in i386.h for NT_TIB)                               */
/* ------------------------------------------------------------------ */
#ifndef _EXCEPTION_REGISTRATION_RECORD_DEFINED
#define _EXCEPTION_REGISTRATION_RECORD_DEFINED
typedef struct _EXCEPTION_REGISTRATION_RECORD {
    struct _EXCEPTION_REGISTRATION_RECORD *Next;
    PVOID                                  Handler;
} EXCEPTION_REGISTRATION_RECORD, *PEXCEPTION_REGISTRATION_RECORD;
#endif

/* ------------------------------------------------------------------ */
/* NT_TIB — Thread Information Block (used in KPCR on x86)             */
/* ------------------------------------------------------------------ */
#ifndef _NT_TIB_DEFINED
#define _NT_TIB_DEFINED
typedef struct _NT_TIB {
    PEXCEPTION_REGISTRATION_RECORD ExceptionList;
    PVOID                          StackBase;
    PVOID                          StackLimit;
    PVOID                          SubSystemTib;
    union {
        PVOID FiberData;
        ULONG Version;
    };
    PVOID ArbitraryUserPointer;
    struct _NT_TIB *Self;
} NT_TIB, *PNT_TIB;
#endif

/* ------------------------------------------------------------------ */
/* LDT_ENTRY — Local Descriptor Table entry (used in i386.h KPCR)      */
/* ------------------------------------------------------------------ */
#ifndef _LDT_ENTRY_DEFINED
#define _LDT_ENTRY_DEFINED
typedef struct _LDT_ENTRY {
    USHORT  LimitLow;
    USHORT  BaseLow;
    union {
        struct {
            UCHAR   BaseMid;
            UCHAR   Flags1;
            UCHAR   Flags2;
            UCHAR   BaseHi;
        } Bytes;
        struct {
            ULONG   BaseMid     : 8;
            ULONG   Type        : 5;
            ULONG   Dpl         : 2;
            ULONG   Pres        : 1;
            ULONG   LimitHi     : 4;
            ULONG   Sys         : 1;
            ULONG   Reserved_0  : 1;
            ULONG   Default_Big : 1;
            ULONG   Granularity : 1;
            ULONG   BaseHi      : 8;
        } Bits;
    } HighWord;
} LDT_ENTRY, *PLDT_ENTRY;
#endif

/* ------------------------------------------------------------------ */
/* Pool allocation tags                                                */
/* ex.h also defines this enum.  We guard ours; ex.h does not guard   */
/* its definition, so we must ensure our definition is IDENTICAL to    */
/* what ex.h provides.  MSVC C allows identical redefinition.          */
/* ------------------------------------------------------------------ */
#ifndef _POOL_TYPE_DEFINED
#define _POOL_TYPE_DEFINED
typedef enum _POOL_TYPE {
    NonPagedPool,
    PagedPool,
    NonPagedPoolMustSucceed,
    DontUseThisType,
    NonPagedPoolCacheAligned,
    PagedPoolCacheAligned,
    NonPagedPoolCacheAlignedMustS,
    MaxPoolType
} POOL_TYPE;
#endif

/* ------------------------------------------------------------------ */
/* Miscellaneous macros used throughout NT kernel                      */
/* ------------------------------------------------------------------ */
#define ARGUMENT_PRESENT(ArgumentPointer) \
    ((CHAR *)((ULONG_PTR)(ArgumentPointer)) != (CHAR *)(NULL))

#define PAGED_CODE()

#define ASSERT(exp)
#define ASSERTMSG(msg, exp)

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

/* NT page size */
#ifndef PAGE_SIZE
#define PAGE_SIZE   4096
#endif
#ifndef PAGE_SHIFT
#define PAGE_SHIFT  12
#endif

/* ------------------------------------------------------------------ */
/* Architecture-specific stubs                                         */
/* ------------------------------------------------------------------ */
#if defined(__x86_64__) || defined(_M_X64)
#define _AMD64_
#elif defined(__i386__) || defined(_M_IX86)
#define _X86_
#elif defined(__aarch64__)
#define _ARM64_
#elif defined(__mips__)
#define _MIPS_
#elif defined(__alpha__)
#define _ALPHA_
#elif defined(__powerpc__)
#define _PPC_
#endif

/* ------------------------------------------------------------------ */
/* Exception handling                                                  */
/* ------------------------------------------------------------------ */
#ifdef _MSC_VER
  /* On MSVC, the CRT excpt.h defines EXCEPTION_DISPOSITION.
   * We must NOT redefine it.  Include the CRT version
   * (found via system include path, not our nt_compat). */
  #include <excpt.h>
#else
  #ifndef _EXCEPTION_DISPOSITION_DEFINED
  #define _EXCEPTION_DISPOSITION_DEFINED
  typedef enum _EXCEPTION_DISPOSITION {
      ExceptionContinueExecution,
      ExceptionContinueSearch,
      ExceptionNestedException,
      ExceptionCollidedUnwind
  } EXCEPTION_DISPOSITION;
  #endif
#endif

/* ------------------------------------------------------------------ */
/* Exception record (needed by ntrtl.h and SEH)                        */
/* ------------------------------------------------------------------ */
#ifndef _EXCEPTION_RECORD_DEFINED
#define _EXCEPTION_RECORD_DEFINED
#define EXCEPTION_MAXIMUM_PARAMETERS 15
typedef struct _EXCEPTION_RECORD {
    NTSTATUS ExceptionCode;
    ULONG    ExceptionFlags;
    struct _EXCEPTION_RECORD *ExceptionRecord;
    PVOID    ExceptionAddress;
    ULONG    NumberParameters;
    ULONG_PTR ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
} EXCEPTION_RECORD, *PEXCEPTION_RECORD;
#endif

/* ------------------------------------------------------------------ */
/* Processor affinity and priority                                     */
/* ------------------------------------------------------------------ */
typedef ULONG_PTR           KAFFINITY;
typedef KAFFINITY          *PKAFFINITY;
typedef LONG                KPRIORITY;

/* PHYSICAL_ADDRESS — used by HAL, NDIS, SCSI */
typedef LARGE_INTEGER       PHYSICAL_ADDRESS;
typedef PHYSICAL_ADDRESS   *PPHYSICAL_ADDRESS;

/* RESTRICTED_POINTER — used in ntosdef.h for KAPC, KDPC */
#ifndef RESTRICTED_POINTER
  #ifdef _MSC_VER
    #define RESTRICTED_POINTER __restrict
  #elif defined(__GNUC__) || defined(__clang__)
    #define RESTRICTED_POINTER __restrict__
  #else
    #define RESTRICTED_POINTER
  #endif
#endif

/* Kernel constants needed before ke.h */
#ifndef THREAD_WAIT_OBJECTS
#define THREAD_WAIT_OBJECTS 3
#endif
#ifndef DISPATCH_LENGTH
#define DISPATCH_LENGTH 106  /* i386 NORMAL_DISPATCH_LENGTH */
#endif

/* Forward-declare KTRAP_FRAME — defined per-arch in i386.h/alpha.h/mips.h */
struct _KTRAP_FRAME;
typedef struct _KTRAP_FRAME *PKTRAP_FRAME;

/* Forward-declare KPROCESSOR_STATE — defined per-arch */
struct _KPROCESSOR_STATE;
typedef struct _KPROCESSOR_STATE KPROCESSOR_STATE;

/* ------------------------------------------------------------------ */
/* Additional types used by NT4 source tree                            */
/* ------------------------------------------------------------------ */
/* NET_API buffer type */
typedef PVOID LPBYTE;

/* EXPORT macro used in some driver headers */
#ifndef EXPORT
  #ifdef _MSC_VER
    #define EXPORT __declspec(dllexport)
  #else
    #define EXPORT __attribute__((visibility("default")))
  #endif
#endif

/* NODE_TYPE_CODE — used in filesystem drivers */
typedef CSHORT NODE_TYPE_CODE;
typedef NODE_TYPE_CODE *PNODE_TYPE_CODE;

/* ------------------------------------------------------------------ */
/* Base constants used throughout NT4 source tree                      */
/* ------------------------------------------------------------------ */
#ifndef ANYSIZE_ARRAY
#define ANYSIZE_ARRAY 1
#endif

#ifndef NOTHING
#define NOTHING void
typedef NOTHING *PNOTHING;
#endif

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef NO_ERROR
#define NO_ERROR 0L
#endif

#ifndef FAR
#define FAR
#endif

#ifndef NEAR
#define NEAR
#endif

/* ------------------------------------------------------------------ */
/* Win32 user-mode pointer types (needed by SDK headers like           */
/* lmaccess.h that leak into kernel compilation)                       */
/* ------------------------------------------------------------------ */
#ifndef _LPDWORD_DEFINED
#define _LPDWORD_DEFINED
typedef DWORD          *LPDWORD;
#endif

#ifndef _LPVOID_DEFINED
#define _LPVOID_DEFINED
typedef void           *LPVOID;
#endif

#ifndef _LPSTR_DEFINED
#define _LPSTR_DEFINED
typedef char           *LPSTR;
#endif

#ifndef _PULONG_PTR_DEFINED
#define _PULONG_PTR_DEFINED
typedef ULONG_PTR      *PULONG_PTR;
#endif

#ifndef _PLONG_PTR_DEFINED
#define _PLONG_PTR_DEFINED
typedef LONG_PTR       *PLONG_PTR;
#endif

#ifndef _PDWORD_PTR_DEFINED
#define _PDWORD_PTR_DEFINED
typedef DWORD_PTR      *PDWORD_PTR;
#endif

/* PSZ — pointer to zero-terminated string (used in OS/2 subsystem) */
#ifndef _PSZ_DEFINED
#define _PSZ_DEFINED
typedef char           *PSZ;
#endif

/* ------------------------------------------------------------------ */
/* I/O Manager types needed before io.h                                */
/* ------------------------------------------------------------------ */
#ifndef _IO_APC_ROUTINE_DEFINED
#define _IO_APC_ROUTINE_DEFINED
typedef VOID (NTAPI *PIO_APC_ROUTINE)(
    IN PVOID ApcContext,
    IN PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG Reserved);
#endif

/* IO_NETWORK_INCREMENT — used in network drivers */
#ifndef IO_NETWORK_INCREMENT
#define IO_NETWORK_INCREMENT 2
#endif

/* FILE_ATTRIBUTE_NORMAL — used in filesystem code */
#ifndef FILE_ATTRIBUTE_NORMAL
#define FILE_ATTRIBUTE_NORMAL 0x00000080
#endif

/* DIRECTORY_ALL_ACCESS */
#ifndef DIRECTORY_ALL_ACCESS
#define DIRECTORY_ALL_ACCESS (SYNCHRONIZE | 0xF)
#endif

/* ------------------------------------------------------------------ */
/* ARC firmware types (needed by arc.h — 34k+ cascading errors)        */
/* ------------------------------------------------------------------ */
#ifndef _DEVICE_FLAGS_DEFINED
#define _DEVICE_FLAGS_DEFINED
typedef struct _DEVICE_FLAGS {
    ULONG Failed : 1;
    ULONG ReadOnly : 1;
    ULONG Removable : 1;
    ULONG ConsoleIn : 1;
    ULONG ConsoleOut : 1;
    ULONG Input : 1;
    ULONG Output : 1;
} DEVICE_FLAGS, *PDEVICE_FLAGS;
#endif

/* ------------------------------------------------------------------ */
/* KPROFILE_SOURCE — kernel profiling source enum (needed by i386.h)    */
/* ------------------------------------------------------------------ */
#ifndef _KPROFILE_SOURCE_DEFINED
#define _KPROFILE_SOURCE_DEFINED
typedef enum _KPROFILE_SOURCE {
    ProfileTime,
    ProfileAlignmentFixup,
    ProfileTotalIssues,
    ProfilePipelineDry,
    ProfileLoadInstructions,
    ProfilePipelineFrozen,
    ProfileBranchInstructions,
    ProfileTotalNonissues,
    ProfileDcacheMisses,
    ProfileIcacheMisses,
    ProfileCacheMisses,
    ProfileBranchMispredictions,
    ProfileStoreInstructions,
    ProfileFpInstructions,
    ProfileIntegerInstructions,
    Profile2Issue,
    Profile3Issue,
    Profile4Issue,
    ProfileSpecialInstructions,
    ProfileTotalCycles,
    ProfileIcacheIssues,
    ProfileDcacheAccesses,
    ProfileMemoryBarrierCycles,
    ProfileLoadLinkedIssues,
    ProfileMaximum
} KPROFILE_SOURCE;
#endif

/* ------------------------------------------------------------------ */
/* CONTEXT — CPU context structure (forward-declare + minimal x86)      */
/* Needed by i386.h KPROCESSOR_STATE which embeds struct _CONTEXT       */
/* ------------------------------------------------------------------ */
#ifndef _CONTEXT_DEFINED
#define _CONTEXT_DEFINED

/* Context flags */
#define CONTEXT_i386    0x00010000
#define CONTEXT_CONTROL         (CONTEXT_i386 | 0x00000001L)
#define CONTEXT_INTEGER         (CONTEXT_i386 | 0x00000002L)
#define CONTEXT_SEGMENTS        (CONTEXT_i386 | 0x00000004L)
#define CONTEXT_FLOATING_POINT  (CONTEXT_i386 | 0x00000008L)
#define CONTEXT_DEBUG_REGISTERS (CONTEXT_i386 | 0x00000010L)
#define CONTEXT_FULL (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_SEGMENTS)

#define SIZE_OF_80387_REGISTERS 80
#define MAXIMUM_SUPPORTED_EXTENSION 512

typedef struct _FLOATING_SAVE_AREA {
    ULONG ControlWord;
    ULONG StatusWord;
    ULONG TagWord;
    ULONG ErrorOffset;
    ULONG ErrorSelector;
    ULONG DataOffset;
    ULONG DataSelector;
    UCHAR RegisterArea[SIZE_OF_80387_REGISTERS];
    ULONG Cr0NpxState;
} FLOATING_SAVE_AREA, *PFLOATING_SAVE_AREA;

typedef struct _CONTEXT {
    ULONG ContextFlags;
    /* Debug registers */
    ULONG Dr0;
    ULONG Dr1;
    ULONG Dr2;
    ULONG Dr3;
    ULONG Dr6;
    ULONG Dr7;
    /* Floating point */
    FLOATING_SAVE_AREA FloatSave;
    /* Segment registers */
    ULONG SegGs;
    ULONG SegFs;
    ULONG SegEs;
    ULONG SegDs;
    /* Integer registers */
    ULONG Edi;
    ULONG Esi;
    ULONG Ebx;
    ULONG Edx;
    ULONG Ecx;
    ULONG Eax;
    /* Control registers */
    ULONG Ebp;
    ULONG Eip;
    ULONG SegCs;
    ULONG EFlags;
    ULONG Esp;
    ULONG SegSs;
    /* Extended registers */
    UCHAR ExtendedRegisters[MAXIMUM_SUPPORTED_EXTENSION];
} CONTEXT, *PCONTEXT;
#endif /* _CONTEXT_DEFINED */

/* ------------------------------------------------------------------ */
/* Forward declarations for I/O, PS, EX types                          */
/* ------------------------------------------------------------------ */
/* I/O objects — defined in io.h but referenced everywhere */
struct _DEVICE_OBJECT;
typedef struct _DEVICE_OBJECT DEVICE_OBJECT, *PDEVICE_OBJECT;
struct _DRIVER_OBJECT;
typedef struct _DRIVER_OBJECT DRIVER_OBJECT, *PDRIVER_OBJECT;
struct _FILE_OBJECT;
typedef struct _FILE_OBJECT FILE_OBJECT, *PFILE_OBJECT;
struct _IRP;
typedef struct _IRP IRP, *PIRP;

/* BUS_DATA_TYPE — used by video.h and HAL */
#ifndef _BUS_DATA_TYPE_DEFINED
#define _BUS_DATA_TYPE_DEFINED
typedef enum _BUS_DATA_TYPE {
    ConfigurationSpaceUndefined = -1,
    Cmos, EisaConfiguration, Pos, CbusConfiguration,
    PCIConfiguration, VMEConfiguration, NuBusConfiguration,
    PCMCIAConfiguration, MPIConfiguration, MPSAConfiguration,
    PNPISAConfiguration, MaximumBusDataType
} BUS_DATA_TYPE, *PBUS_DATA_TYPE;
#endif

/* SYSTEM_HANDLE_INFORMATION — used by ex.h ExSnapShotHandleTables */
struct _SYSTEM_HANDLE_INFORMATION;
typedef struct _SYSTEM_HANDLE_INFORMATION *PSYSTEM_HANDLE_INFORMATION;
struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO;
typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO *PSYSTEM_HANDLE_TABLE_ENTRY_INFO;
struct _SYSTEM_POOL_INFORMATION;
typedef struct _SYSTEM_POOL_INFORMATION *PSYSTEM_POOL_INFORMATION;

/* PROCESS_WS_WATCH_INFORMATION — used by ps.h */
struct _PROCESS_WS_WATCH_INFORMATION;
typedef struct _PROCESS_WS_WATCH_INFORMATION PROCESS_WS_WATCH_INFORMATION, *PPROCESS_WS_WATCH_INFORMATION;

/* PEB — Process Environment Block, used by ps.h EPROCESS */
struct _PEB;
typedef struct _PEB PEB, *PPEB;

/* ------------------------------------------------------------------ */
/* Additional NTSTATUS codes used in NT4 source tree                    */
/* ------------------------------------------------------------------ */
#ifndef STATUS_FS_DRIVER_REQUIRED
#define STATUS_FS_DRIVER_REQUIRED       ((NTSTATUS)0xC000019CL)
#endif
#ifndef STATUS_END_OF_FILE
#define STATUS_END_OF_FILE              ((NTSTATUS)0xC0000011L)
#endif
#ifndef STATUS_INTEGER_OVERFLOW
#define STATUS_INTEGER_OVERFLOW         ((NTSTATUS)0xC0000095L)
#endif
#ifndef STATUS_UNRECOGNIZED_VOLUME
#define STATUS_UNRECOGNIZED_VOLUME      ((NTSTATUS)0xC000014FL)
#endif
#ifndef STATUS_REQUEST_NOT_ACCEPTED
#define STATUS_REQUEST_NOT_ACCEPTED     ((NTSTATUS)0xC00000D0L)
#endif

/* NETBIOS_NAME_SIZE — used in NBT/NetBIOS drivers */
#ifndef NETBIOS_NAME_SIZE
#define NETBIOS_NAME_SIZE 16
#endif

/* ------------------------------------------------------------------ */
/* Structured Exception Handling (SEH) macros                          */
/* NT4 source uses try/except/finally; MSVC C requires __try/__except  */
/* Only in C mode — C++ has its own try/catch keywords.                */
/* ------------------------------------------------------------------ */
#ifndef __cplusplus
#ifdef _MSC_VER
  #ifndef try
    #define try         __try
  #endif
  #ifndef except
    #define except      __except
  #endif
  #ifndef finally
    #define finally     __finally
  #endif
  #ifndef leave
    #define leave       __leave
  #endif
#else
  /* Non-MSVC: SEH is not available; provide no-op stubs */
  #ifndef try
    #define try         if (1)
  #endif
  #ifndef except
    #define except(x)   if (0)
  #endif
  #ifndef finally
    #define finally     if (1)
  #endif
  #ifndef leave
    #define leave       break
  #endif
#endif
#endif /* !__cplusplus */

/* GetExceptionCode / GetExceptionInformation — used in SEH handlers */
#ifndef __cplusplus
#ifdef _MSC_VER
  #ifndef GetExceptionCode
    #define GetExceptionCode()        _exception_code()
  #endif
  #ifndef GetExceptionInformation
    #define GetExceptionInformation()  _exception_info()
  #endif
#else
  #ifndef GetExceptionCode
    #define GetExceptionCode()        0
  #endif
  #ifndef GetExceptionInformation
    #define GetExceptionInformation()  ((void*)0)
  #endif
#endif
#endif /* !__cplusplus */

/* ------------------------------------------------------------------ */
/* FAST_MUTEX — executive fast mutex (needed by fsrtl.h, ex.h)          */
/* The canonical definition uses PKTHREAD and KEVENT, but those are     */
/* complex kernel types.  We provide an opaque-compatible version that  */
/* preserves the struct layout without pulling in ke.h/ps.h.            */
/* ------------------------------------------------------------------ */
#ifndef _FAST_MUTEX_DEFINED
#define _FAST_MUTEX_DEFINED
typedef struct _FAST_MUTEX {
    LONG    Count;
    PVOID   Owner;          /* PKTHREAD — opaque pointer */
    ULONG   Contention;
    PVOID   Event;          /* KEVENT — opaque for layout compat */
    ULONG   OldIrql;
} FAST_MUTEX, *PFAST_MUTEX;
#endif

/* ------------------------------------------------------------------ */
/* ERESOURCE_THREAD — executive resource thread identifier              */
/* ------------------------------------------------------------------ */
#ifndef _ERESOURCE_THREAD_DEFINED
#define _ERESOURCE_THREAD_DEFINED
typedef ULONG_PTR ERESOURCE_THREAD;
typedef ERESOURCE_THREAD *PERESOURCE_THREAD;
#endif

/* ------------------------------------------------------------------ */
/* SHARE_ACCESS — I/O manager share access tracking                     */
/* ------------------------------------------------------------------ */
#ifndef _SHARE_ACCESS_DEFINED
#define _SHARE_ACCESS_DEFINED
typedef struct _SHARE_ACCESS {
    ULONG OpenCount;
    ULONG Readers;
    ULONG Writers;
    ULONG Deleters;
    ULONG SharedRead;
    ULONG SharedWrite;
    ULONG SharedDelete;
} SHARE_ACCESS, *PSHARE_ACCESS;
#endif

/* ------------------------------------------------------------------ */
/* KDEVICE_QUEUE / KDEVICE_QUEUE_ENTRY — kernel device queue            */
/* ------------------------------------------------------------------ */
#ifndef _KDEVICE_QUEUE_DEFINED
#define _KDEVICE_QUEUE_DEFINED
typedef struct _KDEVICE_QUEUE_ENTRY {
    LIST_ENTRY  DeviceListEntry;
    ULONG       SortKey;
    BOOLEAN     Inserted;
} KDEVICE_QUEUE_ENTRY, *PKDEVICE_QUEUE_ENTRY;

typedef struct _KDEVICE_QUEUE {
    SHORT       Type;
    SHORT       Size;
    LIST_ENTRY  DeviceListHead;
    KSPIN_LOCK  Lock;
    BOOLEAN     Busy;
} KDEVICE_QUEUE, *PKDEVICE_QUEUE;
#endif

/* ------------------------------------------------------------------ */
/* WAIT_CONTEXT_BLOCK — I/O manager wait context                        */
/* ------------------------------------------------------------------ */
#ifndef _WAIT_CONTEXT_BLOCK_DEFINED
#define _WAIT_CONTEXT_BLOCK_DEFINED
typedef NTSTATUS (NTAPI *PDRIVER_CONTROL)(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp,
    IN PVOID MapRegisterBase,
    IN PVOID Context);
typedef struct _WAIT_CONTEXT_BLOCK {
    KDEVICE_QUEUE_ENTRY WaitQueueEntry;
    PDRIVER_CONTROL     DeviceRoutine;
    PVOID               DeviceContext;
    ULONG               NumberOfMapRegisters;
    PVOID               DeviceObject;
    PVOID               CurrentIrp;
    PVOID               BufferChainingDpc;  /* PKDPC */
} WAIT_CONTEXT_BLOCK, *PWAIT_CONTEXT_BLOCK;
#endif

/* ------------------------------------------------------------------ */
/* WORK_QUEUE_ITEM — executive work queue item                          */
/* ------------------------------------------------------------------ */
#ifndef _WORK_QUEUE_ITEM_DEFINED
#define _WORK_QUEUE_ITEM_DEFINED
typedef VOID (NTAPI *PWORKER_THREAD_ROUTINE)(IN PVOID Parameter);
typedef struct _WORK_QUEUE_ITEM {
    LIST_ENTRY              List;
    PWORKER_THREAD_ROUTINE  WorkerRoutine;
    PVOID                   Parameter;
} WORK_QUEUE_ITEM, *PWORK_QUEUE_ITEM;
#endif

/* ------------------------------------------------------------------ */
/* SECURITY_DESCRIPTOR — minimal layout for kernel use                  */
/* ------------------------------------------------------------------ */
#ifndef _SECURITY_DESCRIPTOR_STRUCT_DEFINED
#define _SECURITY_DESCRIPTOR_STRUCT_DEFINED
typedef struct _SECURITY_DESCRIPTOR_STRUCT {
    UCHAR   Revision;
    UCHAR   Sbz1;
    USHORT  Control;
    PVOID   Owner;      /* PSID */
    PVOID   Group;      /* PSID */
    PVOID   Sacl;       /* PACL */
    PVOID   Dacl;       /* PACL */
} SECURITY_DESCRIPTOR_STRUCT;
#endif

/* SECURITY_CLIENT_CONTEXT — opaque for kernel drivers */
#ifndef _SECURITY_CLIENT_CONTEXT_DEFINED
#define _SECURITY_CLIENT_CONTEXT_DEFINED
typedef struct _SECURITY_CLIENT_CONTEXT {
    SECURITY_QUALITY_OF_SERVICE SecurityQos;
    PVOID                       ClientToken;
    BOOLEAN                     DirectlyAccessClientToken;
    BOOLEAN                     DirectAccessEffectiveOnly;
    BOOLEAN                     ServerIsRemote;
    PVOID                       ClientTokenControl; /* TOKEN_CONTROL */
} SECURITY_CLIENT_CONTEXT, *PSECURITY_CLIENT_CONTEXT;
#endif

/* ------------------------------------------------------------------ */
/* POWER_STATE — kernel power management                                */
/* ------------------------------------------------------------------ */
#ifndef _POWER_STATE_DEFINED
#define _POWER_STATE_DEFINED
typedef enum _DEVICE_POWER_STATE {
    PowerDeviceUnspecified = 0,
    PowerDeviceD0, PowerDeviceD1, PowerDeviceD2, PowerDeviceD3,
    PowerDeviceMaximum
} DEVICE_POWER_STATE, *PDEVICE_POWER_STATE;
typedef enum _SYSTEM_POWER_STATE {
    PowerSystemUnspecified = 0,
    PowerSystemWorking, PowerSystemSleeping1, PowerSystemSleeping2,
    PowerSystemSleeping3, PowerSystemHibernate, PowerSystemShutdown,
    PowerSystemMaximum
} SYSTEM_POWER_STATE, *PSYSTEM_POWER_STATE;
typedef union _POWER_STATE {
    SYSTEM_POWER_STATE SystemState;
    DEVICE_POWER_STATE DeviceState;
} POWER_STATE, *PPOWER_STATE;
#endif

/* ------------------------------------------------------------------ */
/* RTL types used in NT4 source tree                                    */
/* ------------------------------------------------------------------ */
#ifndef _RTL_TYPES_DEFINED
#define _RTL_TYPES_DEFINED
typedef USHORT RTL_ATOM;
typedef RTL_ATOM *PRTL_ATOM;
typedef struct _RTL_HANDLE_TABLE {
    ULONG MaximumNumberOfHandles;
    ULONG SizeOfHandleTableEntry;
    ULONG Reserved[2];
    PVOID FreeHandles;
    PVOID CommittedHandles;
} RTL_HANDLE_TABLE, *PRTL_HANDLE_TABLE;

typedef enum _RTL_RXACT_OPERATION {
    RtlRXactOperationDelete = 1,
    RtlRXactOperationSetValue,
    RtlRXactOperationAbort
} RTL_RXACT_OPERATION, *PRTL_RXACT_OPERATION;

typedef struct _RTL_QUERY_REGISTRY_TABLE {
    PVOID   QueryRoutine;
    ULONG   Flags;
    PWSTR   Name;
    PVOID   EntryContext;
    ULONG   DefaultType;
    PVOID   DefaultData;
    ULONG   DefaultLength;
} RTL_QUERY_REGISTRY_TABLE, *PRTL_QUERY_REGISTRY_TABLE;
#endif

/* ------------------------------------------------------------------ */
/* EMULATOR_ACCESS_ENTRY — used by video miniport drivers                */
/* ------------------------------------------------------------------ */
#ifndef _EMULATOR_ACCESS_ENTRY_DEFINED
#define _EMULATOR_ACCESS_ENTRY_DEFINED
typedef struct _EMULATOR_ACCESS_ENTRY {
    ULONG   BasePort;
    ULONG   NumConsecutivePorts;
    PVOID   AccessRoutine;
    UCHAR   AccessMode;
    UCHAR   StringSupport;
} EMULATOR_ACCESS_ENTRY, *PEMULATOR_ACCESS_ENTRY;
#endif

/* ------------------------------------------------------------------ */
/* QUAD — 64-bit alignment type used in NT4 pool allocator              */
/* ------------------------------------------------------------------ */
#ifndef _QUAD_DEFINED
#define _QUAD_DEFINED
typedef union _QUAD {
    double  DoNotUseThisField;
    LONGLONG UseThisFieldToCopy;
} QUAD, *PQUAD;
#endif

/* ------------------------------------------------------------------ */
/* WBOOLEAN — wide boolean (ULONG-sized) used in some NT4 drivers       */
/* ------------------------------------------------------------------ */
#ifndef _WBOOLEAN_DEFINED
#define _WBOOLEAN_DEFINED
typedef ULONG WBOOLEAN;
#endif

/* ------------------------------------------------------------------ */
/* CLONG — counted LONG used in some NT4 structures                     */
/* ------------------------------------------------------------------ */
#ifndef _CLONG_DEFINED
#define _CLONG_DEFINED
typedef LONG CLONG;
typedef CLONG *PCLONG;
#endif

/* ------------------------------------------------------------------ */
/* LPSECURITY_ATTRIBUTES — Win32 type that leaks into kernel headers     */
/* ------------------------------------------------------------------ */
#ifndef _LPSECURITY_ATTRIBUTES_DEFINED
#define _LPSECURITY_ATTRIBUTES_DEFINED
typedef struct _SECURITY_ATTRIBUTES {
    ULONG   nLength;
    PVOID   lpSecurityDescriptor;
    BOOLEAN bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;
#endif

/* ------------------------------------------------------------------ */
/* UNICODE_PREFIX_TABLE — used by MUP and redirector drivers             */
/* ------------------------------------------------------------------ */
#ifndef _UNICODE_PREFIX_TABLE_DEFINED
#define _UNICODE_PREFIX_TABLE_DEFINED
typedef struct _UNICODE_PREFIX_TABLE_ENTRY {
    CSHORT          NodeTypeCode;
    CSHORT          NameLength;
    struct _UNICODE_PREFIX_TABLE_ENTRY *NextPrefixTree;
    struct _UNICODE_PREFIX_TABLE_ENTRY *CaseMatch;
    PVOID           Links[3];   /* RTL_SPLAY_LINKS analog */
    PUNICODE_STRING Prefix;
} UNICODE_PREFIX_TABLE_ENTRY, *PUNICODE_PREFIX_TABLE_ENTRY;

typedef struct _UNICODE_PREFIX_TABLE {
    CSHORT                          NodeTypeCode;
    CSHORT                          NameLength;
    PUNICODE_PREFIX_TABLE_ENTRY     NextPrefixTree;
    PUNICODE_PREFIX_TABLE_ENTRY     LastNextEntry;
} UNICODE_PREFIX_TABLE, *PUNICODE_PREFIX_TABLE;
#endif

/* ------------------------------------------------------------------ */
/* PREFIX_TABLE_ENTRY — used by name prefix resolution                   */
/* ------------------------------------------------------------------ */
#ifndef _PREFIX_TABLE_ENTRY_DEFINED
#define _PREFIX_TABLE_ENTRY_DEFINED
typedef struct _PREFIX_TABLE_ENTRY {
    CSHORT          NodeTypeCode;
    CSHORT          NameLength;
    struct _PREFIX_TABLE_ENTRY *NextPrefixTree;
    PVOID           Links[3];   /* RTL_SPLAY_LINKS analog */
    PSTRING         Prefix;
} PREFIX_TABLE_ENTRY, *PPREFIX_TABLE_ENTRY;
#endif

/* ------------------------------------------------------------------ */
/* HEAP types — used by NT4 heap manager                                */
/* ------------------------------------------------------------------ */
#ifndef _HEAP_ENTRY_DEFINED
#define _HEAP_ENTRY_DEFINED
typedef struct _HEAP_ENTRY {
    USHORT  Size;
    USHORT  PreviousSize;
    UCHAR   SmallTagIndex;
    UCHAR   Flags;
    UCHAR   UnusedBytes;
    UCHAR   SegmentIndex;
} HEAP_ENTRY, *PHEAP_ENTRY;

typedef struct _HEAP_ENTRY_EXTRA {
    union {
        struct {
            USHORT  AllocatorBackTraceIndex;
            USHORT  TagIndex;
            ULONG_PTR Settable;
        };
        ULONGLONG ZeroInit;
    };
} HEAP_ENTRY_EXTRA, *PHEAP_ENTRY_EXTRA;
#endif

/* VER_PRODUCTBUILD — NT4 build number */
#ifndef VER_PRODUCTBUILD
#define VER_PRODUCTBUILD 1381
#endif

#ifdef __cplusplus
}
#endif

#endif /* _NTDEF_H_COMPAT_ */
