//
// File: relop.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 03-Mar-2021 16:17:11
//

// Include Files
#include "relop.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <math.h>

// Function Definitions
//
// Arguments    : double x
//                double y
// Return Type  : boolean_T
//
namespace coder
{
  namespace internal
  {
    boolean_T iseq(double x, double y)
    {
      double absx;
      int exponent;
      boolean_T p;
      absx = std::abs(y / 2.0);
      if ((!rtIsInf(absx)) && (!rtIsNaN(absx))) {
        if (absx <= 2.2250738585072014E-308) {
          absx = 4.94065645841247E-324;
        } else {
          frexp(absx, &exponent);
          absx = std::ldexp(1.0, exponent - 53);
        }
      } else {
        absx = rtNaN;
      }

      if ((std::abs(y - x) < absx) || (rtIsInf(x) && rtIsInf(y) && ((x > 0.0) ==
            (y > 0.0)))) {
        p = true;
      } else {
        p = false;
      }

      return p;
    }
  }
}

//
// File trailer for relop.cpp
//
// [EOF]
//
