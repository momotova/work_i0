#pragma once
#ifndef _paramt_h
#define _paramt_h

#include <math.h>
#include <stdio.h>

#include <string>
#include <vector>
#include "base/newstr.h"

#define FLOAT_PRINT_FMT "%.12g"

#define defParIndRef     "Paramref"
#define defModelNameRef  "model name"
#define defModelRef      "model"

class ParamVec;

// "" as value => type = param, cVal = new char[]("");
const double PARAM_UNDEF = 1e28;

class ParamT {
public:
  enum value_type {
    none   = 0, // uninitialized
    text   = 1, // type = nmos
    number = 2, // a = 1.0
    param  = 3, // a = 'a+b' or a=b
    vector = 4, // a = [1  b 'b*b']
    elem   = 5, // a = [node1 node2 ...] or a = [elem1 elem2 ...]
    file   = 6, // filename
    max_type = 6
  };
  
  union param_val{
    double    dVal;
    char*     cVal;
    ParamVec* pVal;
  };
  
public:
  static bool isTextType (ParamT::value_type type) {
    if(type == ParamT::text  ||
       type == ParamT::param ||
       type == ParamT::file)
      return true;
    return false;
  }

  bool isTextType () const { return isTextType(type); }

  static bool isVectorType (ParamT::value_type type) {
    if(type == ParamT::vector || 
       type == ParamT::elem )
      return true;
    return false;
  }

  bool isVectorType () const { return isVectorType(type); }

  static bool isParamType(ParamT::value_type type) {
    return type == ParamT::param;
  }
  bool isParamType() const { return isParamType(type); }

  bool isUndef() const {
    return type     == ParamT::number && 
           val.dVal == PARAM_UNDEF;
  }

public:
  ParamT() : type(ParamT::none) { val.pVal = NULL; val.cVal = NULL; }

  ParamT(const ParamT& src);
  
  ParamT& operator = (ParamT rhs) {
    this->swap(rhs);
    return *this;
  }
  
  ~ParamT();
  
  void swap(ParamT& rhs) {
    std::swap(type, rhs.type);
    std::swap(val.dVal, rhs.val.dVal);
  }

  inline std::string toText() const;
  
  ParamT& operator +=(const ParamT& rhs);
  ParamT& operator +=(double rhs);
  ParamT& operator /=(const ParamT& rhs);
  ParamT& operator *=(const ParamT& rhs);

  friend ParamT operator +  (double lhs, const ParamT& rhs);

  friend ParamT operator +  (const ParamT& lhs, const ParamT& rhs);
  friend bool   operator == (const ParamT& lhs, const ParamT& rhs);
  friend bool   operator != (const ParamT& lhs, const ParamT& rhs);
    
  friend ParamT operator /  (const ParamT& lhs, const ParamT& rhs);
  friend ParamT operator *  (const ParamT& lhs, const ParamT& rhs);

public:
  value_type type;
  param_val  val;
};

inline void swap(ParamT& lhs, ParamT& rhs) {
  lhs.swap(rhs);
}

bool operator == (const ParamT& lhs, const ParamT& rhs);
bool operator != (const ParamT& lhs, const ParamT& rhs);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ParamVec{
public:
  ParamVec() : size(0), data(NULL) {}

  ParamVec(const ParamVec& src) {
    data = new ParamT[src.size];
    for(size_t i=0; i<src.size; ++i)
      data[i] = src.data[i];
    size = src.size;
  }
  
  ~ParamVec() {
    delete[] data; data = NULL;
  }
  
  ParamVec& operator = (const ParamVec& rhs) {
    ParamVec t(rhs);
    this->swap(t);
    return *this;
  }

  std::string toText() {
    std::string result;
    for(size_t i=0; i < size; ++i)
      result += data[i].toText() + " ";
    return result.substr(0, result.size()-1);
  }
  
  void swap(ParamVec& rhs) {
    std::swap(size, rhs.size);
    std::swap(data, rhs.data);
  }

  friend bool operator == (const ParamVec& lhs, const ParamVec& rhs);
  friend bool operator != (const ParamVec& lhs, const ParamVec& rhs);
  
public:
  size_t   size;
  ParamT*  data;
};

inline void swap(ParamVec& lhs, ParamVec& rhs) {
  lhs.swap(rhs);
}

inline bool operator == (const ParamVec& lhs, const ParamVec& rhs) {
  if(lhs.size != rhs.size)
    return false;
  for(size_t i=0; i<lhs.size; ++i)
    if(lhs.data[i] != rhs.data[i])
      return false;
  return true;
}

inline bool operator != (const ParamVec& lhs, const ParamVec& rhs) {
  return !(lhs == rhs);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline ParamT::ParamT(const ParamT& src): type(src.type) {
  if(src.type == ParamT::none)
    return;
  if(src.type == ParamT::number) {
    val.dVal = src.val.dVal;
    return;
  }
  if(isVectorType(src.type)) {
    val.pVal = new ParamVec(*src.val.pVal);
    return;
  }
  val.cVal = newStr(src.val.cVal);  
}

inline ParamT::~ParamT() {
  if(isTextType()) {
    delete[] val.cVal;
    val.cVal = NULL;
  }
  if(isVectorType(type)) {
    delete val.pVal;
    val.pVal = NULL;
  }
}

inline std::string ParamT::toText() const {
  if(type == ParamT::number) {
    char buffer[32];
    sprintf(buffer, FLOAT_PRINT_FMT, val.dVal);
    return buffer;
  }
  if(isTextType())
    return std::string(val.cVal);
  if(isVectorType(type)) 
    return val.pVal->toText();
  return "";
}

inline bool operator == (const ParamT& lhs, const ParamT& rhs) {
  if(lhs.type != rhs.type)
    return false;
  if(lhs.type == ParamT::number) {
    double lVal = fabs(lhs.val.dVal);
    double koeff = 1e-14;
    if(lVal < 1e-10) koeff = 1e-6;
    if(lVal < 1e-15) koeff = 1e-2;
    return fabs((lhs.val.dVal - rhs.val.dVal)) <= koeff * lVal;
  }
  if(ParamT::isTextType(lhs.type))
    return (!strcmp(lhs.val.cVal, rhs.val.cVal));
  if(ParamT::isVectorType(lhs.type))
    return *lhs.val.pVal == *rhs.val.pVal;
  if(lhs.type == ParamT::none)
    return true;
// !!! base ???
  return false;
}

inline bool operator != (const ParamT& lhs, const ParamT& rhs) {
  return !(lhs == rhs);
}

inline ParamT& ParamT::operator *=(const ParamT& rhs) {
  if((   type == ParamT::number) &&
    (rhs.type == ParamT::number)) {
    val.dVal *= rhs.val.dVal;
  } else {
    std::string res = "(" + toText() + ")*(" + rhs.toText() + ")";
    if(isTextType())
      delete[] val.cVal;
    val.cVal = newStr(res);
    type = ParamT::param;
  }
  return *this;
}

inline ParamT& ParamT::operator /=(const ParamT& rhs) {
  if((   type == ParamT::number) &&
    (rhs.type == ParamT::number)) {
    val.dVal /= rhs.val.dVal;
  } else {
    std::string res = "(" + toText() + ")/(" + rhs.toText() + ")";
    if(isTextType())
      delete[] val.cVal;
    val.cVal = newStr(res);
    type = ParamT::param;
  }
  return *this;
}

inline ParamT& ParamT::operator +=(double rhs) {
  char buffer[512];
  if(   type == ParamT::number) {
    val.dVal += rhs;
  } else {
    sprintf(buffer, FLOAT_PRINT_FMT, rhs );
    std::string res = "(" + toText() + ")+" +  buffer;
    if(isTextType())
      delete[] val.cVal;
    val.cVal = newStr(res);
    type = ParamT::param;
  }
  return *this;
}

inline ParamT& ParamT::operator +=(const ParamT& rhs) {
  if((    type == ParamT::number) &&
     (rhs.type == ParamT::number)) {
    val.dVal += rhs.val.dVal;
  } else {
    std::string res = "(" + toText() + ")+(" + rhs.toText() + ")";
    if(isTextType())
      delete[] val.cVal;
    val.cVal = newStr(res);
    type = ParamT::param;
  }
  return *this;
}

inline ParamT operator *  (const ParamT& lhs, const ParamT& rhs) {
  ParamT temp(lhs);
  temp *= rhs;
  return temp;
}

inline ParamT operator / (const ParamT& lhs, const ParamT& rhs) {
  ParamT temp(lhs);
  temp /= rhs;
  return temp;
}

inline ParamT operator + (const ParamT& lhs, const ParamT& rhs) {
  ParamT temp(lhs);
  temp += rhs;
  return temp;
}

inline ParamT operator +  (double lhs, const ParamT& rhs) {
  ParamT temp(rhs);
  temp += lhs;
  return temp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class NamedParam : public ParamT {
  public:
  NamedParam() : ParamT() {}

  NamedParam(const NamedParam& src) : ParamT(src) , par_name(src.par_name) {}

  NamedParam& operator = (const NamedParam& rhs) {
    NamedParam t(rhs);
    this->swap(t);
    return *this;
  }

  ~NamedParam() {}

  void swap(NamedParam& rhs) {
    ParamT::swap(rhs);
    std::swap(par_name, rhs.par_name);
  }

  public:
    std::string par_name;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ParName
{
public:

  ParName() : type(0), pModelName(NULL), par(NULL), isSpectre(0), bsrc_type(0), ModelIndex(-1) { }

  ParName(const ParName& rhs) :
    type(rhs.type),
    pModelName(newStr(rhs.pModelName)),
    par(new ParamVec(*rhs.par)),
    isSpectre(rhs.isSpectre),
    bsrc_type(rhs.bsrc_type),
    ModelIndex(rhs.ModelIndex) {
  }

  ~ParName() {
    delete par;
    par = NULL;
    delete[] pModelName;
    pModelName = NULL;
  }

public:

  int       type;
  char    * pModelName;
  ParamVec* par;
  int       isSpectre;  // 1 when spectre, 0 when hspice
  int       bsrc_type;
  int       ModelIndex;
};

typedef ParName GrpParName;
typedef ParName IndParName;

#endif // _paramt_h


