/*++
 * windef.h — Windows Definitions (Cross-Platform Compatibility Shim)
 *
 * This header provides GDI/User handle types and utility macros needed
 * by the NT4 w32 subsystem (video miniport, GDI drivers).
 *
 * All base types (ULONG, LONG, HANDLE, etc.) come from ntdef.h which
 * MUST be included first.  On all platforms (including _WIN32), we
 * define these types from our own ntdef.h base — we do NOT include
 * the real Windows SDK windef.h because our nt_compat/ directory
 * intercepts the name.
 *--*/

#ifndef _WINDEF_H_COMPAT_
#define _WINDEF_H_COMPAT_

#include "ntdef.h"

typedef ULONG   UINT;
typedef LONG    INT;
typedef LONG    BOOL;
typedef WORD    ATOM;
typedef HANDLE  HWND;
typedef HANDLE  HDC;
typedef HANDLE  HINSTANCE;
#ifndef _HMODULE_DEFINED
#define _HMODULE_DEFINED
typedef HANDLE  HMODULE;
#endif
typedef HANDLE  HBITMAP;
typedef HANDLE  HBRUSH;
typedef HANDLE  HCURSOR;
typedef HANDLE  HFONT;
typedef HANDLE  HICON;
typedef HANDLE  HMENU;
typedef HANDLE  HPEN;
typedef HANDLE  HRGN;
typedef HANDLE  HPALETTE;
typedef HANDLE  HGLOBAL;
typedef HANDLE  HLOCAL;
typedef HANDLE  HKEY;
typedef ULONG   COLORREF;
typedef LONG    HRESULT;
typedef LONG    LRESULT;
typedef ULONG_PTR WPARAM;
typedef LONG_PTR  LPARAM;

typedef struct tagPOINT { LONG x; LONG y; } POINT, *PPOINT, *LPPOINT;
typedef struct tagSIZE  { LONG cx; LONG cy; } SIZE, *PSIZE, *LPSIZE;
typedef struct tagRECT  { LONG left; LONG top; LONG right; LONG bottom; } RECT, *PRECT, *LPRECT;

#ifndef MAKEWORD
#define MAKEWORD(a, b)  ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#endif
#ifndef MAKELONG
#define MAKELONG(a, b)  ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#endif
#ifndef LOWORD
#define LOWORD(l)       ((WORD)(l))
#endif
#ifndef HIWORD
#define HIWORD(l)       ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#endif
#ifndef LOBYTE
#define LOBYTE(w)       ((BYTE)(w))
#endif
#ifndef HIBYTE
#define HIBYTE(w)       ((BYTE)(((WORD)(w) >> 8) & 0xFF))
#endif
#ifndef RGB
#define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#endif

#endif /* _WINDEF_H_COMPAT_ */
