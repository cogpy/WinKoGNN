/*
 * minwindef.h — Minimal Windows Definitions Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Prevents the Windows SDK minwindef.h from being included.
 */
#ifndef _NT_COMPAT_MINWINDEF_H_
#define _NT_COMPAT_MINWINDEF_H_

/* Prevent the Windows SDK minwindef.h from being included */
#ifndef _MINWINDEF_
#define _MINWINDEF_
#endif

#include "ntdef.h"

/* All types already defined in ntdef.h */

#endif /* _NT_COMPAT_MINWINDEF_H_ */
