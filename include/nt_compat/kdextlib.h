/* kdextlib.h — Kernel Debugger Extension Library header stub */
#ifndef _KDEXTLIB_H_COMPAT_
#define _KDEXTLIB_H_COMPAT_
#include "ntdef.h"
typedef VOID (*PWINDBG_OUTPUT_ROUTINE)(PCSTR Format, ...);
typedef ULONG_PTR (*PWINDBG_GET_EXPRESSION)(PCSTR Expression);
typedef VOID (*PWINDBG_GET_SYMBOL)(PVOID Offset, PCHAR Name, PULONG_PTR Displacement);
typedef BOOLEAN (*PWINDBG_READ_PROCESS_MEMORY_ROUTINE)(ULONG_PTR Offset, PVOID Buffer, ULONG Size, PULONG BytesRead);
#endif
