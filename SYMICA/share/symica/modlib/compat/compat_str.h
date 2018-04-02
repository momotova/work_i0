#pragma once
#ifndef _compat_str_h
#define _compat_str_h

#include <stdarg.h>

#if defined(__GNUC__) && !defined(__MINGW32__)

#  define stricmp  strcasecmp
#  define _stricmp strcasecmp
#  define strnicmp strncasecmp

#include <stddef.h>

#endif //UNIX

#ifdef _MSC_VER
#pragma warning (disable : 4710)
#pragma warning (push, 3)
#endif

#include <string>

#ifdef _MSC_VER
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#endif // _MSC_VER

#ifdef __GNUC__
#define _format_printf_(fmt, arg) __attribute__ ((format (printf, (fmt), (arg))))
#else
#define _format_printf_(fmt, arg)
#endif // __GNUC__

char *compat_strupr (char *s);
char compat_tolower (char);

std::string compat_str_sprintf  (const char* fmt, ...)        _format_printf_(1, 2);
std::string compat_str_vsprintf (const char* fmt, va_list va) _format_printf_(1, 0);

std::string compat_str_oem2ansi(const std::string&);
std::string compat_str_ansi2oem(const std::string&);

char compat_isspace (char c);

std::string compat_str_mask(const std::string& s, const std::string& masked = "\\\"");
std::string compat_str_unmask(const std::string& s, const std::string& unmasked = "\\\"");

std::string compat_str_unescape(const std::string& s);

#ifndef KIPARIS_AUTO_BUILD

#include <iterator>
#include <algorithm>

template<typename T1, typename T2, typename T3, typename T4>
T4 translateSymbolicConst(const T1& from, const T2& to, const T3& what, T4 def) {
  auto found = std::find(std::begin(from), std::end(from), what);
  if(found == std::end(from))
    return def;
  return to[found - std::begin(from)];
}
#endif // KIPARIS_AUTO_BUILD

#ifdef _MSC_VER
#pragma warning (pop)
#pragma warning (disable : 4514)
#endif // _MSC_VER

#endif // _compat_str_h
