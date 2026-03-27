/*++ BUILD Version: 0001
 * ntddk.h — NT Device Driver Kit Compatibility Shim
 *
 * Cross-platform stub for the Windows DDK header.
 * Provides driver model types, IRQL, device objects, and IRP stubs.
 * Types already defined in ntdef.h / nt.h / ntrtl.h are NOT redefined here.
 *--*/
#ifndef _NTDDK_H_
#define _NTDDK_H_

#include "nt.h"
#include "ntrtl.h"
#include "ntstatus.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── IRQL levels ── */
#ifndef PASSIVE_LEVEL
#define PASSIVE_LEVEL   0
#define APC_LEVEL       1
#define DISPATCH_LEVEL  2
#define PROFILE_LEVEL   27
#define HIGH_LEVEL      31
#endif

/* ── Device types ── */
#define FILE_DEVICE_UNKNOWN         0x00000022
#define FILE_DEVICE_NETWORK         0x00000012
#define FILE_DEVICE_DISK            0x00000007
#define FILE_DEVICE_KEYBOARD        0x0000000B
#define FILE_DEVICE_MOUSE           0x0000000F
#define FILE_DEVICE_VIDEO           0x00000023
#define FILE_DEVICE_TRANSPORT       0x00000021

/* ── I/O method codes ── */
#define METHOD_BUFFERED             0
#define METHOD_IN_DIRECT            1
#define METHOD_OUT_DIRECT           2
#define METHOD_NEITHER              3

/* ── File access ── */
#define FILE_ANY_ACCESS             0
#define FILE_READ_ACCESS            0x0001
#define FILE_WRITE_ACCESS           0x0002

/* ── CTL_CODE macro ── */
#ifndef CTL_CODE
#define CTL_CODE(DeviceType, Function, Method, Access) \
    (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))
#endif

/* ── Device Object (DDK-specific, not in ntdef.h) ── */
#ifndef _DEVICE_OBJECT_DEFINED
#define _DEVICE_OBJECT_DEFINED
typedef struct _DEVICE_OBJECT {
    SHORT               Type;
    USHORT              Size;
    LONG                ReferenceCount;
    struct _DRIVER_OBJECT *DriverObject;
    struct _DEVICE_OBJECT *NextDevice;
    struct _DEVICE_OBJECT *AttachedDevice;
    ULONG               Flags;
    ULONG               Characteristics;
    PVOID               DeviceExtension;
    ULONG               DeviceType;
    CHAR                StackSize;
    ULONG               AlignmentRequirement;
} DEVICE_OBJECT, *PDEVICE_OBJECT;
#endif

/* ── Driver Object ── */
#ifndef _DRIVER_OBJECT_DEFINED
#define _DRIVER_OBJECT_DEFINED
typedef struct _DRIVER_OBJECT {
    SHORT               Type;
    SHORT               Size;
    PDEVICE_OBJECT      DeviceObject;
    ULONG               Flags;
    PVOID               DriverStart;
    ULONG               DriverSize;
    PVOID               DriverSection;
    PVOID               DriverExtension;
    UNICODE_STRING      DriverName;
    PUNICODE_STRING     HardwareDatabase;
    PVOID               FastIoDispatch;
    PVOID               DriverInit;
    PVOID               DriverStartIo;
    PVOID               DriverUnload;
    PVOID               MajorFunction[28];
} DRIVER_OBJECT, *PDRIVER_OBJECT;
#endif

/* ── IRP (I/O Request Packet) — uses IO_STATUS_BLOCK from ntdef.h ── */
#ifndef _IRP_DEFINED
#define _IRP_DEFINED
typedef struct _IRP {
    SHORT               Type;
    USHORT              Size;
    PVOID               MdlAddress;
    ULONG               Flags;
    union {
        struct _IRP    *MasterIrp;
        PVOID           SystemBuffer;
    } AssociatedIrp;
    IO_STATUS_BLOCK     IoStatus;
    CHAR                RequestorMode;
    BOOLEAN             PendingReturned;
    BOOLEAN             Cancel;
    KIRQL               CancelIrql;
    PVOID               UserBuffer;
    PVOID               Tail;
} IRP, *PIRP;
#endif

/* ── Common DDK macros ── */
#define IoGetCurrentIrpStackLocation(Irp) ((PVOID)(Irp))
#define IoSkipCurrentIrpStackLocation(Irp) ((void)(Irp))
#define IoCallDriver(DeviceObject, Irp) (STATUS_SUCCESS)
#define IoCompleteRequest(Irp, PriorityBoost) ((void)(Irp))
#define IO_NO_INCREMENT 0

/* ── DDK function stubs ── */
static inline NTSTATUS IoCreateDevice(
    PDRIVER_OBJECT DriverObject, ULONG DeviceExtensionSize,
    PUNICODE_STRING DeviceName, ULONG DeviceType_,
    ULONG DeviceCharacteristics, BOOLEAN Exclusive,
    PDEVICE_OBJECT *DeviceObject)
{
    (void)DriverObject; (void)DeviceExtensionSize; (void)DeviceName;
    (void)DeviceType_; (void)DeviceCharacteristics; (void)Exclusive;
    *DeviceObject = NULL;
    return STATUS_SUCCESS;
}

static inline void IoDeleteDevice(PDEVICE_OBJECT DeviceObject)
{
    (void)DeviceObject;
}

#ifndef _KEINITIALIZEDPC_DEFINED
#define _KEINITIALIZEDPC_DEFINED
static inline void KeInitializeDpc(PKDPC Dpc, PVOID DeferredRoutine, PVOID DeferredContext)
{
    (void)Dpc; (void)DeferredRoutine; (void)DeferredContext;
}
#endif

/* KeInitializeTimer already defined in nt.h */

#ifdef __cplusplus
}
#endif

#endif /* _NTDDK_H_ */
