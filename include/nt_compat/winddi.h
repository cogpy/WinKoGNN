/*
 * winddi.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 203 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_WINDDI_H_H_
#define _NT_COMPAT_WINDDI_H_H_

#include "ntdef.h"
#include "windef.h"
#include <stdarg.h>

#ifndef _LPSTR_DEFINED
#define _LPSTR_DEFINED
typedef char *LPSTR;
#endif

#ifndef _INT_DEFINED
#define _INT_DEFINED
typedef int INT;
#endif

#ifndef _UINT_DEFINED
#define _UINT_DEFINED
typedef unsigned int UINT;
#endif
#ifdef __cplusplus
extern "C" {
#endif


/* Windows GDI Display Driver Interface — types for video/display miniport drivers */

#ifndef _WINDDI_H_COMPAT
#define _WINDDI_H_COMPAT

typedef LONG FLOATL;
typedef LONG FIX;
typedef ULONG FLONG;
typedef ULONG IDENT;
typedef ULONG HFF;
typedef ULONG HFC;
typedef ULONG HGLYPH;
typedef PVOID DHPDEV;
typedef PVOID DHSURF;
typedef HANDLE HDEV;
typedef HANDLE HSURF;
typedef HANDLE HSEMAPHORE;

typedef struct _POINTL { LONG x; LONG y; } POINTL, *PPOINTL;
typedef struct _RECTL { LONG left; LONG top; LONG right; LONG bottom; } RECTL, *PRECTL;
typedef struct _SIZEL { LONG cx; LONG cy; } SIZEL, *PSIZEL;

#ifndef _POINTFIX_DEFINED
#define _POINTFIX_DEFINED
typedef struct _POINTFIX { FIX x; FIX y; } POINTFIX, *PPOINTFIX;
#endif

typedef struct _CLIPOBJ { ULONG iUniq; RECTL rclBounds; BYTE iDComplexity; BYTE iFComplexity; BYTE iMode; BYTE fjOptions; } CLIPOBJ;
typedef struct _SURFOBJ { DHSURF dhsurf; HSURF hsurf; DHPDEV dhpdev; HDEV hdev; SIZEL sizlBitmap; ULONG cjBits; PVOID pvBits; PVOID pvScan0; LONG lDelta; ULONG iUniq; ULONG iBitmapFormat; USHORT iType; USHORT fjBitmap; } SURFOBJ;
typedef struct _BRUSHOBJ { ULONG iSolidColor; PVOID pvRbrush; FLONG flColorType; } BRUSHOBJ;
typedef struct _XLATEOBJ { ULONG iUniq; FLONG flXlate; USHORT iSrcType; USHORT iDstType; ULONG cEntries; PULONG pulXlate; } XLATEOBJ;
typedef struct _PALOBJ { ULONG ulReserved; } PALOBJ;
typedef struct _PATHOBJ { FLONG fl; ULONG cCurves; } PATHOBJ;
typedef struct _STROBJ { ULONG cGlyphs; FLONG flAccel; ULONG ulCharInc; RECTL rclBkGround; PVOID pgp; LPWSTR pwszOrg; } STROBJ;
typedef struct _FONTOBJ { ULONG iUniq; ULONG iFace; ULONG cxMax; FLONG flFontType; ULONG_PTR iTTUniq; ULONG_PTR iFile; SIZEL sizLogResPpi; ULONG ulStyleSize; PVOID pvConsumer; PVOID pvProducer; } FONTOBJ;
typedef struct _DEVINFO { FLONG flGraphicsCaps; ULONG lfDefaultFont; ULONG lfAnsiVarFont; ULONG lfAnsiFixFont; ULONG cFonts; ULONG iDitherFormat; USHORT cxDither; USHORT cyDither; ULONG hpalDefault; FLONG flGraphicsCaps2; } DEVINFO;
typedef struct _GDIINFO { ULONG ulVersion; ULONG ulTechnology; ULONG ulHorzSize; ULONG ulVertSize; ULONG ulHorzRes; ULONG ulVertRes; ULONG cBitsPixel; ULONG cPlanes; ULONG ulNumColors; ULONG flRaster; ULONG ulLogPixelsX; ULONG ulLogPixelsY; ULONG flTextCaps; ULONG ulDACRed; ULONG ulDACGreen; ULONG ulDACBlue; ULONG ulAspectX; ULONG ulAspectY; ULONG ulAspectXY; LONG xStyleStep; LONG yStyleStep; LONG denStyleStep; POINTL ptlPhysOffset; SIZEL szlPhysSize; ULONG ulNumPalReg; } GDIINFO;

/* DDI function indices */
#define INDEX_DrvEnablePDEV     0
#define INDEX_DrvCompletePDEV   1
#define INDEX_DrvDisablePDEV    2
#define INDEX_DrvEnableSurface  3
#define INDEX_DrvDisableSurface 4

/* Bitmap format */
#define BMF_1BPP    1
#define BMF_4BPP    2
#define BMF_8BPP    3
#define BMF_16BPP   4
#define BMF_24BPP   5
#define BMF_32BPP   6

/* Complexity */
#define DC_TRIVIAL  0
#define DC_RECT     1
#define DC_COMPLEX  3

/* ROP codes */
#define ROP4_FROM_INDEX(i) ((ULONG)(i))

/* Engine callbacks */
ULONG NTAPI EngMultiByteToWideChar(IN UINT CodePage, OUT LPWSTR WideCharString,
    IN INT BytesInWideCharString, IN LPSTR MultiByteString, IN INT BytesInMultiByteString);
VOID NTAPI EngDebugPrint(IN PCHAR StandardPrefix, IN PCHAR DebugMessage, IN va_list ap);

#endif /* _WINDDI_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_WINDDI_H_H_ */
