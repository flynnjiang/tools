//
// File: magmcali.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 03-Mar-2021 16:17:11
//

// Include Files
#include "magmcali.h"
#include "eig.h"
#include "magmcali_data.h"
#include "magmcali_rtwutil.h"
#include "minOrMax.h"
#include "rt_nonfinite.h"
#include "schur.h"
#include "sqrt.h"
#include "coder_array.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <cstring>

// Function Declarations
static double rt_powd_snf(double u0, double u1);

// Function Definitions
//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_powd_snf(double u0, double u1)
{
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    double d;
    double d1;
    d = std::abs(u0);
    d1 = std::abs(u1);
    if (rtIsInf(u1)) {
      if (d == 1.0) {
        y = 1.0;
      } else if (d > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = std::sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > std::floor(u1))) {
      y = rtNaN;
    } else {
      y = std::pow(u0, u1);
    }
  }

  return y;
}

//
// Arguments    : const coder::array<double, 1U> &x
//                const coder::array<double, 1U> &y
//                const coder::array<double, 1U> &z
//                creal_T Winv[9]
//                creal_T V[3]
// Return Type  : void
//
void magmcali(const coder::array<double, 1U> &x, const coder::array<double, 1U>
              &y, const coder::array<double, 1U> &z, creal_T Winv[9], creal_T V
              [3])
{
  static const signed char iv[9] = { 0, 1, 2, 1, 3, 4, 2, 4, 5 };

  coder::array<double, 2U> d;
  coder::array<double, 1U> r;
  creal_T evc[100];
  creal_T evlmtx[100];
  creal_T eigvals[10];
  creal_T A[9];
  creal_T Q[9];
  creal_T b_x[9];
  creal_T temp;
  double dtd[100];
  double a21;
  double ai;
  double b_d;
  double b_im;
  double bi;
  double bkj;
  double br;
  double brm;
  double d1;
  double im;
  double re;
  double s;
  int boffset;
  int coffset;
  int i;
  int iindx;
  int ix;
  int j;
  int jA;
  int jy;
  int k;
  signed char ipiv[3];
  boolean_T isodd;
  r.set_size(x.size(0));
  jy = x.size(0);
  for (i = 0; i < jy; i++) {
    r[i] = 2.0 * x[i];
  }

  d.set_size(x.size(0), 10);
  jy = x.size(0);
  for (i = 0; i < jy; i++) {
    d[i] = x[i] * x[i];
  }

  jy = r.size(0);
  for (i = 0; i < jy; i++) {
    d[i + d.size(0)] = r[i] * y[i];
  }

  jy = r.size(0);
  for (i = 0; i < jy; i++) {
    d[i + d.size(0) * 2] = r[i] * z[i];
  }

  jy = y.size(0);
  for (i = 0; i < jy; i++) {
    d[i + d.size(0) * 3] = y[i] * y[i];
  }

  jy = y.size(0);
  for (i = 0; i < jy; i++) {
    d[i + d.size(0) * 4] = 2.0 * y[i] * z[i];
  }

  jy = z.size(0);
  for (i = 0; i < jy; i++) {
    d[i + d.size(0) * 5] = z[i] * z[i];
  }

  jy = x.size(0);
  for (i = 0; i < jy; i++) {
    d[i + d.size(0) * 6] = x[i];
  }

  jy = y.size(0);
  for (i = 0; i < jy; i++) {
    d[i + d.size(0) * 7] = y[i];
  }

  jy = z.size(0);
  for (i = 0; i < jy; i++) {
    d[i + d.size(0) * 8] = z[i];
  }

  jy = x.size(0);
  for (i = 0; i < jy; i++) {
    d[i + d.size(0) * 9] = 1.0;
  }

  jy = d.size(0);
  for (j = 0; j < 10; j++) {
    coffset = j * 10;
    boffset = j * d.size(0);
    std::memset(&dtd[coffset], 0, 10U * sizeof(double));
    for (k = 0; k < jy; k++) {
      bkj = d[boffset + k];
      for (ix = 0; ix < 10; ix++) {
        jA = coffset + ix;
        dtd[jA] += d[ix * d.size(0) + k] * bkj;
      }
    }
  }

  coder::eig(dtd, evc, evlmtx);
  for (k = 0; k < 10; k++) {
    eigvals[k] = evlmtx[k + 10 * k];
  }

  coder::internal::minimum(eigvals, &temp, &iindx);
  std::memcpy(&eigvals[0], &evc[iindx * 10 + -10], 10U * sizeof(creal_T));

  // solution has smallest eigenvalue
  jA = 10 * (iindx - 1);
  for (i = 0; i < 3; i++) {
    A[3 * i] = evc[iv[3 * i] + jA];
    jy = 3 * i + 1;
    A[jy] = evc[iv[jy] + jA];
    jy = 3 * i + 2;
    A[jy] = evc[iv[jy] + jA];
  }

  // make symmetric
  std::memcpy(&b_x[0], &A[0], 9U * sizeof(creal_T));
  ipiv[0] = 1;
  ipiv[1] = 2;
  for (j = 0; j < 2; j++) {
    int b_tmp;
    int mmj_tmp;
    mmj_tmp = 1 - j;
    b_tmp = j << 2;
    coffset = b_tmp + 2;
    jy = 3 - j;
    jA = 0;
    ix = b_tmp;
    bkj = std::abs(b_x[b_tmp].re) + std::abs(b_x[b_tmp].im);
    for (k = 2; k <= jy; k++) {
      ix++;
      s = std::abs(b_x[ix].re) + std::abs(b_x[ix].im);
      if (s > bkj) {
        jA = k - 1;
        bkj = s;
      }
    }

    jy = b_tmp + jA;
    if ((b_x[jy].re != 0.0) || (b_x[jy].im != 0.0)) {
      if (jA != 0) {
        jy = j + jA;
        ipiv[j] = static_cast<signed char>(jy + 1);
        temp = b_x[j];
        b_x[j] = b_x[jy];
        b_x[jy] = temp;
        temp = b_x[j + 3];
        b_x[j + 3] = b_x[jy + 3];
        b_x[jy + 3] = temp;
        temp = b_x[j + 6];
        b_x[j + 6] = b_x[jy + 6];
        b_x[jy + 6] = temp;
      }

      i = (b_tmp - j) + 3;
      for (ix = coffset; ix <= i; ix++) {
        a21 = b_x[ix - 1].re;
        ai = b_x[ix - 1].im;
        br = b_x[b_tmp].re;
        bi = b_x[b_tmp].im;
        if (bi == 0.0) {
          if (ai == 0.0) {
            re = a21 / br;
            im = 0.0;
          } else if (a21 == 0.0) {
            re = 0.0;
            im = ai / br;
          } else {
            re = a21 / br;
            im = ai / br;
          }
        } else if (br == 0.0) {
          if (a21 == 0.0) {
            re = ai / bi;
            im = 0.0;
          } else if (ai == 0.0) {
            re = 0.0;
            im = -(a21 / bi);
          } else {
            re = ai / bi;
            im = -(a21 / bi);
          }
        } else {
          brm = std::abs(br);
          bkj = std::abs(bi);
          if (brm > bkj) {
            s = bi / br;
            bkj = br + s * bi;
            re = (a21 + s * ai) / bkj;
            im = (ai - s * a21) / bkj;
          } else if (bkj == brm) {
            if (br > 0.0) {
              s = 0.5;
            } else {
              s = -0.5;
            }

            if (bi > 0.0) {
              bkj = 0.5;
            } else {
              bkj = -0.5;
            }

            re = (a21 * s + ai * bkj) / brm;
            im = (ai * s - a21 * bkj) / brm;
          } else {
            s = br / bi;
            bkj = bi + s * br;
            re = (s * a21 + ai) / bkj;
            im = (s * ai - a21) / bkj;
          }
        }

        b_x[ix - 1].re = re;
        b_x[ix - 1].im = im;
      }
    }

    jy = b_tmp + 3;
    jA = b_tmp;
    for (coffset = 0; coffset <= mmj_tmp; coffset++) {
      if ((b_x[jy].re != 0.0) || (b_x[jy].im != 0.0)) {
        temp.re = -b_x[jy].re - b_x[jy].im * 0.0;
        temp.im = b_x[jy].re * 0.0 + -b_x[jy].im;
        ix = b_tmp + 1;
        i = jA + 5;
        k = (jA - j) + 6;
        for (boffset = i; boffset <= k; boffset++) {
          bkj = b_x[ix].re * temp.im + b_x[ix].im * temp.re;
          b_x[boffset - 1].re += b_x[ix].re * temp.re - b_x[ix].im * temp.im;
          b_x[boffset - 1].im += bkj;
          ix++;
        }
      }

      jy += 3;
      jA += 3;
    }
  }

  temp.re = b_x[0].re * b_x[4].re - b_x[0].im * b_x[4].im;
  temp.im = b_x[0].re * b_x[4].im + b_x[0].im * b_x[4].re;
  isodd = (ipiv[0] > 1);
  re = temp.re * b_x[8].re - temp.im * b_x[8].im;
  im = temp.re * b_x[8].im + temp.im * b_x[8].re;
  temp.re = re;
  temp.im = im;
  if (ipiv[1] > 2) {
    isodd = !isodd;
  }

  if (isodd) {
    temp.re = -re;
    temp.im = -im;
  }

  if (temp.re < 0.0) {
    for (i = 0; i < 9; i++) {
      A[i].re = -A[i].re;
      A[i].im = -A[i].im;
    }

    for (i = 0; i < 10; i++) {
      k = i + 10 * (iindx - 1);
      eigvals[i].re = -evc[k].re;
      eigvals[i].im = -evc[k].im;
    }

    temp.re = -temp.re;
    temp.im = -temp.im;

    // Compensate for -A.
  }

  std::memcpy(&b_x[0], &A[0], 9U * sizeof(creal_T));
  jy = 0;
  coffset = 1;
  boffset = 2;
  bkj = std::abs(A[0].re) + std::abs(A[0].im);
  a21 = std::abs(A[1].re) + std::abs(A[1].im);
  if (a21 > bkj) {
    bkj = a21;
    jy = 1;
    coffset = 0;
  }

  if (std::abs(A[2].re) + std::abs(A[2].im) > bkj) {
    jy = 2;
    coffset = 1;
    boffset = 0;
  }

  a21 = A[coffset].re;
  ai = A[coffset].im;
  br = A[jy].re;
  bi = A[jy].im;
  if (bi == 0.0) {
    if (ai == 0.0) {
      b_x[coffset].re = a21 / br;
      b_x[coffset].im = 0.0;
    } else if (a21 == 0.0) {
      b_x[coffset].re = 0.0;
      b_x[coffset].im = ai / br;
    } else {
      b_x[coffset].re = a21 / br;
      b_x[coffset].im = ai / br;
    }
  } else if (br == 0.0) {
    if (a21 == 0.0) {
      b_x[coffset].re = ai / bi;
      b_x[coffset].im = 0.0;
    } else if (ai == 0.0) {
      b_x[coffset].re = 0.0;
      b_x[coffset].im = -(a21 / bi);
    } else {
      b_x[coffset].re = ai / bi;
      b_x[coffset].im = -(a21 / bi);
    }
  } else {
    brm = std::abs(br);
    bkj = std::abs(bi);
    if (brm > bkj) {
      s = bi / br;
      bkj = br + s * bi;
      b_x[coffset].re = (a21 + s * ai) / bkj;
      b_x[coffset].im = (ai - s * a21) / bkj;
    } else if (bkj == brm) {
      if (br > 0.0) {
        s = 0.5;
      } else {
        s = -0.5;
      }

      if (bi > 0.0) {
        bkj = 0.5;
      } else {
        bkj = -0.5;
      }

      b_x[coffset].re = (a21 * s + ai * bkj) / brm;
      b_x[coffset].im = (ai * s - a21 * bkj) / brm;
    } else {
      s = br / bi;
      bkj = bi + s * br;
      b_x[coffset].re = (s * a21 + ai) / bkj;
      b_x[coffset].im = (s * ai - a21) / bkj;
    }
  }

  a21 = b_x[boffset].re;
  ai = b_x[boffset].im;
  br = b_x[jy].re;
  bi = b_x[jy].im;
  if (bi == 0.0) {
    if (ai == 0.0) {
      re = a21 / br;
      im = 0.0;
    } else if (a21 == 0.0) {
      re = 0.0;
      im = ai / br;
    } else {
      re = a21 / br;
      im = ai / br;
    }
  } else if (br == 0.0) {
    if (a21 == 0.0) {
      re = ai / bi;
      im = 0.0;
    } else if (ai == 0.0) {
      re = 0.0;
      im = -(a21 / bi);
    } else {
      re = ai / bi;
      im = -(a21 / bi);
    }
  } else {
    brm = std::abs(br);
    bkj = std::abs(bi);
    if (brm > bkj) {
      s = bi / br;
      bkj = br + s * bi;
      re = (a21 + s * ai) / bkj;
      im = (ai - s * a21) / bkj;
    } else if (bkj == brm) {
      if (br > 0.0) {
        s = 0.5;
      } else {
        s = -0.5;
      }

      if (bi > 0.0) {
        bkj = 0.5;
      } else {
        bkj = -0.5;
      }

      re = (a21 * s + ai * bkj) / brm;
      im = (ai * s - a21 * bkj) / brm;
    } else {
      s = br / bi;
      bkj = bi + s * br;
      re = (s * a21 + ai) / bkj;
      im = (s * ai - a21) / bkj;
    }
  }

  b_x[boffset].re = re;
  b_x[boffset].im = im;
  bkj = b_x[coffset].re * b_x[jy + 3].im + b_x[coffset].im * b_x[jy + 3].re;
  b_x[coffset + 3].re -= b_x[coffset].re * b_x[jy + 3].re - b_x[coffset].im *
    b_x[jy + 3].im;
  b_x[coffset + 3].im -= bkj;
  bkj = b_x[boffset].re * b_x[jy + 3].im + b_x[boffset].im * b_x[jy + 3].re;
  b_x[boffset + 3].re -= b_x[boffset].re * b_x[jy + 3].re - b_x[boffset].im *
    b_x[jy + 3].im;
  b_x[boffset + 3].im -= bkj;
  bkj = b_x[coffset].re * b_x[jy + 6].im + b_x[coffset].im * b_x[jy + 6].re;
  b_x[coffset + 6].re -= b_x[coffset].re * b_x[jy + 6].re - b_x[coffset].im *
    b_x[jy + 6].im;
  b_x[coffset + 6].im -= bkj;
  bkj = b_x[boffset].re * b_x[jy + 6].im + b_x[boffset].im * b_x[jy + 6].re;
  b_x[boffset + 6].re -= b_x[boffset].re * b_x[jy + 6].re - b_x[boffset].im *
    b_x[jy + 6].im;
  b_x[boffset + 6].im -= bkj;
  if (std::abs(b_x[boffset + 3].re) + std::abs(b_x[boffset + 3].im) > std::abs
      (b_x[coffset + 3].re) + std::abs(b_x[coffset + 3].im)) {
    jA = coffset;
    coffset = boffset;
    boffset = jA;
  }

  a21 = b_x[boffset + 3].re;
  ai = b_x[boffset + 3].im;
  br = b_x[coffset + 3].re;
  bi = b_x[coffset + 3].im;
  if (bi == 0.0) {
    if (ai == 0.0) {
      re = a21 / br;
      im = 0.0;
    } else if (a21 == 0.0) {
      re = 0.0;
      im = ai / br;
    } else {
      re = a21 / br;
      im = ai / br;
    }
  } else if (br == 0.0) {
    if (a21 == 0.0) {
      re = ai / bi;
      im = 0.0;
    } else if (ai == 0.0) {
      re = 0.0;
      im = -(a21 / bi);
    } else {
      re = ai / bi;
      im = -(a21 / bi);
    }
  } else {
    brm = std::abs(br);
    bkj = std::abs(bi);
    if (brm > bkj) {
      s = bi / br;
      bkj = br + s * bi;
      re = (a21 + s * ai) / bkj;
      im = (ai - s * a21) / bkj;
    } else if (bkj == brm) {
      if (br > 0.0) {
        s = 0.5;
      } else {
        s = -0.5;
      }

      if (bi > 0.0) {
        bkj = 0.5;
      } else {
        bkj = -0.5;
      }

      re = (a21 * s + ai * bkj) / brm;
      im = (ai * s - a21 * bkj) / brm;
    } else {
      s = br / bi;
      bkj = bi + s * br;
      re = (s * a21 + ai) / bkj;
      im = (s * ai - a21) / bkj;
    }
  }

  b_x[boffset + 3].re = re;
  b_x[boffset + 3].im = im;
  bkj = b_x[boffset + 3].re * b_x[coffset + 6].im + b_x[boffset + 3].im *
    b_x[coffset + 6].re;
  b_x[boffset + 6].re -= b_x[boffset + 3].re * b_x[coffset + 6].re - b_x[boffset
    + 3].im * b_x[coffset + 6].im;
  b_x[boffset + 6].im -= bkj;
  V[1].re = eigvals[coffset + 6].re - (eigvals[jy + 6].re * b_x[coffset].re -
    eigvals[jy + 6].im * b_x[coffset].im);
  V[1].im = eigvals[coffset + 6].im - (eigvals[jy + 6].re * b_x[coffset].im +
    eigvals[jy + 6].im * b_x[coffset].re);
  V[2].re = (eigvals[boffset + 6].re - (eigvals[jy + 6].re * b_x[boffset].re -
              eigvals[jy + 6].im * b_x[boffset].im)) - (V[1].re * b_x[boffset +
    3].re - V[1].im * b_x[boffset + 3].im);
  V[2].im = (eigvals[boffset + 6].im - (eigvals[jy + 6].re * b_x[boffset].im +
              eigvals[jy + 6].im * b_x[boffset].re)) - (V[1].re * b_x[boffset +
    3].im + V[1].im * b_x[boffset + 3].re);
  br = b_x[boffset + 6].re;
  bi = b_x[boffset + 6].im;
  if (bi == 0.0) {
    if (V[2].im == 0.0) {
      re = V[2].re / br;
      im = 0.0;
    } else if (V[2].re == 0.0) {
      re = 0.0;
      im = V[2].im / br;
    } else {
      re = V[2].re / br;
      im = V[2].im / br;
    }
  } else if (br == 0.0) {
    if (V[2].re == 0.0) {
      re = V[2].im / bi;
      im = 0.0;
    } else if (V[2].im == 0.0) {
      re = 0.0;
      im = -(V[2].re / bi);
    } else {
      re = V[2].im / bi;
      im = -(V[2].re / bi);
    }
  } else {
    brm = std::abs(br);
    bkj = std::abs(bi);
    if (brm > bkj) {
      s = bi / br;
      bkj = br + s * bi;
      re = (V[2].re + s * V[2].im) / bkj;
      im = (V[2].im - s * V[2].re) / bkj;
    } else if (bkj == brm) {
      if (br > 0.0) {
        s = 0.5;
      } else {
        s = -0.5;
      }

      if (bi > 0.0) {
        bkj = 0.5;
      } else {
        bkj = -0.5;
      }

      re = (V[2].re * s + V[2].im * bkj) / brm;
      im = (V[2].im * s - V[2].re * bkj) / brm;
    } else {
      s = br / bi;
      bkj = bi + s * br;
      re = (s * V[2].re + V[2].im) / bkj;
      im = (s * V[2].im - V[2].re) / bkj;
    }
  }

  V[1].re -= re * b_x[coffset + 6].re - im * b_x[coffset + 6].im;
  V[1].im -= re * b_x[coffset + 6].im + im * b_x[coffset + 6].re;
  br = b_x[coffset + 3].re;
  bi = b_x[coffset + 3].im;
  if (bi == 0.0) {
    if (V[1].im == 0.0) {
      ai = V[1].re / br;
      b_im = 0.0;
    } else if (V[1].re == 0.0) {
      ai = 0.0;
      b_im = V[1].im / br;
    } else {
      ai = V[1].re / br;
      b_im = V[1].im / br;
    }
  } else if (br == 0.0) {
    if (V[1].re == 0.0) {
      ai = V[1].im / bi;
      b_im = 0.0;
    } else if (V[1].im == 0.0) {
      ai = 0.0;
      b_im = -(V[1].re / bi);
    } else {
      ai = V[1].im / bi;
      b_im = -(V[1].re / bi);
    }
  } else {
    brm = std::abs(br);
    bkj = std::abs(bi);
    if (brm > bkj) {
      s = bi / br;
      bkj = br + s * bi;
      ai = (V[1].re + s * V[1].im) / bkj;
      b_im = (V[1].im - s * V[1].re) / bkj;
    } else if (bkj == brm) {
      if (br > 0.0) {
        s = 0.5;
      } else {
        s = -0.5;
      }

      if (bi > 0.0) {
        bkj = 0.5;
      } else {
        bkj = -0.5;
      }

      ai = (V[1].re * s + V[1].im * bkj) / brm;
      b_im = (V[1].im * s - V[1].re * bkj) / brm;
    } else {
      s = br / bi;
      bkj = bi + s * br;
      ai = (s * V[1].re + V[1].im) / bkj;
      b_im = (s * V[1].im - V[1].re) / bkj;
    }
  }

  V[0].re = (eigvals[jy + 6].re - (re * b_x[jy + 6].re - im * b_x[jy + 6].im)) -
    (ai * b_x[jy + 3].re - b_im * b_x[jy + 3].im);
  V[0].im = (eigvals[jy + 6].im - (re * b_x[jy + 6].im + im * b_x[jy + 6].re)) -
    (ai * b_x[jy + 3].im + b_im * b_x[jy + 3].re);
  br = b_x[jy].re;
  bi = b_x[jy].im;
  if (bi == 0.0) {
    if (V[0].im == 0.0) {
      a21 = V[0].re / br;
      bkj = 0.0;
    } else if (V[0].re == 0.0) {
      a21 = 0.0;
      bkj = V[0].im / br;
    } else {
      a21 = V[0].re / br;
      bkj = V[0].im / br;
    }
  } else if (br == 0.0) {
    if (V[0].re == 0.0) {
      a21 = V[0].im / bi;
      bkj = 0.0;
    } else if (V[0].im == 0.0) {
      a21 = 0.0;
      bkj = -(V[0].re / bi);
    } else {
      a21 = V[0].im / bi;
      bkj = -(V[0].re / bi);
    }
  } else {
    brm = std::abs(br);
    bkj = std::abs(bi);
    if (brm > bkj) {
      s = bi / br;
      bkj = br + s * bi;
      a21 = (V[0].re + s * V[0].im) / bkj;
      bkj = (V[0].im - s * V[0].re) / bkj;
    } else if (bkj == brm) {
      if (br > 0.0) {
        s = 0.5;
      } else {
        s = -0.5;
      }

      if (bi > 0.0) {
        bkj = 0.5;
      } else {
        bkj = -0.5;
      }

      a21 = (V[0].re * s + V[0].im * bkj) / brm;
      bkj = (V[0].im * s - V[0].re * bkj) / brm;
    } else {
      s = br / bi;
      bkj = bi + s * br;
      a21 = (s * V[0].re + V[0].im) / bkj;
      bkj = (s * V[0].im - V[0].re) / bkj;
    }
  }

  V[0].re = -0.5 * a21;
  V[0].im = -0.5 * bkj;
  V[1].re = -0.5 * ai;
  V[1].im = -0.5 * b_im;
  V[2].re = -0.5 * re;
  V[2].im = -0.5 * im;

  // hard iron offset
  //  We correct Winv and B by det(A) because we don't know which has the
  //  gain. By convention, normalize A.
  a21 = temp.re;
  if ((temp.im == 0.0) && (temp.re >= 0.0)) {
    temp.re = rt_powd_snf(a21, 0.33333333333333331);
    temp.im = 0.0;
  } else {
    if (temp.im == 0.0) {
      b_d = temp.re;
      temp.re = std::log(b_d);
      temp.im = 0.0;
    } else if ((temp.re > 8.9884656743115785E+307) || (temp.im >
                8.9884656743115785E+307)) {
      b_d = temp.re;
      d1 = temp.im;
      brm = temp.im;
      b_im = temp.re;
      temp.re = std::log(rt_hypotd_snf(b_d / 2.0, d1 / 2.0)) +
        0.69314718055994529;
      temp.im = rt_atan2d_snf(brm, b_im);
    } else {
      b_d = temp.re;
      d1 = temp.im;
      brm = temp.im;
      b_im = temp.re;
      temp.re = std::log(rt_hypotd_snf(b_d, d1));
      temp.im = rt_atan2d_snf(brm, b_im);
    }

    temp.re *= 0.33333333333333331;
    temp.im *= 0.33333333333333331;
    if (temp.im == 0.0) {
      b_d = temp.re;
      temp.re = std::exp(b_d);
      temp.im = 0.0;
    } else if (rtIsInf(temp.im) && rtIsInf(temp.re) && (temp.re < 0.0)) {
      temp.re = 0.0;
      temp.im = 0.0;
    } else {
      bkj = std::exp(temp.re / 2.0);
      b_d = temp.im;
      d1 = temp.im;
      temp.re = bkj * (bkj * std::cos(b_d));
      temp.im = bkj * (bkj * std::sin(d1));
    }
  }

  for (i = 0; i < 9; i++) {
    a21 = A[i].re;
    ai = A[i].im;
    if (temp.im == 0.0) {
      if (ai == 0.0) {
        re = a21 / temp.re;
        im = 0.0;
      } else if (a21 == 0.0) {
        re = 0.0;
        im = ai / temp.re;
      } else {
        re = a21 / temp.re;
        im = ai / temp.re;
      }
    } else if (temp.re == 0.0) {
      if (a21 == 0.0) {
        re = ai / temp.im;
        im = 0.0;
      } else if (ai == 0.0) {
        re = 0.0;
        im = -(a21 / temp.im);
      } else {
        re = ai / temp.im;
        im = -(a21 / temp.im);
      }
    } else {
      brm = std::abs(temp.re);
      bkj = std::abs(temp.im);
      if (brm > bkj) {
        s = temp.im / temp.re;
        bkj = temp.re + s * temp.im;
        re = (a21 + s * ai) / bkj;
        im = (ai - s * a21) / bkj;
      } else if (bkj == brm) {
        if (temp.re > 0.0) {
          s = 0.5;
        } else {
          s = -0.5;
        }

        if (temp.im > 0.0) {
          bkj = 0.5;
        } else {
          bkj = -0.5;
        }

        re = (a21 * s + ai * bkj) / brm;
        im = (ai * s - a21 * bkj) / brm;
      } else {
        s = temp.re / temp.im;
        bkj = temp.im + s * temp.re;
        re = (s * a21 + ai) / bkj;
        im = (s * ai - a21) / bkj;
      }
    }

    A[i].re = re;
    A[i].im = im;
  }

  coder::schur(A, Q, b_x);
  std::memset(&A[0], 0, 9U * sizeof(creal_T));
  j = -1;
  int exitg2;
  do {
    exitg2 = 0;
    if (j + 1 < 3) {
      int exitg1;
      ix = 0;
      do {
        exitg1 = 0;
        if (ix <= j) {
          jy = ix + 3 * (j + 1);
          if ((b_x[jy].re != 0.0) || (b_x[jy].im != 0.0)) {
            for (j = 0; j < 3; j++) {
              jA = j + 3 * j;
              A[jA] = b_x[jA];
              coder::b_sqrt(&A[j + 3 * j]);
              for (ix = j; ix >= 1; ix--) {
                temp.re = 0.0;
                temp.im = 0.0;
                i = ix + 1;
                for (k = i; k <= j; k++) {
                  jy = 3 * j + 1;
                  temp.re += A[ix + 2].re * A[jy].re - A[ix + 2].im * A[jy].im;
                  temp.im += A[ix + 2].re * A[jy].im + A[ix + 2].im * A[jy].re;
                }

                jy = (ix + 3 * j) - 1;
                a21 = b_x[jy].re - temp.re;
                ai = b_x[jy].im - temp.im;
                coffset = (ix + 3 * (ix - 1)) - 1;
                br = A[coffset].re + A[jA].re;
                bi = A[coffset].im + A[jA].im;
                if (bi == 0.0) {
                  if (ai == 0.0) {
                    re = a21 / br;
                    im = 0.0;
                  } else if (a21 == 0.0) {
                    re = 0.0;
                    im = ai / br;
                  } else {
                    re = a21 / br;
                    im = ai / br;
                  }
                } else if (br == 0.0) {
                  if (a21 == 0.0) {
                    re = ai / bi;
                    im = 0.0;
                  } else if (ai == 0.0) {
                    re = 0.0;
                    im = -(a21 / bi);
                  } else {
                    re = ai / bi;
                    im = -(a21 / bi);
                  }
                } else {
                  brm = std::abs(br);
                  bkj = std::abs(bi);
                  if (brm > bkj) {
                    s = bi / br;
                    bkj = br + s * bi;
                    re = (a21 + s * ai) / bkj;
                    im = (ai - s * a21) / bkj;
                  } else if (bkj == brm) {
                    if (br > 0.0) {
                      s = 0.5;
                    } else {
                      s = -0.5;
                    }

                    if (bi > 0.0) {
                      bkj = 0.5;
                    } else {
                      bkj = -0.5;
                    }

                    re = (a21 * s + ai * bkj) / brm;
                    im = (ai * s - a21 * bkj) / brm;
                  } else {
                    s = br / bi;
                    bkj = bi + s * br;
                    re = (s * a21 + ai) / bkj;
                    im = (s * ai - a21) / bkj;
                  }
                }

                A[jy].re = re;
                A[jy].im = im;
              }
            }

            exitg1 = 1;
          } else {
            ix++;
          }
        } else {
          j++;
          exitg1 = 2;
        }
      } while (exitg1 == 0);

      if (exitg1 == 1) {
        exitg2 = 1;
      }
    } else {
      A[0] = b_x[0];
      coder::b_sqrt(&A[0]);
      A[4] = b_x[4];
      coder::b_sqrt(&A[4]);
      A[8] = b_x[8];
      coder::b_sqrt(&A[8]);
      exitg2 = 1;
    }
  } while (exitg2 == 0);

  for (i = 0; i < 3; i++) {
    double d2;
    double d3;
    b_d = Q[i].re;
    d1 = Q[i].im;
    brm = Q[i + 3].re;
    b_im = Q[i + 3].im;
    d2 = Q[i + 6].re;
    d3 = Q[i + 6].im;
    for (k = 0; k < 3; k++) {
      jy = 3 * k + 1;
      coffset = 3 * k + 2;
      boffset = i + 3 * k;
      bkj = A[3 * k].re;
      a21 = A[3 * k].im;
      ai = A[jy].re;
      s = A[jy].im;
      br = A[coffset].re;
      bi = A[coffset].im;
      b_x[boffset].re = ((b_d * bkj - d1 * a21) + (brm * ai - b_im * s)) + (d2 *
        br - d3 * bi);
      b_x[boffset].im = ((b_d * a21 + d1 * bkj) + (brm * s + b_im * ai)) + (d2 *
        bi + d3 * br);
    }

    b_d = b_x[i].re;
    d1 = b_x[i].im;
    brm = b_x[i + 3].re;
    b_im = b_x[i + 3].im;
    d2 = b_x[i + 6].re;
    d3 = b_x[i + 6].im;
    for (k = 0; k < 3; k++) {
      bkj = Q[k].re;
      a21 = -Q[k].im;
      re = b_d * bkj - d1 * a21;
      im = b_d * a21 + d1 * bkj;
      bkj = Q[k + 3].re;
      a21 = -Q[k + 3].im;
      re += brm * bkj - b_im * a21;
      im += brm * a21 + b_im * bkj;
      bkj = Q[k + 6].re;
      a21 = -Q[k + 6].im;
      re += d2 * bkj - d3 * a21;
      im += d2 * a21 + d3 * bkj;
      boffset = i + 3 * k;
      Winv[boffset].re = re;
      Winv[boffset].im = im;
    }
  }
}

//
// File trailer for magmcali.cpp
//
// [EOF]
//
