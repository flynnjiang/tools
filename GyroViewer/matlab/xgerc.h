//
// File: xgerc.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 03-Mar-2021 16:17:11
//
#ifndef XGERC_H
#define XGERC_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder
{
  namespace internal
  {
    namespace blas
    {
      void xgerc(int m, int n, const creal_T alpha1, int ix0, const creal_T y[3],
                 creal_T A[9], int ia0);
    }
  }
}

#endif

//
// File trailer for xgerc.h
//
// [EOF]
//
