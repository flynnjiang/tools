//
// File: xgerc.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 03-Mar-2021 16:17:11
//

// Include Files
#include "xgerc.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : int m
//                int n
//                const creal_T alpha1
//                int ix0
//                const creal_T y[3]
//                creal_T A[9]
//                int ia0
// Return Type  : void
//
namespace coder
{
  namespace internal
  {
    namespace blas
    {
      void xgerc(int m, int n, const creal_T alpha1, int ix0, const creal_T y[3],
                 creal_T A[9], int ia0)
      {
        int jA;
        if ((!(alpha1.re == 0.0)) || (!(alpha1.im == 0.0))) {
          int jy;
          jA = ia0;
          jy = 0;
          for (int j = 0; j < n; j++) {
            if ((y[jy].re != 0.0) || (y[jy].im != 0.0)) {
              double temp_im;
              double temp_re;
              int i;
              int ix;
              temp_re = y[jy].re * alpha1.re + y[jy].im * alpha1.im;
              temp_im = y[jy].re * alpha1.im - y[jy].im * alpha1.re;
              ix = ix0;
              i = m + jA;
              for (int ijA = jA; ijA < i; ijA++) {
                double A_im;
                A_im = A[ix - 1].re * temp_im + A[ix - 1].im * temp_re;
                A[ijA - 1].re += A[ix - 1].re * temp_re - A[ix - 1].im * temp_im;
                A[ijA - 1].im += A_im;
                ix++;
              }
            }

            jy++;
            jA += 3;
          }
        }
      }
    }
  }
}

//
// File trailer for xgerc.cpp
//
// [EOF]
//
