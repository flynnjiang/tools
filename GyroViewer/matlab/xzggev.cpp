//
// File: xzggev.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 03-Mar-2021 16:17:11
//

// Include Files
#include "xzggev.h"
#include "magmcali_rtwutil.h"
#include "rt_nonfinite.h"
#include "xzhgeqz.h"
#include "xzlartg.h"
#include "xztgevc.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : creal_T A[100]
//                int *info
//                creal_T alpha1[10]
//                creal_T beta1[10]
//                creal_T V[100]
// Return Type  : void
//
namespace coder
{
  namespace internal
  {
    namespace reflapack
    {
      void xzggev(creal_T A[100], int *info, creal_T alpha1[10], creal_T beta1
                  [10], creal_T V[100])
      {
        creal_T atmp;
        double absxk;
        double anrm;
        int rscale[10];
        int jcol;
        signed char b_I[100];
        boolean_T exitg1;
        *info = 0;
        anrm = 0.0;
        jcol = 0;
        exitg1 = false;
        while ((!exitg1) && (jcol < 100)) {
          absxk = rt_hypotd_snf(A[jcol].re, A[jcol].im);
          if (rtIsNaN(absxk)) {
            anrm = rtNaN;
            exitg1 = true;
          } else {
            if (absxk > anrm) {
              anrm = absxk;
            }

            jcol++;
          }
        }

        if (rtIsInf(anrm) || rtIsNaN(anrm)) {
          for (int i = 0; i < 10; i++) {
            alpha1[i].re = rtNaN;
            alpha1[i].im = 0.0;
            beta1[i].re = rtNaN;
            beta1[i].im = 0.0;
          }

          for (int jrow = 0; jrow < 100; jrow++) {
            V[jrow].re = rtNaN;
            V[jrow].im = 0.0;
          }
        } else {
          double a;
          double anrmto;
          double cto1;
          double ctoc;
          double stemp_im;
          int A_tmp;
          int exitg3;
          int i;
          int ihi;
          int ii;
          int ilo;
          int j;
          int jcolp1;
          int jrow;
          int nzcount;
          boolean_T exitg4;
          boolean_T guard1 = false;
          boolean_T ilascl;
          boolean_T notdone;
          ilascl = false;
          anrmto = anrm;
          guard1 = false;
          if ((anrm > 0.0) && (anrm < 6.7178761075670888E-139)) {
            anrmto = 6.7178761075670888E-139;
            ilascl = true;
            guard1 = true;
          } else {
            if (anrm > 1.4885657073574029E+138) {
              anrmto = 1.4885657073574029E+138;
              ilascl = true;
              guard1 = true;
            }
          }

          if (guard1) {
            absxk = anrm;
            ctoc = anrmto;
            notdone = true;
            while (notdone) {
              stemp_im = absxk * 2.0041683600089728E-292;
              cto1 = ctoc / 4.9896007738368E+291;
              if ((stemp_im > ctoc) && (ctoc != 0.0)) {
                a = 2.0041683600089728E-292;
                absxk = stemp_im;
              } else if (cto1 > absxk) {
                a = 4.9896007738368E+291;
                ctoc = cto1;
              } else {
                a = ctoc / absxk;
                notdone = false;
              }

              for (jrow = 0; jrow < 100; jrow++) {
                A[jrow].re *= a;
                A[jrow].im *= a;
              }
            }
          }

          for (i = 0; i < 10; i++) {
            rscale[i] = 1;
          }

          ilo = 1;
          ihi = 10;
          do {
            exitg3 = 0;
            i = 0;
            j = 0;
            notdone = false;
            ii = ihi;
            exitg1 = false;
            while ((!exitg1) && (ii > 0)) {
              nzcount = 0;
              i = ii;
              j = ihi;
              jcol = 0;
              exitg4 = false;
              while ((!exitg4) && (jcol <= ihi - 1)) {
                A_tmp = (ii + 10 * jcol) - 1;
                if ((A[A_tmp].re != 0.0) || (A[A_tmp].im != 0.0) || (ii == jcol
                     + 1)) {
                  if (nzcount == 0) {
                    j = jcol + 1;
                    nzcount = 1;
                    jcol++;
                  } else {
                    nzcount = 2;
                    exitg4 = true;
                  }
                } else {
                  jcol++;
                }
              }

              if (nzcount < 2) {
                notdone = true;
                exitg1 = true;
              } else {
                ii--;
              }
            }

            if (!notdone) {
              exitg3 = 2;
            } else {
              if (i != ihi) {
                for (jcol = 0; jcol < 10; jcol++) {
                  jcolp1 = (i + 10 * jcol) - 1;
                  atmp = A[jcolp1];
                  jrow = (ihi + 10 * jcol) - 1;
                  A[jcolp1] = A[jrow];
                  A[jrow] = atmp;
                }
              }

              if (j != ihi) {
                for (jcol = 0; jcol < ihi; jcol++) {
                  jcolp1 = jcol + 10 * (j - 1);
                  atmp = A[jcolp1];
                  jrow = jcol + 10 * (ihi - 1);
                  A[jcolp1] = A[jrow];
                  A[jrow] = atmp;
                }
              }

              rscale[ihi - 1] = j;
              ihi--;
              if (ihi == 1) {
                rscale[0] = 1;
                exitg3 = 1;
              }
            }
          } while (exitg3 == 0);

          if (exitg3 != 1) {
            int exitg2;
            do {
              exitg2 = 0;
              i = 0;
              j = 0;
              notdone = false;
              jcol = ilo;
              exitg1 = false;
              while ((!exitg1) && (jcol <= ihi)) {
                nzcount = 0;
                i = ihi;
                j = jcol;
                ii = ilo;
                exitg4 = false;
                while ((!exitg4) && (ii <= ihi)) {
                  A_tmp = (ii + 10 * (jcol - 1)) - 1;
                  if ((A[A_tmp].re != 0.0) || (A[A_tmp].im != 0.0) || (ii ==
                       jcol)) {
                    if (nzcount == 0) {
                      i = ii;
                      nzcount = 1;
                      ii++;
                    } else {
                      nzcount = 2;
                      exitg4 = true;
                    }
                  } else {
                    ii++;
                  }
                }

                if (nzcount < 2) {
                  notdone = true;
                  exitg1 = true;
                } else {
                  jcol++;
                }
              }

              if (!notdone) {
                exitg2 = 1;
              } else {
                if (i != ilo) {
                  for (jcol = ilo; jcol < 11; jcol++) {
                    jcolp1 = 10 * (jcol - 1);
                    ii = (i + jcolp1) - 1;
                    atmp = A[ii];
                    jrow = (ilo + jcolp1) - 1;
                    A[ii] = A[jrow];
                    A[jrow] = atmp;
                  }
                }

                if (j != ilo) {
                  for (jcol = 0; jcol < ihi; jcol++) {
                    jcolp1 = jcol + 10 * (j - 1);
                    atmp = A[jcolp1];
                    jrow = jcol + 10 * (ilo - 1);
                    A[jcolp1] = A[jrow];
                    A[jrow] = atmp;
                  }
                }

                rscale[ilo - 1] = j;
                ilo++;
                if (ilo == ihi) {
                  rscale[ilo - 1] = ilo;
                  exitg2 = 1;
                }
              }
            } while (exitg2 == 0);
          }

          std::memset(&b_I[0], 0, 100U * sizeof(signed char));
          for (jcol = 0; jcol < 10; jcol++) {
            b_I[jcol + 10 * jcol] = 1;
          }

          for (jrow = 0; jrow < 100; jrow++) {
            V[jrow].re = b_I[jrow];
            V[jrow].im = 0.0;
          }

          if (ihi >= ilo + 2) {
            for (jcol = ilo - 1; jcol + 1 < ihi - 1; jcol++) {
              jcolp1 = jcol + 2;
              for (jrow = ihi - 1; jrow + 1 > jcol + 2; jrow--) {
                A_tmp = jrow + 10 * jcol;
                xzlartg(A[A_tmp - 1], A[A_tmp], &absxk, &atmp, &A[(jrow + 10 *
                         jcol) - 1]);
                A[A_tmp].re = 0.0;
                A[A_tmp].im = 0.0;
                for (j = jcolp1; j < 11; j++) {
                  nzcount = jrow + 10 * (j - 1);
                  ctoc = absxk * A[nzcount - 1].re + (atmp.re * A[nzcount].re -
                    atmp.im * A[nzcount].im);
                  stemp_im = absxk * A[nzcount - 1].im + (atmp.re * A[nzcount].
                    im + atmp.im * A[nzcount].re);
                  cto1 = A[nzcount - 1].im;
                  a = A[nzcount - 1].re;
                  A[nzcount].re = absxk * A[nzcount].re - (atmp.re * A[nzcount -
                    1].re + atmp.im * A[nzcount - 1].im);
                  A[nzcount].im = absxk * A[nzcount].im - (atmp.re * cto1 -
                    atmp.im * a);
                  A[nzcount - 1].re = ctoc;
                  A[nzcount - 1].im = stemp_im;
                }

                atmp.re = -atmp.re;
                atmp.im = -atmp.im;
                for (i = 1; i <= ihi; i++) {
                  nzcount = (i + 10 * (jrow - 1)) - 1;
                  ii = (i + 10 * jrow) - 1;
                  ctoc = absxk * A[ii].re + (atmp.re * A[nzcount].re - atmp.im *
                    A[nzcount].im);
                  stemp_im = absxk * A[ii].im + (atmp.re * A[nzcount].im +
                    atmp.im * A[nzcount].re);
                  cto1 = A[ii].im;
                  a = A[ii].re;
                  A[nzcount].re = absxk * A[nzcount].re - (atmp.re * A[ii].re +
                    atmp.im * A[ii].im);
                  A[nzcount].im = absxk * A[nzcount].im - (atmp.re * cto1 -
                    atmp.im * a);
                  A[ii].re = ctoc;
                  A[ii].im = stemp_im;
                }

                for (i = 0; i < 10; i++) {
                  nzcount = i + 10 * (jrow - 1);
                  ii = i + 10 * jrow;
                  ctoc = absxk * V[ii].re + (atmp.re * V[nzcount].re - atmp.im *
                    V[nzcount].im);
                  stemp_im = absxk * V[ii].im + (atmp.re * V[nzcount].im +
                    atmp.im * V[nzcount].re);
                  cto1 = V[ii].re;
                  V[nzcount].re = absxk * V[nzcount].re - (atmp.re * V[ii].re +
                    atmp.im * V[ii].im);
                  V[nzcount].im = absxk * V[nzcount].im - (atmp.re * V[ii].im -
                    atmp.im * cto1);
                  V[ii].re = ctoc;
                  V[ii].im = stemp_im;
                }
              }
            }
          }

          xzhgeqz(A, ilo, ihi, V, info, alpha1, beta1);
          if (*info == 0) {
            xztgevc(A, V);
            if (ilo > 1) {
              for (i = ilo - 2; i + 1 >= 1; i--) {
                jcol = rscale[i] - 1;
                if (rscale[i] != i + 1) {
                  for (j = 0; j < 10; j++) {
                    jcolp1 = i + 10 * j;
                    atmp = V[jcolp1];
                    nzcount = jcol + 10 * j;
                    V[jcolp1] = V[nzcount];
                    V[nzcount] = atmp;
                  }
                }
              }
            }

            if (ihi < 10) {
              jrow = ihi + 1;
              for (i = jrow; i < 11; i++) {
                ii = rscale[i - 1];
                if (ii != i) {
                  for (j = 0; j < 10; j++) {
                    jcolp1 = (i + 10 * j) - 1;
                    atmp = V[jcolp1];
                    nzcount = (ii + 10 * j) - 1;
                    V[jcolp1] = V[nzcount];
                    V[nzcount] = atmp;
                  }
                }
              }
            }

            for (nzcount = 0; nzcount < 10; nzcount++) {
              absxk = std::abs(V[10 * nzcount].re) + std::abs(V[10 * nzcount].im);
              for (jcol = 0; jcol < 9; jcol++) {
                ii = (jcol + 10 * nzcount) + 1;
                ctoc = std::abs(V[ii].re) + std::abs(V[ii].im);
                if (ctoc > absxk) {
                  absxk = ctoc;
                }
              }

              if (absxk >= 6.7178761075670888E-139) {
                absxk = 1.0 / absxk;
                for (jcol = 0; jcol < 10; jcol++) {
                  jrow = jcol + 10 * nzcount;
                  V[jrow].re *= absxk;
                  V[jrow].im *= absxk;
                }
              }
            }

            if (ilascl) {
              notdone = true;
              while (notdone) {
                stemp_im = anrmto * 2.0041683600089728E-292;
                cto1 = anrm / 4.9896007738368E+291;
                if ((stemp_im > anrm) && (anrm != 0.0)) {
                  a = 2.0041683600089728E-292;
                  anrmto = stemp_im;
                } else if (cto1 > anrmto) {
                  a = 4.9896007738368E+291;
                  anrm = cto1;
                } else {
                  a = anrm / anrmto;
                  notdone = false;
                }

                for (jrow = 0; jrow < 10; jrow++) {
                  alpha1[jrow].re *= a;
                  alpha1[jrow].im *= a;
                }
              }
            }
          }
        }
      }
    }
  }
}

//
// File trailer for xzggev.cpp
//
// [EOF]
//
