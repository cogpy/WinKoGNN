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
typedef union _LARGE_INTEGER {
    struct {
        ULONG LowPart;
        LONG  HighPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;
#endif

#ifndef _ULARGE_INTEGER_DEFINED
#define _ULARGE_INTEGER_DEFINED
typedef union _ULARGE_INTEGER {
    struct {
        ULONG LowPart;
        ULONG HighPart;
    } u;
    ULONGLONG QuadPart;
} ULARGE_INTEGER, *PULARGE_INTEGER;
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
/* Dispatcher objects                                                  */
/* ------------------------------------------------------------------ */
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

#ifndef _KEVENT_DEFINED
#define _KEVENT_DEFINED
typedef struct _KEVENT {
    DISPATCHER_HEADER Header;
} KEVENT, *PKEVENT, *PRKEVENT;
#endif

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

#ifndef _KSEMAPHORE_DEFINED
#define _KSEMAPHORE_DEFINED
typedef struct _KSEMAPHORE {
    DISPATCHER_HEADER Header;
    LONG              Limit;
} KSEMAPHORE, *PKSEMAPHORE, *PRKSEMAPHORE;
#endif

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

typedef enum _KWAIT_REASON {
    Executive, FreePage, PageIn, PoolAllocation, DelayExecution,
    Suspended, UserRequest, WrExecutive, WrFreePage, WrPageIn,
    WrPoolAllocation, WrDelayExecution, WrSuspended, WrUserRequest,
    WrQueue, WrLpcReceive, WrLpcReply, WrVirtualMemory, WrPageOut,
    WrRendezvous, MaximumWaitReason
} KWAIT_REASON;

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
/* ------------------------------------------------------------------ */
#ifndef _POOL_TYPE_DEFINED
#define _POOL_TYPE_DEFINED
typedef enum _POOL_TYPE {
    NonPagedPool, PagedPool, NonPagedPoolMustSucceed, DontUseThisType,
    NonPagedPoolCacheAligned, PagedPoolCacheAligned,
    NonPagedPoolCacheAlignedMustS, MaxPoolType
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

#ifdef __cplusplus
}
#endif

#endif /* _NTDEF_H_COMPAT_ */
