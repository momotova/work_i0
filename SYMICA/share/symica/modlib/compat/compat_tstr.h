#pragma once
#ifndef _compat_tstr_h
#define _compat_tstr_h

#ifdef _MSC_VER 
#  ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t; // for YURI build
#  define _WCHAR_T_DEFINED
#  endif
#endif // _MSC_VER

typedef const char *LPCSTR;

#ifdef UNICODE
   typedef wchar_t _TCHAR;
#else // char
   typedef char _TCHAR;
#endif // UNICODE
#define TCHAR _TCHAR


#ifdef UNICODE
#ifdef _MSC_VER
#pragma warning (disable : 4514)
#endif
#include <wchar.h>
#   define tstrtod  wcstod
#else // char
#   define tstrtod  strtod
#endif // UNICODE

#ifdef _MSC_VER
#pragma warning (disable : 4514)
#endif
#endif // _compat_tstr_h
