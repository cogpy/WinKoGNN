/* Restore default packing */
#ifndef _PACKOFF_H_COMPAT
#define _PACKOFF_H_COMPAT
#ifdef _MSC_VER
#pragma pack(pop)
#elif defined(__GNUC__) || defined(__clang__)
#pragma pack(pop)
#endif
#endif
