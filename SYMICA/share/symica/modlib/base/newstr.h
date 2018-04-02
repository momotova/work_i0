#pragma once
#ifndef NEW_STR_DEFINED
#define NEW_STR_DEFINED

#include <string.h>
#include <string>

inline char* newStr(const char* rhs) {
  char* ret = new char[strlen(rhs)+1];
  strcpy(ret, rhs);
  return ret;
}

inline char* newStr(const std::string& rhs) {
  char* ret = new char[rhs.size()+1];
  strcpy(ret, rhs.c_str());
  return ret;
}

#endif // NEW_STR_DEFINED
