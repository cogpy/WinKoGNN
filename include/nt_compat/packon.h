/* Pack structures on 1-byte boundary */
#ifndef _PACKON_H_COMPAT
#define _PACKON_H_COMPAT
#ifdef _MSC_VER
#pragma pack(push, 1)
#elif defined(__GNUC__) || defined(__clang__)
#pragma pack(push, 1)
#endif
#endif
