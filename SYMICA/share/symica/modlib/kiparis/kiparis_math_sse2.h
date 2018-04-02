#pragma once
#ifndef _KIPARIS_MATH_SSE2_
#define _KIPARIS_MATH_SSE2_

#ifdef USE_KIP_EXP

namespace kiparis
{
  namespace math
  {
    double exp(double x);
    double log(double x);
  }
}

namespace kip = kiparis::math;

#define KIP_EXP(x) ( kiparis::math::exp((x)) )
#define KIP_LOG(x) ( kiparis::math::log((x)) )

#else

#include <math.h>

#define KIP_EXP(x) ( ::exp( (x) ) )
#define KIP_LOG(x) ( ::log( (x) ) )

#endif // USE_KIP_EXP

#endif // _KIPARIS_MATH_SSE2_
