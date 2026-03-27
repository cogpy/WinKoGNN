/*++ BUILD Version: 0001
 * dderror.h — DirectDraw Error Codes Compatibility Shim
 *
 * Used by video miniport drivers (weitekp9, xga, etc.)
 *--*/
#ifndef _DDERROR_H_
#define _DDERROR_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DD_OK
#define DD_OK                           0x00000000L
#endif

#define DDERR_GENERIC                   0x80004005L
#define DDERR_UNSUPPORTED               0x80004001L
#define DDERR_INVALIDPARAMS             0x80070057L
#define DDERR_OUTOFMEMORY               0x8007000EL
#define DDERR_SURFACELOST               0x887601C2L
#define DDERR_WASSTILLDRAWING           0x8876021CL
#define DDERR_INVALIDRECT               0x88760228L
#define DDERR_INVALIDCAPS               0x88760230L
#define DDERR_NOTFOUND                  0x88760236L
#define DDERR_EXCEPTION                 0x88760243L
#define DDERR_CURRENTLYNOTAVAIL         0x88760258L
#define DDERR_WRONGMODE                 0x88760274L
#define DDERR_NOEXCLUSIVEMODE           0x88760280L
#define DDERR_DCALREADYCREATED          0x887602A0L
#define DDERR_NOTFLIPPABLE              0x887602C0L
#define DDERR_CANTDUPLICATE             0x887602D0L
#define DDERR_NOTLOCKED                 0x887602E0L
#define DDERR_CANTCREATEDC              0x887602F0L
#define DDERR_NODC                      0x88760300L
#define DDERR_INVALIDSURFACETYPE        0x88760310L

/* ── Video port errors ── */
#define NO_ERROR                        0L
#define ERROR_INVALID_FUNCTION          1L
#define ERROR_NOT_ENOUGH_MEMORY         8L
#define ERROR_DEV_NOT_EXIST             55L
#define ERROR_INSUFFICIENT_BUFFER       122L
#define ERROR_MORE_DATA                 234L

/* ── Video port status ── */
#ifndef VP_STATUS
typedef LONG VP_STATUS;
#define NO_ERROR_VP                     0L
#define ERROR_INVALID_PARAMETER_VP      87L
#endif

#ifdef __cplusplus
}
#endif

#endif /* _DDERROR_H_ */
