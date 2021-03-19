//
// File: xdlapy3.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 03-Mar-2021 16:17:11
//

// Include Files
#include "xdlapy3.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : double x1
//                double x2
//                double x3
// Return Type  : double
//
namespace coder
{
  namespace internal
  {
    double xdlapy3(double x1, double x2, double x3)
    {
      double a;
      double b;
      double c;
      double y;
      a = std::abs(x1);
      b = std::abs(x2);
      c = std::abs(x3);
      if ((a > b) || rtIsNaN(b)) {
        y = a;
      } else {
        y = b;
      }

      if (c > y) {
        y = c;
      }

      if ((y > 0.0) && (!rtIsInf(y))) {
        a /= y;
        b /= y;
        c /= y;
        y *= std::sqrt((a * a + c * c) + b * b);
      } else {
        y = (a + b) + c;
      }

      return y;
    }
  }
}

//
// File trailer for xdlapy3.cpp
//
// [EOF]
//
