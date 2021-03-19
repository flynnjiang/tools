//
// File: schur.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 03-Mar-2021 16:17:11
//

// Include Files
#include "schur.h"
#include "magmcali_data.h"
#include "magmcali_rtwutil.h"
#include "recip.h"
#include "rt_nonfinite.h"
#include "xdlapy3.h"
#include "xgerc.h"
#include "xhseqr.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const creal_T A[9]
//                creal_T V[9]
//                creal_T T[9]
// Return Type  : void
//
namespace coder
{
  void schur(const creal_T A[9], creal_T V[9], creal_T T[9])
  {
    creal_T work[3];
    creal_T tau[2];
    creal_T c;
    double alpha1_im;
    double alpha1_re;
    double beta1;
    int i;
    int ia;
    int knt;
    boolean_T p;
    p = true;
    for (i = 0; i < 9; i++) {
      if ((!p) || (rtIsInf(A[i].re) || rtIsInf(A[i].im) || (rtIsNaN(A[i].re) ||
            rtIsNaN(A[i].im)))) {
        p = false;
      }
    }

    if (!p) {
      int b_i;
      for (b_i = 0; b_i < 9; b_i++) {
        V[b_i].re = rtNaN;
        V[b_i].im = 0.0;
      }

      for (i = 2; i < 4; i++) {
        V[i - 1].re = 0.0;
        V[i - 1].im = 0.0;
      }

      V[5].re = 0.0;
      V[5].im = 0.0;
      for (b_i = 0; b_i < 9; b_i++) {
        T[b_i].re = rtNaN;
        T[b_i].im = 0.0;
      }
    } else {
      double xnorm;
      int b_i;
      int exitg1;
      int i1;
      int ijA;
      int ix;
      int lastc;
      int lastv;
      boolean_T exitg2;
      std::memcpy(&T[0], &A[0], 9U * sizeof(creal_T));
      work[0].re = 0.0;
      work[0].im = 0.0;
      work[1].re = 0.0;
      work[1].im = 0.0;
      work[2].re = 0.0;
      work[2].im = 0.0;
      alpha1_re = T[1].re;
      alpha1_im = T[1].im;
      tau[0].re = 0.0;
      tau[0].im = 0.0;
      xnorm = rt_hypotd_snf(T[2].re, T[2].im);
      if ((xnorm != 0.0) || (T[1].im != 0.0)) {
        beta1 = internal::xdlapy3(T[1].re, T[1].im, xnorm);
        if (T[1].re >= 0.0) {
          beta1 = -beta1;
        }

        if (std::abs(beta1) < 1.0020841800044864E-292) {
          knt = -1;
          do {
            knt++;
            for (i = 3; i < 4; i++) {
              xnorm = 9.9792015476736E+291 * T[2].im + 0.0 * T[2].re;
              T[2].re = 9.9792015476736E+291 * T[2].re - 0.0 * T[2].im;
              T[2].im = xnorm;
            }

            beta1 *= 9.9792015476736E+291;
            alpha1_re *= 9.9792015476736E+291;
            alpha1_im *= 9.9792015476736E+291;
          } while (!(std::abs(beta1) >= 1.0020841800044864E-292));

          beta1 = internal::xdlapy3(alpha1_re, alpha1_im, rt_hypotd_snf(T[2].re,
            T[2].im));
          if (alpha1_re >= 0.0) {
            beta1 = -beta1;
          }

          xnorm = beta1 - alpha1_re;
          if (0.0 - alpha1_im == 0.0) {
            tau[0].re = xnorm / beta1;
            tau[0].im = 0.0;
          } else if (xnorm == 0.0) {
            tau[0].re = 0.0;
            tau[0].im = (0.0 - alpha1_im) / beta1;
          } else {
            tau[0].re = xnorm / beta1;
            tau[0].im = (0.0 - alpha1_im) / beta1;
          }

          c.re = alpha1_re - beta1;
          c.im = alpha1_im;
          c = internal::recip(c);
          for (i = 3; i < 4; i++) {
            xnorm = c.re * T[2].im + c.im * T[2].re;
            T[2].re = c.re * T[2].re - c.im * T[2].im;
            T[2].im = xnorm;
          }

          for (i = 0; i <= knt; i++) {
            beta1 *= 1.0020841800044864E-292;
          }

          alpha1_re = beta1;
          alpha1_im = 0.0;
        } else {
          xnorm = beta1 - T[1].re;
          if (0.0 - T[1].im == 0.0) {
            tau[0].re = xnorm / beta1;
            tau[0].im = 0.0;
          } else if (xnorm == 0.0) {
            tau[0].re = 0.0;
            tau[0].im = (0.0 - T[1].im) / beta1;
          } else {
            tau[0].re = xnorm / beta1;
            tau[0].im = (0.0 - T[1].im) / beta1;
          }

          c.re = T[1].re - beta1;
          c.im = T[1].im;
          c = internal::recip(c);
          for (i = 3; i < 4; i++) {
            xnorm = c.re * T[2].im + c.im * T[2].re;
            T[2].re = c.re * T[2].re - c.im * T[2].im;
            T[2].im = xnorm;
          }

          alpha1_re = beta1;
          alpha1_im = 0.0;
        }
      }

      T[1].re = 1.0;
      T[1].im = 0.0;
      if ((tau[0].re != 0.0) || (tau[0].im != 0.0)) {
        lastv = 1;
        i = 2;
        while ((lastv + 1 > 0) && ((T[i].re == 0.0) && (T[i].im == 0.0))) {
          lastv--;
          i--;
        }

        lastc = 3;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          ia = lastc + 3;
          do {
            exitg1 = 0;
            if (ia <= (lastc + lastv * 3) + 3) {
              if ((T[ia - 1].re != 0.0) || (T[ia - 1].im != 0.0)) {
                exitg1 = 1;
              } else {
                ia += 3;
              }
            } else {
              lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);

          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        lastv = -1;
        lastc = 0;
      }

      if (lastv + 1 > 0) {
        if (lastc != 0) {
          if (0 <= lastc - 1) {
            std::memset(&work[0], 0, lastc * sizeof(creal_T));
          }

          ix = 1;
          b_i = 3 * lastv + 4;
          for (knt = 4; knt <= b_i; knt += 3) {
            c.re = T[ix].re - 0.0 * T[ix].im;
            c.im = T[ix].im + 0.0 * T[ix].re;
            i = 0;
            i1 = (knt + lastc) - 1;
            for (ia = knt; ia <= i1; ia++) {
              xnorm = T[ia - 1].re;
              beta1 = T[ia - 1].im;
              work[i].re += xnorm * c.re - beta1 * c.im;
              work[i].im += xnorm * c.im + beta1 * c.re;
              i++;
            }

            ix++;
          }
        }

        c.re = -tau[0].re;
        c.im = -tau[0].im;
        if ((!(-tau[0].re == 0.0)) || (!(-tau[0].im == 0.0))) {
          knt = 3;
          i = 1;
          for (ia = 0; ia <= lastv; ia++) {
            if ((T[i].re != 0.0) || (T[i].im != 0.0)) {
              xnorm = T[i].re * c.re + T[i].im * c.im;
              beta1 = T[i].re * c.im - T[i].im * c.re;
              ix = 0;
              b_i = knt + 1;
              i1 = lastc + knt;
              for (ijA = b_i; ijA <= i1; ijA++) {
                T[ijA - 1].re += work[ix].re * xnorm - work[ix].im * beta1;
                T[ijA - 1].im += work[ix].re * beta1 + work[ix].im * xnorm;
                ix++;
              }
            }

            i++;
            knt += 3;
          }
        }
      }

      if ((tau[0].re != 0.0) || (-tau[0].im != 0.0)) {
        lastv = 2;
        i = 2;
        while ((lastv > 0) && ((T[i].re == 0.0) && (T[i].im == 0.0))) {
          lastv--;
          i--;
        }

        lastc = 2;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          knt = (lastc - 1) * 3 + 5;
          ia = knt;
          do {
            exitg1 = 0;
            if (ia <= (knt + lastv) - 1) {
              if ((T[ia - 1].re != 0.0) || (T[ia - 1].im != 0.0)) {
                exitg1 = 1;
              } else {
                ia++;
              }
            } else {
              lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);

          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        lastv = 0;
        lastc = 0;
      }

      if (lastv > 0) {
        if (lastc != 0) {
          if (0 <= lastc - 1) {
            std::memset(&work[0], 0, lastc * sizeof(creal_T));
          }

          i = 0;
          b_i = 3 * (lastc - 1) + 5;
          for (knt = 5; knt <= b_i; knt += 3) {
            ix = 1;
            c.re = 0.0;
            c.im = 0.0;
            i1 = (knt + lastv) - 1;
            for (ia = knt; ia <= i1; ia++) {
              c.re += T[ia - 1].re * T[ix].re + T[ia - 1].im * T[ix].im;
              c.im += T[ia - 1].re * T[ix].im - T[ia - 1].im * T[ix].re;
              ix++;
            }

            work[i].re += c.re - 0.0 * c.im;
            work[i].im += c.im + 0.0 * c.re;
            i++;
          }
        }

        c.re = -tau[0].re;
        c.im = tau[0].im;
        internal::blas::xgerc(lastv, lastc, c, 2, work, T, 5);
      }

      T[1].re = alpha1_re;
      T[1].im = alpha1_im;
      alpha1_re = T[5].re;
      alpha1_im = T[5].im;
      tau[1].re = 0.0;
      tau[1].im = 0.0;
      if (T[5].im != 0.0) {
        beta1 = internal::xdlapy3(T[5].re, T[5].im, 0.0);
        if (T[5].re >= 0.0) {
          beta1 = -beta1;
        }

        if (std::abs(beta1) < 1.0020841800044864E-292) {
          knt = -1;
          do {
            knt++;
            beta1 *= 9.9792015476736E+291;
            alpha1_re *= 9.9792015476736E+291;
            alpha1_im *= 9.9792015476736E+291;
          } while (!(std::abs(beta1) >= 1.0020841800044864E-292));

          beta1 = internal::xdlapy3(alpha1_re, alpha1_im, 0.0);
          if (alpha1_re >= 0.0) {
            beta1 = -beta1;
          }

          xnorm = beta1 - alpha1_re;
          if (0.0 - alpha1_im == 0.0) {
            tau[1].re = xnorm / beta1;
            tau[1].im = 0.0;
          } else if (xnorm == 0.0) {
            tau[1].re = 0.0;
            tau[1].im = (0.0 - alpha1_im) / beta1;
          } else {
            tau[1].re = xnorm / beta1;
            tau[1].im = (0.0 - alpha1_im) / beta1;
          }

          for (i = 0; i <= knt; i++) {
            beta1 *= 1.0020841800044864E-292;
          }

          alpha1_re = beta1;
          alpha1_im = 0.0;
        } else {
          xnorm = beta1 - T[5].re;
          if (0.0 - T[5].im == 0.0) {
            tau[1].re = xnorm / beta1;
            tau[1].im = 0.0;
          } else if (xnorm == 0.0) {
            tau[1].re = 0.0;
            tau[1].im = (0.0 - T[5].im) / beta1;
          } else {
            tau[1].re = xnorm / beta1;
            tau[1].im = (0.0 - T[5].im) / beta1;
          }

          alpha1_re = beta1;
          alpha1_im = 0.0;
        }
      }

      T[5].re = 1.0;
      T[5].im = 0.0;
      if ((tau[1].re != 0.0) || (tau[1].im != 0.0)) {
        lastv = 0;
        i = 5;
        while ((lastv + 1 > 0) && ((T[i].re == 0.0) && (T[i].im == 0.0))) {
          lastv--;
          i--;
        }

        lastc = 3;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          ia = lastc + 6;
          do {
            exitg1 = 0;
            if (ia <= (lastc + lastv * 3) + 6) {
              if ((T[ia - 1].re != 0.0) || (T[ia - 1].im != 0.0)) {
                exitg1 = 1;
              } else {
                ia += 3;
              }
            } else {
              lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);

          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        lastv = -1;
        lastc = 0;
      }

      if (lastv + 1 > 0) {
        if (lastc != 0) {
          if (0 <= lastc - 1) {
            std::memset(&work[0], 0, lastc * sizeof(creal_T));
          }

          ix = 5;
          b_i = 3 * lastv + 7;
          for (knt = 7; knt <= b_i; knt += 3) {
            c.re = T[ix].re - 0.0 * T[ix].im;
            c.im = T[ix].im + 0.0 * T[ix].re;
            i = 0;
            i1 = (knt + lastc) - 1;
            for (ia = knt; ia <= i1; ia++) {
              xnorm = T[ia - 1].re;
              beta1 = T[ia - 1].im;
              work[i].re += xnorm * c.re - beta1 * c.im;
              work[i].im += xnorm * c.im + beta1 * c.re;
              i++;
            }

            ix++;
          }
        }

        c.re = -tau[1].re;
        c.im = -tau[1].im;
        if ((!(-tau[1].re == 0.0)) || (!(-tau[1].im == 0.0))) {
          knt = 6;
          i = 5;
          for (ia = 0; ia <= lastv; ia++) {
            if ((T[i].re != 0.0) || (T[i].im != 0.0)) {
              xnorm = T[i].re * c.re + T[i].im * c.im;
              beta1 = T[i].re * c.im - T[i].im * c.re;
              ix = 0;
              b_i = knt + 1;
              i1 = lastc + knt;
              for (ijA = b_i; ijA <= i1; ijA++) {
                T[ijA - 1].re += work[ix].re * xnorm - work[ix].im * beta1;
                T[ijA - 1].im += work[ix].re * beta1 + work[ix].im * xnorm;
                ix++;
              }
            }

            i++;
            knt += 3;
          }
        }
      }

      if ((tau[1].re != 0.0) || (-tau[1].im != 0.0)) {
        lastv = 1;
        if ((T[5].re == 0.0) && (T[5].im == 0.0)) {
          lastv = 0;
        }

        lastc = 1;
        ia = 9;
        do {
          exitg1 = 0;
          if (ia <= lastv + 8) {
            if ((T[8].re != 0.0) || (T[8].im != 0.0)) {
              exitg1 = 1;
            } else {
              ia = 10;
            }
          } else {
            lastc = 0;
            exitg1 = 1;
          }
        } while (exitg1 == 0);
      } else {
        lastv = 0;
        lastc = 0;
      }

      if (lastv > 0) {
        if (lastc != 0) {
          work[0].re = 0.0;
          work[0].im = 0.0;
          i = 0;
          for (knt = 9; knt <= 9; knt += 3) {
            ix = 5;
            c.re = 0.0;
            c.im = 0.0;
            for (ia = 9; ia < 10; ia++) {
              c.re += T[8].re * T[ix].re + T[8].im * T[ix].im;
              c.im += T[8].re * T[ix].im - T[8].im * T[ix].re;
              ix++;
            }

            work[i].re += c.re - 0.0 * c.im;
            work[i].im += c.im + 0.0 * c.re;
            i++;
          }
        }

        c.re = -tau[1].re;
        c.im = tau[1].im;
        internal::blas::xgerc(1, lastc, c, 6, work, T, 9);
      }

      T[5].re = alpha1_re;
      T[5].im = alpha1_im;
      std::memcpy(&V[0], &T[0], 9U * sizeof(creal_T));
      for (i = 0; i < 2; i++) {
        V[i + 6].re = 0.0;
        V[i + 6].im = 0.0;
      }

      V[3].re = 0.0;
      V[3].im = 0.0;
      c = V[2];
      V[5] = c;
      V[1].re = 0.0;
      V[1].im = 0.0;
      V[2].re = 0.0;
      V[2].im = 0.0;
      V[0].re = 1.0;
      V[0].im = 0.0;
      work[0].re = 0.0;
      work[0].im = 0.0;
      work[1].re = 0.0;
      work[1].im = 0.0;
      work[2].re = 0.0;
      work[2].im = 0.0;
      V[8].re = 1.0 - tau[1].re;
      V[8].im = 0.0 - tau[1].im;
      V[7].re = 0.0;
      V[7].im = 0.0;
      V[4].re = 1.0;
      V[4].im = 0.0;
      if ((tau[0].re != 0.0) || (tau[0].im != 0.0)) {
        lastv = 2;
        i = 8;
        while ((lastv > 0) && ((V[i - 3].re == 0.0) && (V[i - 3].im == 0.0))) {
          lastv--;
          i--;
        }

        lastc = 1;
        ia = 8;
        do {
          exitg1 = 0;
          if (ia <= lastv + 7) {
            if ((V[ia - 1].re != 0.0) || (V[ia - 1].im != 0.0)) {
              exitg1 = 1;
            } else {
              ia++;
            }
          } else {
            lastc = 0;
            exitg1 = 1;
          }
        } while (exitg1 == 0);
      } else {
        lastv = 0;
        lastc = 0;
      }

      if (lastv > 0) {
        if (lastc != 0) {
          work[0].re = 0.0;
          work[0].im = 0.0;
          i = 0;
          b_i = lastv + 7;
          for (knt = 8; knt <= 8; knt += 3) {
            ix = 4;
            c.re = 0.0;
            c.im = 0.0;
            for (ia = 8; ia <= b_i; ia++) {
              c.re += V[ia - 1].re * V[ix].re + V[ia - 1].im * V[ix].im;
              c.im += V[ia - 1].re * V[ix].im - V[ia - 1].im * V[ix].re;
              ix++;
            }

            work[i].re += c.re - 0.0 * c.im;
            work[i].im += c.im + 0.0 * c.re;
            i++;
          }
        }

        c.re = -tau[0].re;
        c.im = -tau[0].im;
        internal::blas::xgerc(lastv, lastc, c, 5, work, V, 8);
      }

      c.re = -tau[0].re;
      c.im = -tau[0].im;
      for (i = 6; i < 7; i++) {
        xnorm = c.re * V[5].im + c.im * V[5].re;
        V[5].re = c.re * V[5].re - c.im * V[5].im;
        V[5].im = xnorm;
      }

      V[4].re = 1.0 - tau[0].re;
      V[4].im = 0.0 - tau[0].im;
      internal::lapack::xhseqr(T, V);
    }
  }
}

//
// File trailer for schur.cpp
//
// [EOF]
//
