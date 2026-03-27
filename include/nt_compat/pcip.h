/* PCI Private — internal PCI bus driver types */
#ifndef _PCIP_H_COMPAT
#define _PCIP_H_COMPAT
#include "ntddk.h"
typedef struct _PCI_SLOT_NUMBER { union { struct { ULONG DeviceNumber:5; ULONG FunctionNumber:3; ULONG Reserved:24; } bits; ULONG AsULONG; } u; } PCI_SLOT_NUMBER, *PPCI_SLOT_NUMBER;
#endif
