/* Security Opaque — internal security types */
#ifndef _SEOPAQUE_H_COMPAT
#define _SEOPAQUE_H_COMPAT
#include "ntdef.h"
typedef struct _SEP_LOGON_SESSION_REFERENCES { struct _SEP_LOGON_SESSION_REFERENCES *Next; LUID LogonId; ULONG ReferenceCount; ULONG Flags; } SEP_LOGON_SESSION_REFERENCES, *PSEP_LOGON_SESSION_REFERENCES;
#endif
