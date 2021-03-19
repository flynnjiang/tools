//
// File: xzhgeqz.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 03-Mar-2021 16:17:11
//
#ifndef XZHGEQZ_H
#define XZHGEQZ_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder
{
  namespace internal
  {
    namespace reflapack
    {
      void xzhgeqz(creal_T A[100], int ilo, int ihi, creal_T Z[100], int *info,
                   creal_T alpha1[10], creal_T beta1[10]);
    }
  }
}

#endif

//
// File trailer for xzhgeqz.h
//
// [EOF]
//
