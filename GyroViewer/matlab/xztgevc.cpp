//
// File: xztgevc.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 03-Mar-2021 16:17:11
//

// Include Files
#include "xztgevc.h"
#include "magmcali_data.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const creal_T A[100]
//                creal_T V[100]
// Return Type  : void
//
namespace coder
{
  namespace internal
  {
    namespace reflapack
    {
      void xztgevc(const creal_T A[100], creal_T V[100])
      {
        creal_T work1[10];
        creal_T work2[10];
        double rworka[10];
        double acoeff;
        double anorm;
        double ascale;
        double salpha_im;
        double salpha_re;
        double y;
        int i;
        int j;
        int re_tmp;
        std::memset(&rworka[0], 0, 10U * sizeof(double));
        anorm = std::abs(A[0].re) + std::abs(A[0].im);
        for (j = 0; j < 9; j++) {
          for (i = 0; i <= j; i++) {
            re_tmp = i + 10 * (j + 1);
            rworka[j + 1] += std::abs(A[re_tmp].re) + std::abs(A[re_tmp].im);
          }

          re_tmp = (j + 10 * (j + 1)) + 1;
          y = rworka[j + 1] + (std::abs(A[re_tmp].re) + std::abs(A[re_tmp].im));
          if (y > anorm) {
            anorm = y;
          }
        }

        y = anorm;
        if (2.2250738585072014E-308 > anorm) {
          y = 2.2250738585072014E-308;
        }

        ascale = 1.0 / y;
        for (int je = 0; je < 10; je++) {
          double dmin;
          double scale;
          double temp;
          double xmx;
          int b_i;
          int jr;
          int x_tmp_tmp;
          boolean_T lscalea;
          boolean_T lscaleb;
          x_tmp_tmp = 10 * (9 - je);
          re_tmp = (x_tmp_tmp - je) + 9;
          xmx = A[re_tmp].re;
          scale = A[re_tmp].im;
          y = (std::abs(xmx) + std::abs(scale)) * ascale;
          if (1.0 > y) {
            y = 1.0;
          }

          temp = 1.0 / y;
          salpha_re = ascale * (temp * xmx);
          salpha_im = ascale * (temp * scale);
          acoeff = temp * ascale;
          if ((temp >= 2.2250738585072014E-308) && (acoeff <
               1.0020841800044864E-291)) {
            lscalea = true;
          } else {
            lscalea = false;
          }

          xmx = std::abs(salpha_re) + std::abs(salpha_im);
          if ((xmx >= 2.2250738585072014E-308) && (xmx < 1.0020841800044864E-291))
          {
            lscaleb = true;
          } else {
            lscaleb = false;
          }

          scale = 1.0;
          if (lscalea) {
            y = anorm;
            if (9.9792015476736E+290 < anorm) {
              y = 9.9792015476736E+290;
            }

            scale = 1.0020841800044864E-291 / temp * y;
          }

          if (lscaleb) {
            y = 1.0020841800044864E-291 / xmx;
            if (y > scale) {
              scale = y;
            }
          }

          if (lscalea || lscaleb) {
            y = acoeff;
            if (1.0 > acoeff) {
              y = 1.0;
            }

            if (xmx > y) {
              y = xmx;
            }

            y = 1.0 / (2.2250738585072014E-308 * y);
            if (y < scale) {
              scale = y;
            }

            if (lscalea) {
              acoeff = ascale * (scale * temp);
            } else {
              acoeff *= scale;
            }

            salpha_re *= scale;
            salpha_im *= scale;
          }

          std::memset(&work1[0], 0, 10U * sizeof(creal_T));
          work1[9 - je].re = 1.0;
          work1[9 - je].im = 0.0;
          dmin = 2.2204460492503131E-16 * acoeff * anorm;
          y = 2.2204460492503131E-16 * (std::abs(salpha_re) + std::abs(salpha_im));
          if (y > dmin) {
            dmin = y;
          }

          if (2.2250738585072014E-308 > dmin) {
            dmin = 2.2250738585072014E-308;
          }

          b_i = 8 - je;
          for (jr = 0; jr <= b_i; jr++) {
            re_tmp = jr + x_tmp_tmp;
            work1[jr].re = acoeff * A[re_tmp].re;
            work1[jr].im = acoeff * A[re_tmp].im;
          }

          work1[9 - je].re = 1.0;
          work1[9 - je].im = 0.0;
          b_i = static_cast<int>(((-1.0 - ((-static_cast<double>(je) + 10.0) -
            1.0)) + 1.0) / -1.0);
          for (j = 0; j < b_i; j++) {
            double ai;
            double brm;
            double d_im;
            double d_re;
            i = 8 - (je + j);
            re_tmp = i + 10 * i;
            d_re = acoeff * A[re_tmp].re - salpha_re;
            d_im = acoeff * A[re_tmp].im - salpha_im;
            if (std::abs(d_re) + std::abs(d_im) <= dmin) {
              d_re = dmin;
              d_im = 0.0;
            }

            brm = std::abs(d_re);
            y = std::abs(d_im);
            xmx = brm + y;
            if (xmx < 1.0) {
              scale = std::abs(work1[i].re) + std::abs(work1[i].im);
              if (scale >= 4.49423283715579E+306 * xmx) {
                temp = 1.0 / scale;
                re_tmp = 9 - je;
                for (jr = 0; jr <= re_tmp; jr++) {
                  work1[jr].re *= temp;
                  work1[jr].im *= temp;
                }
              }
            }

            temp = -work1[i].re;
            ai = -work1[i].im;
            if (d_im == 0.0) {
              if (ai == 0.0) {
                y = temp / d_re;
                xmx = 0.0;
              } else if (temp == 0.0) {
                y = 0.0;
                xmx = ai / d_re;
              } else {
                y = temp / d_re;
                xmx = ai / d_re;
              }
            } else if (d_re == 0.0) {
              if (temp == 0.0) {
                y = ai / d_im;
                xmx = 0.0;
              } else if (ai == 0.0) {
                y = 0.0;
                xmx = -(temp / d_im);
              } else {
                y = ai / d_im;
                xmx = -(temp / d_im);
              }
            } else if (brm > y) {
              scale = d_im / d_re;
              xmx = d_re + scale * d_im;
              y = (temp + scale * ai) / xmx;
              xmx = (ai - scale * temp) / xmx;
            } else if (y == brm) {
              if (d_re > 0.0) {
                scale = 0.5;
              } else {
                scale = -0.5;
              }

              if (d_im > 0.0) {
                xmx = 0.5;
              } else {
                xmx = -0.5;
              }

              y = (temp * scale + ai * xmx) / brm;
              xmx = (ai * scale - temp * xmx) / brm;
            } else {
              scale = d_re / d_im;
              xmx = d_im + scale * d_re;
              y = (scale * temp + ai) / xmx;
              xmx = (scale * ai - temp) / xmx;
            }

            work1[i].re = y;
            work1[i].im = xmx;
            if (i + 1 > 1) {
              if (std::abs(work1[i].re) + std::abs(work1[i].im) > 1.0) {
                temp = 1.0 / (std::abs(work1[i].re) + std::abs(work1[i].im));
                if (acoeff * rworka[i] >= 4.49423283715579E+306 * temp) {
                  re_tmp = 9 - je;
                  for (jr = 0; jr <= re_tmp; jr++) {
                    work1[jr].re *= temp;
                    work1[jr].im *= temp;
                  }
                }
              }

              d_re = acoeff * work1[i].re;
              d_im = acoeff * work1[i].im;
              for (jr = 0; jr < i; jr++) {
                re_tmp = jr + 10 * i;
                work1[jr].re += d_re * A[re_tmp].re - d_im * A[re_tmp].im;
                work1[jr].im += d_re * A[re_tmp].im + d_im * A[re_tmp].re;
              }
            }
          }

          std::memset(&work2[0], 0, 10U * sizeof(creal_T));
          b_i = 9 - je;
          for (i = 0; i <= b_i; i++) {
            xmx = work1[i].re;
            scale = work1[i].im;
            for (jr = 0; jr < 10; jr++) {
              re_tmp = jr + 10 * i;
              work2[jr].re += V[re_tmp].re * xmx - V[re_tmp].im * scale;
              work2[jr].im += V[re_tmp].re * scale + V[re_tmp].im * xmx;
            }
          }

          xmx = std::abs(work2[0].re) + std::abs(work2[0].im);
          for (jr = 0; jr < 9; jr++) {
            y = std::abs(work2[jr + 1].re) + std::abs(work2[jr + 1].im);
            if (y > xmx) {
              xmx = y;
            }
          }

          if (xmx > 2.2250738585072014E-308) {
            temp = 1.0 / xmx;
            for (jr = 0; jr < 10; jr++) {
              b_i = jr + x_tmp_tmp;
              V[b_i].re = temp * work2[jr].re;
              V[b_i].im = temp * work2[jr].im;
            }
          } else {
            std::memset(&V[x_tmp_tmp], 0, 10U * sizeof(creal_T));
          }
        }
      }
    }
  }
}

//
// File trailer for xztgevc.cpp
//
// [EOF]
//
