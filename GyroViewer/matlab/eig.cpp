//
// File: eig.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 03-Mar-2021 16:17:11
//

// Include Files
#include "eig.h"
#include "magmcali_data.h"
#include "magmcali_rtwutil.h"
#include "rt_nonfinite.h"
#include "xhseqr.h"
#include "xnrm2.h"
#include "xzggev.h"
#include "xzlarf.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const double A[100]
//                creal_T V[100]
//                creal_T D[100]
// Return Type  : void
//
namespace coder
{
  void eig(const double A[100], creal_T V[100], creal_T D[100])
  {
    creal_T At[100];
    creal_T alpha1[10];
    creal_T beta1[10];
    double b_A[100];
    double b_V[100];
    double work[10];
    double tau[9];
    double absxk;
    double colnorm;
    double t;
    int iaii;
    int k;
    int kend;
    boolean_T p;
    p = true;
    for (k = 0; k < 100; k++) {
      if ((!p) || (rtIsInf(A[k]) || rtIsNaN(A[k]))) {
        p = false;
      }
    }

    if (!p) {
      int i;
      for (i = 0; i < 100; i++) {
        V[i].re = rtNaN;
        V[i].im = 0.0;
        D[i].re = 0.0;
        D[i].im = 0.0;
      }

      for (k = 0; k < 10; k++) {
        i = k + 10 * k;
        D[i].re = rtNaN;
        D[i].im = 0.0;
      }
    } else {
      int b_i;
      int exitg1;
      int j;
      boolean_T exitg2;
      p = true;
      j = 0;
      exitg2 = false;
      while ((!exitg2) && (j < 10)) {
        b_i = 0;
        do {
          exitg1 = 0;
          if (b_i <= j) {
            if (!(A[b_i + 10 * j] == A[j + 10 * b_i])) {
              p = false;
              exitg1 = 1;
            } else {
              b_i++;
            }
          } else {
            j++;
            exitg1 = 2;
          }
        } while (exitg1 == 0);

        if (exitg1 == 1) {
          exitg2 = true;
        }
      }

      if (p) {
        int i;
        p = true;
        for (k = 0; k < 100; k++) {
          if ((!p) || (rtIsInf(A[k]) || rtIsNaN(A[k]))) {
            p = false;
          }
        }

        if (!p) {
          for (i = 0; i < 100; i++) {
            b_V[i] = rtNaN;
          }

          kend = 2;
          for (j = 0; j < 9; j++) {
            if (kend <= 10) {
              std::memset(&b_V[(j * 10 + kend) + -1], 0, (11 - kend) * sizeof
                          (double));
            }

            kend++;
          }

          for (i = 0; i < 100; i++) {
            b_A[i] = rtNaN;
          }
        } else {
          std::memcpy(&b_A[0], &A[0], 100U * sizeof(double));
          std::memset(&work[0], 0, 10U * sizeof(double));
          for (b_i = 0; b_i < 9; b_i++) {
            double scale;
            int alpha1_tmp;
            int im1n_tmp;
            int in;
            int jy;
            int lastc;
            int lastv;
            im1n_tmp = b_i * 10 + 2;
            in = (b_i + 1) * 10;
            alpha1_tmp = (b_i + 10 * b_i) + 1;
            t = b_A[alpha1_tmp];
            if (b_i + 3 < 10) {
              jy = b_i + 1;
            } else {
              jy = 8;
            }

            j = jy + im1n_tmp;
            tau[b_i] = 0.0;
            scale = internal::blas::xnrm2(8 - b_i, b_A, j);
            if (scale != 0.0) {
              absxk = rt_hypotd_snf(t, scale);
              if (t >= 0.0) {
                absxk = -absxk;
              }

              if (std::abs(absxk) < 1.0020841800044864E-292) {
                kend = -1;
                i = (j - b_i) + 7;
                do {
                  kend++;
                  for (k = j; k <= i; k++) {
                    b_A[k - 1] *= 9.9792015476736E+291;
                  }

                  absxk *= 9.9792015476736E+291;
                  t *= 9.9792015476736E+291;
                } while (!(std::abs(absxk) >= 1.0020841800044864E-292));

                absxk = rt_hypotd_snf(t, internal::blas::xnrm2(8 - b_i, b_A, j));
                if (t >= 0.0) {
                  absxk = -absxk;
                }

                tau[b_i] = (absxk - t) / absxk;
                scale = 1.0 / (t - absxk);
                i = (j - b_i) + 7;
                for (k = j; k <= i; k++) {
                  b_A[k - 1] *= scale;
                }

                for (k = 0; k <= kend; k++) {
                  absxk *= 1.0020841800044864E-292;
                }

                t = absxk;
              } else {
                tau[b_i] = (absxk - t) / absxk;
                scale = 1.0 / (t - absxk);
                i = (j - b_i) + 7;
                for (k = j; k <= i; k++) {
                  b_A[k - 1] *= scale;
                }

                t = absxk;
              }
            }

            b_A[alpha1_tmp] = 1.0;
            jy = (b_i + im1n_tmp) - 1;
            j = in + 1;
            if (tau[b_i] != 0.0) {
              lastv = 8 - b_i;
              kend = (jy - b_i) + 8;
              while ((lastv + 1 > 0) && (b_A[kend] == 0.0)) {
                lastv--;
                kend--;
              }

              lastc = 10;
              exitg2 = false;
              while ((!exitg2) && (lastc > 0)) {
                kend = in + lastc;
                k = kend;
                do {
                  exitg1 = 0;
                  if (k <= kend + lastv * 10) {
                    if (b_A[k - 1] != 0.0) {
                      exitg1 = 1;
                    } else {
                      k += 10;
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
              int i1;
              int ix;
              if (lastc != 0) {
                if (0 <= lastc - 1) {
                  std::memset(&work[0], 0, lastc * sizeof(double));
                }

                ix = jy;
                i = (in + 10 * lastv) + 1;
                for (kend = j; kend <= i; kend += 10) {
                  iaii = 0;
                  i1 = (kend + lastc) - 1;
                  for (k = kend; k <= i1; k++) {
                    work[iaii] += b_A[k - 1] * b_A[ix];
                    iaii++;
                  }

                  ix++;
                }
              }

              if (!(-tau[b_i] == 0.0)) {
                kend = in;
                for (j = 0; j <= lastv; j++) {
                  if (b_A[jy] != 0.0) {
                    scale = b_A[jy] * -tau[b_i];
                    ix = 0;
                    i = kend + 1;
                    i1 = lastc + kend;
                    for (iaii = i; iaii <= i1; iaii++) {
                      b_A[iaii - 1] += work[ix] * scale;
                      ix++;
                    }
                  }

                  jy++;
                  kend += 10;
                }
              }
            }

            internal::reflapack::xzlarf(9 - b_i, 9 - b_i, b_i + im1n_tmp,
              tau[b_i], b_A, (b_i + in) + 2, work);
            b_A[alpha1_tmp] = t;
          }

          std::memcpy(&b_V[0], &b_A[0], 100U * sizeof(double));
          for (j = 8; j >= 0; j--) {
            k = (j + 1) * 10;
            for (b_i = 0; b_i <= j; b_i++) {
              b_V[k + b_i] = 0.0;
            }

            i = j + 3;
            for (b_i = i; b_i < 11; b_i++) {
              kend = k + b_i;
              b_V[kend - 1] = b_V[kend - 11];
            }
          }

          std::memset(&b_V[0], 0, 10U * sizeof(double));
          b_V[0] = 1.0;
          kend = 8;
          std::memset(&work[0], 0, 10U * sizeof(double));
          for (b_i = 8; b_i >= 0; b_i--) {
            iaii = (b_i + b_i * 10) + 11;
            if (b_i + 1 < 9) {
              b_V[iaii] = 1.0;
              internal::reflapack::xzlarf(9 - b_i, 8 - b_i, iaii + 1, tau[kend],
                b_V, iaii + 11, work);
              j = iaii + 2;
              i = (iaii - b_i) + 9;
              for (k = j; k <= i; k++) {
                b_V[k - 1] *= -tau[kend];
              }
            }

            b_V[iaii] = 1.0 - tau[kend];
            for (j = 0; j < b_i; j++) {
              b_V[(iaii - j) - 1] = 0.0;
            }

            kend--;
          }

          internal::lapack::xhseqr(b_A, b_V);
        }

        for (j = 0; j < 9; j++) {
          b_A[(j + 10 * j) + 1] = 0.0;
          for (b_i = 0; b_i <= j; b_i++) {
            b_A[b_i + 10 * (j + 1)] = 0.0;
          }
        }

        for (i = 0; i < 100; i++) {
          V[i].re = b_V[i];
          V[i].im = 0.0;
          D[i].re = b_A[i];
          D[i].im = 0.0;
        }
      } else {
        double ai;
        double ar;
        double scale;
        int i;
        for (i = 0; i < 100; i++) {
          At[i].re = A[i];
          At[i].im = 0.0;
        }

        internal::reflapack::xzggev(At, &kend, alpha1, beta1, V);
        for (iaii = 0; iaii <= 90; iaii += 10) {
          colnorm = 0.0;
          scale = 3.3121686421112381E-170;
          kend = iaii + 10;
          for (k = iaii + 1; k <= kend; k++) {
            absxk = std::abs(V[k - 1].re);
            if (absxk > scale) {
              t = scale / absxk;
              colnorm = colnorm * t * t + 1.0;
              scale = absxk;
            } else {
              t = absxk / scale;
              colnorm += t * t;
            }

            absxk = std::abs(V[k - 1].im);
            if (absxk > scale) {
              t = scale / absxk;
              colnorm = colnorm * t * t + 1.0;
              scale = absxk;
            } else {
              t = absxk / scale;
              colnorm += t * t;
            }
          }

          colnorm = scale * std::sqrt(colnorm);
          i = iaii + 10;
          for (j = iaii + 1; j <= i; j++) {
            ar = V[j - 1].re;
            ai = V[j - 1].im;
            if (ai == 0.0) {
              absxk = ar / colnorm;
              scale = 0.0;
            } else if (ar == 0.0) {
              absxk = 0.0;
              scale = ai / colnorm;
            } else {
              absxk = ar / colnorm;
              scale = ai / colnorm;
            }

            V[j - 1].re = absxk;
            V[j - 1].im = scale;
          }
        }

        std::memset(&D[0], 0, 100U * sizeof(creal_T));
        for (k = 0; k < 10; k++) {
          ar = alpha1[k].re;
          ai = alpha1[k].im;
          t = beta1[k].re;
          colnorm = beta1[k].im;
          if (colnorm == 0.0) {
            if (ai == 0.0) {
              i = k + 10 * k;
              D[i].re = ar / t;
              D[i].im = 0.0;
            } else if (ar == 0.0) {
              i = k + 10 * k;
              D[i].re = 0.0;
              D[i].im = ai / t;
            } else {
              i = k + 10 * k;
              D[i].re = ar / t;
              D[i].im = ai / t;
            }
          } else if (t == 0.0) {
            if (ar == 0.0) {
              i = k + 10 * k;
              D[i].re = ai / colnorm;
              D[i].im = 0.0;
            } else if (ai == 0.0) {
              i = k + 10 * k;
              D[i].re = 0.0;
              D[i].im = -(ar / colnorm);
            } else {
              i = k + 10 * k;
              D[i].re = ai / colnorm;
              D[i].im = -(ar / colnorm);
            }
          } else {
            double brm;
            brm = std::abs(t);
            scale = std::abs(colnorm);
            if (brm > scale) {
              absxk = colnorm / t;
              scale = t + absxk * colnorm;
              i = k + 10 * k;
              D[i].re = (ar + absxk * ai) / scale;
              D[i].im = (ai - absxk * ar) / scale;
            } else if (scale == brm) {
              if (t > 0.0) {
                absxk = 0.5;
              } else {
                absxk = -0.5;
              }

              if (colnorm > 0.0) {
                scale = 0.5;
              } else {
                scale = -0.5;
              }

              i = k + 10 * k;
              D[i].re = (ar * absxk + ai * scale) / brm;
              D[i].im = (ai * absxk - ar * scale) / brm;
            } else {
              absxk = t / colnorm;
              scale = colnorm + absxk * t;
              i = k + 10 * k;
              D[i].re = (absxk * ar + ai) / scale;
              D[i].im = (absxk * ai - ar) / scale;
            }
          }
        }
      }
    }
  }
}

//
// File trailer for eig.cpp
//
// [EOF]
//
