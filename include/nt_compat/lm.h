/*
 * lm.h — LAN Manager Master Header Compatibility
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Aggregates LAN Manager sub-headers for the srv subsystem.
 */
#ifndef _NT_COMPAT_LM_H_
#define _NT_COMPAT_LM_H_

/* Prevent the Windows SDK lm.h from being included */
#ifndef _LM_
#define _LM_
#endif

#include "lmaccess.h"

#endif /* _NT_COMPAT_LM_H_ */
