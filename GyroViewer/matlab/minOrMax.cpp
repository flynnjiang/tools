//
// File: minOrMax.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 03-Mar-2021 16:17:11
//

// Include Files
#include "minOrMax.h"
#include "magmcali_rtwutil.h"
#include "relop.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const creal_T x[10]
//                creal_T *ex
//                int *idx
// Return Type  : void
//
namespace coder
{
  namespace internal
  {
    void minimum(const creal_T x[10], creal_T *ex, int *idx)
    {
      *idx = 1;
      *ex = x[0];
      for (int k = 0; k < 9; k++) {
        creal_T dc;
        boolean_T SCALEA;
        dc = x[k + 1];
        if (rtIsNaN(dc.re) || rtIsNaN(x[k + 1].im)) {
          SCALEA = false;
        } else if (rtIsNaN(ex->re) || rtIsNaN(ex->im)) {
          SCALEA = true;
        } else {
          double absbi;
          double b_x;
          double ma;
          double mb;
          boolean_T SCALEB;
          ma = std::abs(ex->re);
          if ((ma > 8.9884656743115785E+307) || (std::abs(ex->im) >
               8.9884656743115785E+307)) {
            SCALEA = true;
          } else {
            SCALEA = false;
          }

          mb = std::abs(x[k + 1].re);
          if ((mb > 8.9884656743115785E+307) || (std::abs(x[k + 1].im) >
               8.9884656743115785E+307)) {
            SCALEB = true;
          } else {
            SCALEB = false;
          }

          if (SCALEA || SCALEB) {
            b_x = rt_hypotd_snf(ex->re / 2.0, ex->im / 2.0);
            absbi = rt_hypotd_snf(x[k + 1].re / 2.0, x[k + 1].im / 2.0);
          } else {
            b_x = rt_hypotd_snf(ex->re, ex->im);
            absbi = rt_hypotd_snf(x[k + 1].re, x[k + 1].im);
          }

          if (iseq(b_x, absbi)) {
            double Ma;
            double absai;
            absai = std::abs(ex->im);
            absbi = std::abs(x[k + 1].im);
            if (ma > absai) {
              Ma = ma;
              ma = absai;
            } else {
              Ma = absai;
            }

            if (mb > absbi) {
              absai = mb;
              mb = absbi;
            } else {
              absai = absbi;
            }

            if (Ma > absai) {
              if (ma < mb) {
                b_x = Ma - absai;
                absbi = (ma / 2.0 + mb / 2.0) / (Ma / 2.0 + absai / 2.0) * (mb -
                  ma);
              } else {
                b_x = Ma;
                absbi = absai;
              }
            } else if (Ma < absai) {
              if (ma > mb) {
                absbi = absai - Ma;
                b_x = (ma / 2.0 + mb / 2.0) / (Ma / 2.0 + absai / 2.0) * (ma -
                  mb);
              } else {
                b_x = Ma;
                absbi = absai;
              }
            } else {
              b_x = ma;
              absbi = mb;
            }

            if (iseq(b_x, absbi)) {
              b_x = rt_atan2d_snf(ex->im, ex->re);
              absbi = rt_atan2d_snf(x[k + 1].im, x[k + 1].re);
              if (iseq(b_x, absbi)) {
                absbi = x[k + 1].re;
                absai = x[k + 1].im;
                if (b_x > 0.78539816339744828) {
                  if (b_x > 2.3561944901923448) {
                    b_x = -ex->im;
                    absbi = -absai;
                  } else {
                    b_x = -ex->re;
                    absbi = -absbi;
                  }
                } else if (b_x > -0.78539816339744828) {
                  b_x = ex->im;
                  absbi = absai;
                } else if (b_x > -2.3561944901923448) {
                  b_x = ex->re;
                } else {
                  b_x = -ex->im;
                  absbi = -absai;
                }

                if (iseq(b_x, absbi)) {
                  b_x = 0.0;
                  absbi = 0.0;
                }
              }
            }
          }

          SCALEA = (b_x > absbi);
        }

        if (SCALEA) {
          *ex = dc;
          *idx = k + 2;
        }
      }
    }
  }
}

//
// File trailer for minOrMax.cpp
//
// [EOF]
//
