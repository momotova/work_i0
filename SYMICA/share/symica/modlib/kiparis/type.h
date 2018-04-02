/*-------------------------------------------------------*/
/* Copyright 1999 Kokin S.A.                             */
/*-------------------------------------------------------*/

#pragma once
#ifndef  _TYPE_H
#define  _TYPE_H

#include "compat/compat_tstr.h"
#include <float.h>

#ifdef __BORLANDC__
#include <systypes.h>
#include <classlib/defs.h>
#else

typedef signed char  int8;
typedef signed short int16;
typedef signed int   int32;

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;

#endif

typedef float   REAL4;
typedef double  REAL;

typedef int32 bOOL;
#define tRUE  1
#define fALSE 0

#define LINESIZE  80             // linesize for output routines

typedef int       INTEGER;       // precision of integer arithmetic
typedef int       INT_32BIT;     // 32 bit int, for random number generators

typedef double    REAL;          // precision of floating point arithmetic
typedef float     FLOAT_IEEE;    // IEEE float, for random number generators

#define INTEGER_MIN     INT_MIN;       // Constants from limits.h that
#define INTEGER_MAX     INT_MAX;       // correspond to the integer type
                                       // that INTEGER represents.

#define REAL_RADIX      FLT_RADIX      // Constants from float.h that 
#define REAL_ROUNDS     FLT_ROUNDS     // correspond to the float type 
#define REAL_MANT_DIG   DBL_MANT_DIG   // that REAL represents.
#define REAL_DIG        DBL_DIG 
#define REAL_MIN_EXP    DBL_MIN_EXP
#define REAL_MIN_10_EXP DBL_MIN_10_EXP
#define REAL_MAX_EXP    DBL_MAX_EXP
#define REAL_MAX_10_EXP DBL_MAX_10_EXP  
#define REAL_MAX        DBL_MAX        // About 1.7976e+308 for an IEEE double.
#define REAL_EPSILON    DBL_EPSILON    // About 2.2205e-16 for an IEEE double.
#define REAL_MIN        DBL_MIN        // About 2.2251e-308 for an IEEE double.

#define EPS       1.0e-13       // relative tolerance for rank determination 
                                // REAL_EPSILON/1.e-3 is reasonable.  


#define BIT0    0x01
#define BIT1    0x02
#define BIT2    0x04
#define BIT3    0x08
#define BIT4    0x10
#define BIT5    0x20
#define BIT6    0x40
#define BIT7    0x80
#define BIT8    0x100
#define BIT9    0x200
#define BIT10   0x400
#define BIT11   0x800
#define BIT12   0x1000
#define BIT13   0x2000
#define BIT14   0x4000
#define BIT15   0x8000

#endif //  _TYPE_H

