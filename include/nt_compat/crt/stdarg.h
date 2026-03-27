/*
 * crt/stdarg.h — Redirect to system stdarg.h
 *
 * This is only reached via #include "crt/stdarg.h" from NT4 source.
 * Standard #include <stdarg.h> finds the system version directly.
 */
#ifndef _CRT_STDARG_H_COMPAT_
#define _CRT_STDARG_H_COMPAT_

#ifdef _MSC_VER
  /* MSVC: use vadefs.h which provides va_list */
  #include <vadefs.h>
#else
  /* GCC/Clang: use compiler built-in */
  #ifndef __GNUC_VA_LIST
  #define __GNUC_VA_LIST
  typedef __builtin_va_list va_list;
  #endif
  #define va_start(ap, last) __builtin_va_start(ap, last)
  #define va_arg(ap, type)   __builtin_va_arg(ap, type)
  #define va_end(ap)         __builtin_va_end(ap)
  #define va_copy(dest, src) __builtin_va_copy(dest, src)
#endif

#endif /* _CRT_STDARG_H_COMPAT_ */
