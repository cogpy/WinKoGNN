/*++
 * windef.h — Windows Definitions (Cross-Platform Compatibility Shim)
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
typedef HANDLE  HMODULE;
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

typedef struct tagPOINT {
    LONG x;
    LONG y;
} POINT, *PPOINT, *LPPOINT;

typedef struct tagSIZE {
    LONG cx;
    LONG cy;
} SIZE, *PSIZE, *LPSIZE;

typedef struct tagRECT {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT, *PRECT, *LPRECT;

#define MAKEWORD(a, b)  ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b)  ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)       ((WORD)(l))
#define HIWORD(l)       ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)       ((BYTE)(w))
#define HIBYTE(w)       ((BYTE)(((WORD)(w) >> 8) & 0xFF))

#define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

#endif /* _WINDEF_H_COMPAT_ */
