/*++
 * ntdef.h — NT Base Type Definitions (Cross-Platform Compatibility Shim)
 *
 * Part of WinKoGNN Unified AGI-OS Build
 *
 * This header provides the fundamental Windows NT type definitions
 * required by the NT4 kernel source tree (private/ntos/) when building
 * on non-Windows platforms (Linux, macOS, WASM).  It maps NT types to
 * portable C99/C11 equivalents so that the kernel object model, memory
 * manager, process manager, and IPC subsystems can compile as OBJECT
 * libraries for analysis, instrumentation, and cognitive integration.
 *
 * Design principle: provide *just enough* of the NT type system to let
 * the ntos source compile without modification, while remaining a pure
 * C header with zero external dependencies.
 *--*/

#ifndef _NTDEF_H_COMPAT_
#define _NTDEF_H_COMPAT_

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */
/* Detect platform and set up base integer types                       */
/* ------------------------------------------------------------------ */
#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Fundamental scalar types                                            */
/* ------------------------------------------------------------------ */
typedef unsigned char       UCHAR;
typedef unsigned char       BYTE;
typedef unsigned char       BOOLEAN;
typedef char                CHAR;
typedef int16_t             SHORT;
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
typedef BYTE               *PBYTE;
typedef BOOLEAN            *PBOOLEAN;
typedef LONG               *PLONG;
typedef LONGLONG           *PLONGLONG;
typedef ULONGLONG          *PULONGLONG;
typedef const char         *PCSTR;
typedef const char         *LPCSTR;
typedef void                VOID;

/* Pointer-sized types */
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
/* Calling conventions (no-op on non-Windows)                          */
/* ------------------------------------------------------------------ */
#ifndef _WIN32
#define NTAPI
#define WINAPI
#define CALLBACK
#define APIENTRY
#define __stdcall
#define __cdecl
#define __fastcall
#define DECLSPEC_IMPORT
#define DECLSPEC_NORETURN
#define FORCEINLINE         static inline
#define IN
#define OUT
#define OPTIONAL
#define CONST               const
#define UNALIGNED
#endif

/* ------------------------------------------------------------------ */
/* Unicode string types                                                */
/* ------------------------------------------------------------------ */
typedef uint16_t            WCHAR;
typedef WCHAR              *PWCHAR;
typedef WCHAR              *PWSTR;
typedef const WCHAR        *PCWSTR;
typedef const WCHAR        *LPCWSTR;

typedef struct _UNICODE_STRING {
    USHORT  Length;          /* byte count (not including terminator) */
    USHORT  MaximumLength;  /* byte count of buffer allocation       */
    PWSTR   Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef const UNICODE_STRING *PCUNICODE_STRING;

typedef struct _STRING {
    USHORT  Length;
    USHORT  MaximumLength;
    PCHAR   Buffer;
} STRING, *PSTRING, ANSI_STRING, *PANSI_STRING, OEM_STRING, *POEM_STRING;

/* ------------------------------------------------------------------ */
/* LARGE_INTEGER                                                       */
/* ------------------------------------------------------------------ */
typedef union _LARGE_INTEGER {
    struct {
        ULONG LowPart;
        LONG  HighPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

typedef union _ULARGE_INTEGER {
    struct {
        ULONG LowPart;
        ULONG HighPart;
    } u;
    ULONGLONG QuadPart;
} ULARGE_INTEGER, *PULARGE_INTEGER;

/* ------------------------------------------------------------------ */
/* Doubly-linked list (NT kernel fundamental data structure)            */
/* ------------------------------------------------------------------ */
typedef struct _LIST_ENTRY {
    struct _LIST_ENTRY *Flink;
    struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY;

typedef struct _SINGLE_LIST_ENTRY {
    struct _SINGLE_LIST_ENTRY *Next;
} SINGLE_LIST_ENTRY, *PSINGLE_LIST_ENTRY;

/* List manipulation macros */
#define InitializeListHead(ListHead) \
    ((ListHead)->Flink = (ListHead)->Blink = (ListHead))

#define IsListEmpty(ListHead) \
    ((ListHead)->Flink == (ListHead))

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
/* IRQL and synchronization primitives                                 */
/* ------------------------------------------------------------------ */
typedef UCHAR               KIRQL;
typedef KIRQL              *PKIRQL;

#define PASSIVE_LEVEL       0
#define LOW_LEVEL           0
#define APC_LEVEL           1
#define DISPATCH_LEVEL      2
#define PROFILE_LEVEL       27
#define HIGH_LEVEL          31

typedef struct _KSPIN_LOCK {
    volatile ULONG_PTR Lock;
} KSPIN_LOCK, *PKSPIN_LOCK;

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

/* ------------------------------------------------------------------ */
/* Dispatcher objects                                                  */
/* ------------------------------------------------------------------ */
typedef struct _DISPATCHER_HEADER {
    UCHAR       Type;
    UCHAR       Absolute;
    UCHAR       Size;
    UCHAR       Inserted;
    LONG        SignalState;
    LIST_ENTRY  WaitListHead;
} DISPATCHER_HEADER;

typedef struct _KEVENT {
    DISPATCHER_HEADER Header;
} KEVENT, *PKEVENT, *PRKEVENT;

typedef struct _KMUTANT {
    DISPATCHER_HEADER Header;
    LIST_ENTRY        MutantListEntry;
    PVOID             OwnerThread;
    BOOLEAN           Abandoned;
    UCHAR             ApcDisable;
} KMUTANT, *PKMUTANT, *PRKMUTANT, KMUTEX, *PKMUTEX;

typedef struct _KSEMAPHORE {
    DISPATCHER_HEADER Header;
    LONG              Limit;
} KSEMAPHORE, *PKSEMAPHORE, *PRKSEMAPHORE;

typedef struct _KTIMER {
    DISPATCHER_HEADER Header;
    ULARGE_INTEGER    DueTime;
    LIST_ENTRY        TimerListEntry;
    PKDPC             Dpc;
    LONG              Period;
} KTIMER, *PKTIMER, *PRKTIMER;

/* ------------------------------------------------------------------ */
/* Object type forward declarations                                    */
/* ------------------------------------------------------------------ */
typedef struct _OBJECT_TYPE *POBJECT_TYPE;

/* ------------------------------------------------------------------ */
/* Memory descriptor list                                              */
/* ------------------------------------------------------------------ */
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

/* ------------------------------------------------------------------ */
/* Access mask                                                         */
/* ------------------------------------------------------------------ */
typedef ULONG ACCESS_MASK;
typedef ACCESS_MASK *PACCESS_MASK;

#define DELETE                  0x00010000L
#define READ_CONTROL            0x00020000L
#define WRITE_DAC               0x00040000L
#define WRITE_OWNER             0x00080000L
#define SYNCHRONIZE             0x00100000L
#define GENERIC_READ            0x80000000L
#define GENERIC_WRITE           0x40000000L
#define GENERIC_EXECUTE         0x20000000L
#define GENERIC_ALL             0x10000000L

/* ------------------------------------------------------------------ */
/* Processor modes                                                     */
/* ------------------------------------------------------------------ */
typedef enum _MODE {
    KernelMode,
    UserMode,
    MaximumMode
} MODE;

typedef enum _KWAIT_REASON {
    Executive,
    FreePage,
    PageIn,
    PoolAllocation,
    DelayExecution,
    Suspended,
    UserRequest,
    WrExecutive,
    WrFreePage,
    WrPageIn,
    WrPoolAllocation,
    WrDelayExecution,
    WrSuspended,
    WrUserRequest,
    WrQueue,
    WrLpcReceive,
    WrLpcReply,
    WrVirtualMemory,
    WrPageOut,
    WrRendezvous,
    MaximumWaitReason
} KWAIT_REASON;

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
#define PAGE_SHIFT  12

/* ------------------------------------------------------------------ */
/* Pool allocation tags                                                */
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
#endif /* _POOL_TYPE_DEFINED */

/* ------------------------------------------------------------------ */
/* Security types (minimal stubs)                                      */
/* ------------------------------------------------------------------ */
typedef PVOID PSECURITY_DESCRIPTOR;
typedef PVOID PACCESS_STATE;
typedef PVOID PSID;
typedef PVOID PACL;

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

#ifdef __cplusplus
}
#endif

#endif /* _NTDEF_H_COMPAT_ */
