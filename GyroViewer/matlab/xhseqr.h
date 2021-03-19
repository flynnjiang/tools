//
// File: xhseqr.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 03-Mar-2021 16:17:11
//
#ifndef XHSEQR_H
#define XHSEQR_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder
{
  namespace internal
  {
    namespace lapack
    {
      int xhseqr(double h[100], double z[100]);
      int xhseqr(creal_T h[9], creal_T z[9]);
    }
  }
}

#endif

//
// File trailer for xhseqr.h
//
// [EOF]
//
