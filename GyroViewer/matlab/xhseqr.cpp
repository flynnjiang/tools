//
// File: xhseqr.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 03-Mar-2021 16:17:11
//

// Include Files
#include "xhseqr.h"
#include "magmcali_data.h"
#include "magmcali_rtwutil.h"
#include "recip.h"
#include "rt_nonfinite.h"
#include "sqrt.h"
#include "xdlanv2.h"
#include "xdlapy3.h"
#include "xzlarfg.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : double h[100]
//                double z[100]
// Return Type  : int
//
namespace coder
{
  namespace internal
  {
    namespace lapack
    {
      int xhseqr(double h[100], double z[100])
      {
        double v[3];
        double aa;
        double ab;
        double ba;
        double bb;
        double rt1r;
        double rt2r;
        double s;
        double s_tmp;
        double tst;
        int b_i;
        int hoffset;
        int i;
        int info;
        int iy;
        int j;
        int nr;
        boolean_T exitg1;
        info = 0;
        v[0] = 0.0;
        v[1] = 0.0;
        v[2] = 0.0;
        for (j = 0; j < 7; j++) {
          i = j + 10 * j;
          h[i + 2] = 0.0;
          h[i + 3] = 0.0;
        }

        h[79] = 0.0;
        b_i = 9;
        exitg1 = false;
        while ((!exitg1) && (b_i + 1 >= 1)) {
          int L;
          int its;
          int k;
          int m;
          boolean_T exitg2;
          boolean_T goto150;
          L = 1;
          goto150 = false;
          its = 0;
          exitg2 = false;
          while ((!exitg2) && (its < 301)) {
            boolean_T exitg3;
            k = b_i;
            exitg3 = false;
            while ((!exitg3) && (k + 1 > L)) {
              i = k + 10 * (k - 1);
              ba = std::abs(h[i]);
              if (ba <= 1.0020841800044864E-291) {
                exitg3 = true;
              } else {
                hoffset = k + 10 * k;
                bb = std::abs(h[hoffset]);
                tst = std::abs(h[i - 1]) + bb;
                if (tst == 0.0) {
                  if (k - 1 >= 1) {
                    tst = std::abs(h[(k + 10 * (k - 2)) - 1]);
                  }

                  if (k + 2 <= 10) {
                    tst += std::abs(h[hoffset + 1]);
                  }
                }

                if (ba <= 2.2204460492503131E-16 * tst) {
                  tst = std::abs(h[hoffset - 1]);
                  if (ba > tst) {
                    ab = ba;
                    ba = tst;
                  } else {
                    ab = tst;
                  }

                  tst = std::abs(h[i - 1] - h[hoffset]);
                  if (bb > tst) {
                    aa = bb;
                    bb = tst;
                  } else {
                    aa = tst;
                  }

                  s = aa + ab;
                  tst = 2.2204460492503131E-16 * (bb * (aa / s));
                  if ((1.0020841800044864E-291 > tst) || rtIsNaN(tst)) {
                    tst = 1.0020841800044864E-291;
                  }

                  if (ba * (ab / s) <= tst) {
                    exitg3 = true;
                  } else {
                    k--;
                  }
                } else {
                  k--;
                }
              }
            }

            L = k + 1;
            if (k + 1 > 1) {
              h[k + 10 * (k - 1)] = 0.0;
            }

            if (k + 1 >= b_i) {
              goto150 = true;
              exitg2 = true;
            } else {
              if (its == 10) {
                hoffset = k + 10 * k;
                s = std::abs(h[hoffset + 1]) + std::abs(h[(k + 10 * (k + 1)) + 2]);
                tst = 0.75 * s + h[hoffset];
                ab = -0.4375 * s;
                aa = s;
                ba = tst;
              } else if (its == 20) {
                s = std::abs(h[b_i + 10 * (b_i - 1)]) + std::abs(h[(b_i + 10 *
                  (b_i - 2)) - 1]);
                tst = 0.75 * s + h[b_i + 10 * b_i];
                ab = -0.4375 * s;
                aa = s;
                ba = tst;
              } else {
                hoffset = b_i + 10 * (b_i - 1);
                tst = h[hoffset - 1];
                aa = h[hoffset];
                ab = h[(b_i + 10 * b_i) - 1];
                ba = h[b_i + 10 * b_i];
              }

              s = ((std::abs(tst) + std::abs(ab)) + std::abs(aa)) + std::abs(ba);
              if (s == 0.0) {
                rt1r = 0.0;
                bb = 0.0;
                rt2r = 0.0;
                ba = 0.0;
              } else {
                tst /= s;
                aa /= s;
                ab /= s;
                ba /= s;
                bb = (tst + ba) / 2.0;
                tst = (tst - bb) * (ba - bb) - ab * aa;
                aa = std::sqrt(std::abs(tst));
                if (tst >= 0.0) {
                  rt1r = bb * s;
                  rt2r = rt1r;
                  bb = aa * s;
                  ba = -bb;
                } else {
                  rt1r = bb + aa;
                  rt2r = bb - aa;
                  if (std::abs(rt1r - ba) <= std::abs(rt2r - ba)) {
                    rt1r *= s;
                    rt2r = rt1r;
                  } else {
                    rt2r *= s;
                    rt1r = rt2r;
                  }

                  bb = 0.0;
                  ba = 0.0;
                }
              }

              m = b_i - 1;
              exitg3 = false;
              while ((!exitg3) && (m >= k + 1)) {
                hoffset = m + 10 * (m - 1);
                tst = h[hoffset];
                s_tmp = h[hoffset - 1];
                aa = s_tmp - rt2r;
                s = (std::abs(aa) + std::abs(ba)) + std::abs(tst);
                ab = tst / s;
                hoffset = m + 10 * m;
                v[0] = (ab * h[hoffset - 1] + (s_tmp - rt1r) * (aa / s)) - bb *
                  (ba / s);
                tst = h[hoffset];
                v[1] = ab * (((s_tmp + tst) - rt1r) - rt2r);
                v[2] = ab * h[hoffset + 1];
                s = (std::abs(v[0]) + std::abs(v[1])) + std::abs(v[2]);
                v[0] /= s;
                v[1] /= s;
                v[2] /= s;
                if (m == k + 1) {
                  exitg3 = true;
                } else {
                  i = m + 10 * (m - 2);
                  if (std::abs(h[i - 1]) * (std::abs(v[1]) + std::abs(v[2])) <=
                      2.2204460492503131E-16 * std::abs(v[0]) * ((std::abs(h[i -
                         2]) + std::abs(s_tmp)) + std::abs(tst))) {
                    exitg3 = true;
                  } else {
                    m--;
                  }
                }
              }

              for (int b_k = m; b_k <= b_i; b_k++) {
                nr = (b_i - b_k) + 2;
                if (3 < nr) {
                  nr = 3;
                }

                if (b_k > m) {
                  hoffset = (b_k + 10 * (b_k - 2)) - 1;
                  for (j = 0; j < nr; j++) {
                    v[j] = h[j + hoffset];
                  }
                }

                tst = v[0];
                bb = reflapack::xzlarfg(nr, &tst, v);
                v[0] = tst;
                if (b_k > m) {
                  h[(b_k + 10 * (b_k - 2)) - 1] = tst;
                  i = b_k + 10 * (b_k - 2);
                  h[i] = 0.0;
                  if (b_k < b_i) {
                    h[i + 1] = 0.0;
                  }
                } else {
                  if (m > k + 1) {
                    h[(b_k + 10 * (b_k - 2)) - 1] *= 1.0 - bb;
                  }
                }

                s = v[1];
                aa = bb * v[1];
                if (nr == 3) {
                  rt1r = v[2];
                  tst = bb * v[2];
                  for (j = b_k; j < 11; j++) {
                    iy = b_k + 10 * (j - 1);
                    ab = (h[iy - 1] + s * h[iy]) + rt1r * h[iy + 1];
                    h[iy - 1] -= ab * bb;
                    h[iy] -= ab * aa;
                    h[iy + 1] -= ab * tst;
                  }

                  if (b_k + 3 < b_i + 1) {
                    i = b_k + 2;
                  } else {
                    i = b_i;
                  }

                  for (j = 0; j <= i; j++) {
                    iy = j + 10 * (b_k - 1);
                    hoffset = j + 10 * b_k;
                    nr = j + 10 * (b_k + 1);
                    ab = (h[iy] + s * h[hoffset]) + rt1r * h[nr];
                    h[iy] -= ab * bb;
                    h[hoffset] -= ab * aa;
                    h[nr] -= ab * tst;
                  }

                  for (j = 0; j < 10; j++) {
                    iy = j + 10 * (b_k - 1);
                    hoffset = j + 10 * b_k;
                    nr = j + 10 * (b_k + 1);
                    ab = (z[iy] + s * z[hoffset]) + rt1r * z[nr];
                    z[iy] -= ab * bb;
                    z[hoffset] -= ab * aa;
                    z[nr] -= ab * tst;
                  }
                } else {
                  if (nr == 2) {
                    for (j = b_k; j < 11; j++) {
                      iy = b_k + 10 * (j - 1);
                      tst = h[iy - 1];
                      ab = tst + s * h[iy];
                      h[iy - 1] = tst - ab * bb;
                      h[iy] -= ab * aa;
                    }

                    for (j = 0; j <= b_i; j++) {
                      iy = j + 10 * (b_k - 1);
                      hoffset = j + 10 * b_k;
                      ab = h[iy] + s * h[hoffset];
                      h[iy] -= ab * bb;
                      h[hoffset] -= ab * aa;
                    }

                    for (j = 0; j < 10; j++) {
                      iy = j + 10 * (b_k - 1);
                      tst = z[iy];
                      hoffset = j + 10 * b_k;
                      ab = tst + s * z[hoffset];
                      z[iy] = tst - ab * bb;
                      z[hoffset] -= ab * aa;
                    }
                  }
                }
              }

              its++;
            }
          }

          if (!goto150) {
            info = b_i + 1;
            exitg1 = true;
          } else {
            if ((L != b_i + 1) && (L == b_i)) {
              i = b_i + 10 * b_i;
              s = h[i - 1];
              m = 10 * (b_i - 1);
              hoffset = b_i + m;
              rt1r = h[hoffset];
              tst = h[i];
              reflapack::xdlanv2(&h[(b_i + 10 * (b_i - 1)) - 1], &s, &rt1r, &tst,
                                 &aa, &ab, &bb, &ba, &s_tmp, &rt2r);
              h[i - 1] = s;
              h[hoffset] = rt1r;
              h[i] = tst;
              if (10 > b_i + 1) {
                hoffset = 8 - b_i;
                iy = b_i + (b_i + 1) * 10;
                nr = iy - 1;
                for (k = 0; k <= hoffset; k++) {
                  tst = s_tmp * h[nr] + rt2r * h[iy];
                  h[iy] = s_tmp * h[iy] - rt2r * h[nr];
                  h[nr] = tst;
                  iy += 10;
                  nr += 10;
                }
              }

              hoffset = b_i * 10;
              if (b_i - 1 >= 1) {
                nr = m;
                iy = hoffset;
                for (k = 0; k <= b_i - 2; k++) {
                  tst = s_tmp * h[nr] + rt2r * h[iy];
                  h[iy] = s_tmp * h[iy] - rt2r * h[nr];
                  h[nr] = tst;
                  iy++;
                  nr++;
                }
              }

              for (k = 0; k < 10; k++) {
                tst = s_tmp * z[m] + rt2r * z[hoffset];
                z[hoffset] = s_tmp * z[hoffset] - rt2r * z[m];
                z[m] = tst;
                hoffset++;
                m++;
              }
            }

            b_i = L - 2;
          }
        }

        hoffset = 4;
        for (j = 0; j < 7; j++) {
          if (hoffset <= 10) {
            std::memset(&h[(j * 10 + hoffset) + -1], 0, (11 - hoffset) * sizeof
                        (double));
          }

          hoffset++;
        }

        return info;
      }

      //
      // Arguments    : creal_T h[9]
      //                creal_T z[9]
      // Return Type  : int
      //
      int xhseqr(creal_T h[9], creal_T z[9])
      {
        creal_T b_v[2];
        creal_T sc;
        creal_T v;
        double ba;
        double br;
        double htmp2;
        double im;
        double re;
        double tst;
        int i;
        int info;
        int k;
        boolean_T exitg1;
        info = 0;
        h[2].re = 0.0;
        h[2].im = 0.0;
        if (h[1].im != 0.0) {
          tst = h[1].re;
          htmp2 = h[1].im;
          br = std::abs(h[1].re) + std::abs(h[1].im);
          if (htmp2 == 0.0) {
            sc.re = tst / br;
            sc.im = 0.0;
          } else if (tst == 0.0) {
            sc.re = 0.0;
            sc.im = htmp2 / br;
          } else {
            sc.re = tst / br;
            sc.im = htmp2 / br;
          }

          br = rt_hypotd_snf(sc.re, sc.im);
          if (-sc.im == 0.0) {
            re = sc.re / br;
            im = 0.0;
          } else if (sc.re == 0.0) {
            re = 0.0;
            im = -sc.im / br;
          } else {
            re = sc.re / br;
            im = -sc.im / br;
          }

          h[1].re = rt_hypotd_snf(h[1].re, h[1].im);
          h[1].im = 0.0;
          for (k = 5; k <= 8; k += 3) {
            htmp2 = re * h[k - 1].im + im * h[k - 1].re;
            h[k - 1].re = re * h[k - 1].re - im * h[k - 1].im;
            h[k - 1].im = htmp2;
          }

          for (k = 4; k < 7; k++) {
            htmp2 = re * h[k - 1].im + -im * h[k - 1].re;
            h[k - 1].re = re * h[k - 1].re - -im * h[k - 1].im;
            h[k - 1].im = htmp2;
            htmp2 = re * z[k - 1].im + -im * z[k - 1].re;
            z[k - 1].re = re * z[k - 1].re - -im * z[k - 1].im;
            z[k - 1].im = htmp2;
          }
        }

        if (h[5].im != 0.0) {
          tst = h[5].re;
          htmp2 = h[5].im;
          br = std::abs(h[5].re) + std::abs(h[5].im);
          if (htmp2 == 0.0) {
            sc.re = tst / br;
            sc.im = 0.0;
          } else if (tst == 0.0) {
            sc.re = 0.0;
            sc.im = htmp2 / br;
          } else {
            sc.re = tst / br;
            sc.im = htmp2 / br;
          }

          br = rt_hypotd_snf(sc.re, sc.im);
          if (-sc.im == 0.0) {
            re = sc.re / br;
            im = 0.0;
          } else if (sc.re == 0.0) {
            re = 0.0;
            im = -sc.im / br;
          } else {
            re = sc.re / br;
            im = -sc.im / br;
          }

          h[5].re = rt_hypotd_snf(h[5].re, h[5].im);
          h[5].im = 0.0;
          for (k = 9; k <= 9; k += 3) {
            htmp2 = re * h[8].im + im * h[8].re;
            h[8].re = re * h[8].re - im * h[8].im;
            h[8].im = htmp2;
          }

          for (k = 7; k < 10; k++) {
            htmp2 = re * h[k - 1].im + -im * h[k - 1].re;
            h[k - 1].re = re * h[k - 1].re - -im * h[k - 1].im;
            h[k - 1].im = htmp2;
            htmp2 = re * z[k - 1].im + -im * z[k - 1].re;
            z[k - 1].re = re * z[k - 1].re - -im * z[k - 1].im;
            z[k - 1].im = htmp2;
          }
        }

        i = 2;
        exitg1 = false;
        while ((!exitg1) && (i + 1 >= 1)) {
          int L;
          int its;
          boolean_T exitg2;
          boolean_T goto140;
          L = -1;
          goto140 = false;
          its = 0;
          exitg2 = false;
          while ((!exitg2) && (its < 301)) {
            double ab;
            double bb;
            double s;
            int knt;
            int t_re_tmp;
            boolean_T exitg3;
            k = i;
            exitg3 = false;
            while ((!exitg3) && (k + 1 > L + 2)) {
              t_re_tmp = k + 3 * (k - 1);
              htmp2 = std::abs(h[t_re_tmp].re);
              ba = htmp2 + std::abs(h[t_re_tmp].im);
              if (ba <= 3.0062525400134592E-292) {
                exitg3 = true;
              } else {
                knt = k + 3 * k;
                bb = std::abs(h[knt].re) + std::abs(h[knt].im);
                tst = (std::abs(h[t_re_tmp - 1].re) + std::abs(h[t_re_tmp - 1].
                        im)) + bb;
                if (tst == 0.0) {
                  if (k - 1 >= 1) {
                    tst = std::abs(h[(k + 3 * (k - 2)) - 1].re);
                  }

                  if (k + 2 <= 3) {
                    tst += std::abs(h[knt + 1].re);
                  }
                }

                if (htmp2 <= 2.2204460492503131E-16 * tst) {
                  htmp2 = std::abs(h[knt - 1].re) + std::abs(h[knt - 1].im);
                  if (ba > htmp2) {
                    ab = ba;
                    ba = htmp2;
                  } else {
                    ab = htmp2;
                  }

                  htmp2 = std::abs(h[t_re_tmp - 1].re - h[knt].re) + std::abs
                    (h[t_re_tmp - 1].im - h[knt].im);
                  if (bb > htmp2) {
                    tst = bb;
                    bb = htmp2;
                  } else {
                    tst = htmp2;
                  }

                  s = tst + ab;
                  htmp2 = 2.2204460492503131E-16 * (bb * (tst / s));
                  if ((3.0062525400134592E-292 > htmp2) || rtIsNaN(htmp2)) {
                    htmp2 = 3.0062525400134592E-292;
                  }

                  if (ba * (ab / s) <= htmp2) {
                    exitg3 = true;
                  } else {
                    k--;
                  }
                } else {
                  k--;
                }
              }
            }

            L = k - 1;
            if (k + 1 > 1) {
              t_re_tmp = k + 3 * (k - 1);
              h[t_re_tmp].re = 0.0;
              h[t_re_tmp].im = 0.0;
            }

            if (k + 1 >= i + 1) {
              goto140 = true;
              exitg2 = true;
            } else {
              double t_re;
              int c_k;
              int m;
              boolean_T goto70;
              if (its == 10) {
                t_re_tmp = k + 3 * k;
                t_re = 0.75 * std::abs(h[(k + 3 * k) + 1].re) + h[t_re_tmp].re;
                ab = h[t_re_tmp].im;
              } else if (its == 20) {
                t_re_tmp = i + 3 * i;
                t_re = 0.75 * std::abs(h[i + 3 * (i - 1)].re) + h[t_re_tmp].re;
                ab = h[t_re_tmp].im;
              } else {
                double u_im;
                double u_re;
                t_re_tmp = i + 3 * i;
                t_re = h[t_re_tmp].re;
                ab = h[t_re_tmp].im;
                v = h[t_re_tmp - 1];
                b_sqrt(&v);
                knt = i + 3 * (i - 1);
                sc = h[knt];
                b_sqrt(&sc);
                u_re = v.re * sc.re - v.im * sc.im;
                u_im = v.re * sc.im + v.im * sc.re;
                s = std::abs(u_re) + std::abs(u_im);
                if (s != 0.0) {
                  ba = 0.5 * (h[knt - 1].re - h[t_re_tmp].re);
                  im = 0.5 * (h[knt - 1].im - h[t_re_tmp].im);
                  bb = std::abs(ba) + std::abs(im);
                  if ((!(s > bb)) && (!rtIsNaN(bb))) {
                    s = bb;
                  }

                  if (im == 0.0) {
                    t_re = ba / s;
                    ab = 0.0;
                  } else if (ba == 0.0) {
                    t_re = 0.0;
                    ab = im / s;
                  } else {
                    t_re = ba / s;
                    ab = im / s;
                  }

                  re = t_re * t_re - ab * ab;
                  tst = t_re * ab;
                  if (u_im == 0.0) {
                    sc.re = u_re / s;
                    sc.im = 0.0;
                  } else if (u_re == 0.0) {
                    sc.re = 0.0;
                    sc.im = u_im / s;
                  } else {
                    sc.re = u_re / s;
                    sc.im = u_im / s;
                  }

                  htmp2 = sc.re * sc.re - sc.im * sc.im;
                  ab = sc.re * sc.im;
                  v.re = re + htmp2;
                  v.im = (tst + tst) + (ab + ab);
                  b_sqrt(&v);
                  sc.re = s * v.re;
                  sc.im = s * v.im;
                  if (bb > 0.0) {
                    if (im == 0.0) {
                      t_re = ba / bb;
                      ab = 0.0;
                    } else if (ba == 0.0) {
                      t_re = 0.0;
                      ab = im / bb;
                    } else {
                      t_re = ba / bb;
                      ab = im / bb;
                    }

                    if (t_re * sc.re + ab * sc.im < 0.0) {
                      sc.re = -sc.re;
                      sc.im = -sc.im;
                    }
                  }

                  br = ba + sc.re;
                  ab = im + sc.im;
                  if (ab == 0.0) {
                    if (u_im == 0.0) {
                      ba = u_re / br;
                      tst = 0.0;
                    } else if (u_re == 0.0) {
                      ba = 0.0;
                      tst = u_im / br;
                    } else {
                      ba = u_re / br;
                      tst = u_im / br;
                    }
                  } else if (br == 0.0) {
                    if (u_re == 0.0) {
                      ba = u_im / ab;
                      tst = 0.0;
                    } else if (u_im == 0.0) {
                      ba = 0.0;
                      tst = -(u_re / ab);
                    } else {
                      ba = u_im / ab;
                      tst = -(u_re / ab);
                    }
                  } else {
                    bb = std::abs(br);
                    tst = std::abs(ab);
                    if (bb > tst) {
                      s = ab / br;
                      tst = br + s * ab;
                      ba = (u_re + s * u_im) / tst;
                      tst = (u_im - s * u_re) / tst;
                    } else if (tst == bb) {
                      if (br > 0.0) {
                        htmp2 = 0.5;
                      } else {
                        htmp2 = -0.5;
                      }

                      if (ab > 0.0) {
                        tst = 0.5;
                      } else {
                        tst = -0.5;
                      }

                      ba = (u_re * htmp2 + u_im * tst) / bb;
                      tst = (u_im * htmp2 - u_re * tst) / bb;
                    } else {
                      s = br / ab;
                      tst = ab + s * br;
                      ba = (s * u_re + u_im) / tst;
                      tst = (s * u_im - u_re) / tst;
                    }
                  }

                  t_re = h[t_re_tmp].re - (u_re * ba - u_im * tst);
                  ab = h[t_re_tmp].im - (u_re * tst + u_im * ba);
                }
              }

              goto70 = false;
              m = i;
              exitg3 = false;
              while ((!exitg3) && (m > k + 1)) {
                sc.re = h[4].re - t_re;
                sc.im = h[4].im - ab;
                tst = h[5].re;
                s = (std::abs(sc.re) + std::abs(sc.im)) + std::abs(tst);
                if (sc.im == 0.0) {
                  re = sc.re / s;
                  im = 0.0;
                } else if (sc.re == 0.0) {
                  re = 0.0;
                  im = sc.im / s;
                } else {
                  re = sc.re / s;
                  im = sc.im / s;
                }

                sc.re = re;
                sc.im = im;
                tst /= s;
                b_v[0] = sc;
                b_v[1].re = tst;
                b_v[1].im = 0.0;
                if (std::abs(h[1].re) * std::abs(tst) <= 2.2204460492503131E-16 *
                    ((std::abs(re) + std::abs(im)) * ((std::abs(h[4].re) + std::
                       abs(h[4].im)) + (std::abs(h[8].re) + std::abs(h[8].im)))))
                {
                  goto70 = true;
                  exitg3 = true;
                } else {
                  m = 1;
                }
              }

              if (!goto70) {
                knt = k + 3 * k;
                sc.re = h[knt].re - t_re;
                sc.im = h[knt].im - ab;
                tst = h[(k + 3 * k) + 1].re;
                s = (std::abs(sc.re) + std::abs(sc.im)) + std::abs(tst);
                if (sc.im == 0.0) {
                  b_v[0].re = sc.re / s;
                  b_v[0].im = 0.0;
                } else if (sc.re == 0.0) {
                  b_v[0].re = 0.0;
                  b_v[0].im = sc.im / s;
                } else {
                  b_v[0].re = sc.re / s;
                  b_v[0].im = sc.im / s;
                }

                tst /= s;
                b_v[1].re = tst;
                b_v[1].im = 0.0;
              }

              for (int b_k = m; b_k <= i; b_k++) {
                int j;
                if (b_k > m) {
                  b_v[0] = h[1];
                  b_v[1] = h[2];
                }

                sc = b_v[0];
                ba = b_v[1].re;
                im = b_v[1].im;
                t_re = 0.0;
                ab = 0.0;
                tst = rt_hypotd_snf(b_v[1].re, b_v[1].im);
                if ((tst != 0.0) || (b_v[0].im != 0.0)) {
                  htmp2 = xdlapy3(b_v[0].re, b_v[0].im, tst);
                  if (b_v[0].re >= 0.0) {
                    htmp2 = -htmp2;
                  }

                  if (std::abs(htmp2) < 1.0020841800044864E-292) {
                    knt = -1;
                    do {
                      knt++;
                      ba *= 9.9792015476736E+291;
                      im *= 9.9792015476736E+291;
                      htmp2 *= 9.9792015476736E+291;
                      sc.re *= 9.9792015476736E+291;
                      sc.im *= 9.9792015476736E+291;
                    } while (!(std::abs(htmp2) >= 1.0020841800044864E-292));

                    htmp2 = xdlapy3(sc.re, sc.im, rt_hypotd_snf(ba, im));
                    if (sc.re >= 0.0) {
                      htmp2 = -htmp2;
                    }

                    tst = htmp2 - sc.re;
                    if (0.0 - sc.im == 0.0) {
                      t_re = tst / htmp2;
                      ab = 0.0;
                    } else if (tst == 0.0) {
                      t_re = 0.0;
                      ab = (0.0 - sc.im) / htmp2;
                    } else {
                      t_re = tst / htmp2;
                      ab = (0.0 - sc.im) / htmp2;
                    }

                    v.re = sc.re - htmp2;
                    v.im = sc.im;
                    sc = recip(v);
                    re = sc.re * ba - sc.im * im;
                    im = sc.re * im + sc.im * ba;
                    ba = re;
                    for (c_k = 0; c_k <= knt; c_k++) {
                      htmp2 *= 1.0020841800044864E-292;
                    }

                    sc.re = htmp2;
                    sc.im = 0.0;
                  } else {
                    tst = htmp2 - b_v[0].re;
                    if (0.0 - b_v[0].im == 0.0) {
                      t_re = tst / htmp2;
                      ab = 0.0;
                    } else if (tst == 0.0) {
                      t_re = 0.0;
                      ab = (0.0 - b_v[0].im) / htmp2;
                    } else {
                      t_re = tst / htmp2;
                      ab = (0.0 - b_v[0].im) / htmp2;
                    }

                    v.re = b_v[0].re - htmp2;
                    v.im = b_v[0].im;
                    v = recip(v);
                    ba = v.re * b_v[1].re - v.im * b_v[1].im;
                    im = v.re * b_v[1].im + v.im * b_v[1].re;
                    sc.re = htmp2;
                    sc.im = 0.0;
                  }
                }

                b_v[0] = sc;
                b_v[1].re = ba;
                b_v[1].im = im;
                if (b_k > m) {
                  h[1] = sc;
                  h[2].re = 0.0;
                  h[2].im = 0.0;
                }

                tst = t_re * ba - ab * im;
                for (j = b_k; j < 4; j++) {
                  c_k = b_k + 3 * (j - 1);
                  sc.re = (t_re * h[c_k - 1].re - -ab * h[c_k - 1].im) + tst *
                    h[c_k].re;
                  sc.im = (t_re * h[c_k - 1].im + -ab * h[c_k - 1].re) + tst *
                    h[c_k].im;
                  h[c_k - 1].re -= sc.re;
                  h[c_k - 1].im -= sc.im;
                  h[c_k].re -= sc.re * ba - sc.im * im;
                  h[c_k].im -= sc.re * im + sc.im * ba;
                }

                for (j = 0; j <= i; j++) {
                  t_re_tmp = j + 3 * (b_k - 1);
                  knt = j + 3 * b_k;
                  sc.re = (t_re * h[t_re_tmp].re - ab * h[t_re_tmp].im) + tst *
                    h[knt].re;
                  sc.im = (t_re * h[t_re_tmp].im + ab * h[t_re_tmp].re) + tst *
                    h[knt].im;
                  h[t_re_tmp].re -= sc.re;
                  h[t_re_tmp].im -= sc.im;
                  h[knt].re -= sc.re * ba - sc.im * -im;
                  h[knt].im -= sc.re * -im + sc.im * ba;
                }

                t_re_tmp = 3 * (b_k - 1);
                sc.re = (t_re * z[t_re_tmp].re - ab * z[t_re_tmp].im) + tst * z
                  [3 * b_k].re;
                sc.im = (t_re * z[t_re_tmp].im + ab * z[t_re_tmp].re) + tst * z
                  [3 * b_k].im;
                z[t_re_tmp].re -= sc.re;
                z[t_re_tmp].im -= sc.im;
                z[3 * b_k].re -= sc.re * ba - sc.im * -im;
                z[3 * b_k].im -= sc.re * -im + sc.im * ba;
                knt = 3 * b_k + 1;
                sc.re = (t_re * z[t_re_tmp + 1].re - ab * z[t_re_tmp + 1].im) +
                  tst * z[knt].re;
                sc.im = (t_re * z[t_re_tmp + 1].im + ab * z[t_re_tmp + 1].re) +
                  tst * z[knt].im;
                z[t_re_tmp + 1].re -= sc.re;
                z[t_re_tmp + 1].im -= sc.im;
                z[knt].re -= sc.re * ba - sc.im * -im;
                z[knt].im -= sc.re * -im + sc.im * ba;
                knt = 3 * b_k + 2;
                sc.re = (t_re * z[t_re_tmp + 2].re - ab * z[t_re_tmp + 2].im) +
                  tst * z[knt].re;
                sc.im = (t_re * z[t_re_tmp + 2].im + ab * z[t_re_tmp + 2].re) +
                  tst * z[knt].im;
                z[t_re_tmp + 2].re -= sc.re;
                z[t_re_tmp + 2].im -= sc.im;
                z[knt].re -= sc.re * ba - sc.im * -im;
                z[knt].im -= sc.re * -im + sc.im * ba;
                if ((b_k == m) && (m > k + 1)) {
                  br = rt_hypotd_snf(1.0 - t_re, 0.0 - ab);
                  if (0.0 - ab == 0.0) {
                    re = (1.0 - t_re) / br;
                    im = 0.0;
                  } else if (1.0 - t_re == 0.0) {
                    re = 0.0;
                    im = (0.0 - ab) / br;
                  } else {
                    re = (1.0 - t_re) / br;
                    im = (0.0 - ab) / br;
                  }

                  htmp2 = h[5].re * -im + h[5].im * re;
                  h[5].re = h[5].re * re - h[5].im * -im;
                  h[5].im = htmp2;
                  for (j = 2; j <= i + 1; j++) {
                    if (j != 3) {
                      htmp2 = re * h[7].im + im * h[7].re;
                      h[7].re = re * h[7].re - im * h[7].im;
                      h[7].im = htmp2;
                      htmp2 = re * h[3].im + -im * h[3].re;
                      h[3].re = re * h[3].re - -im * h[3].im;
                      h[3].im = htmp2;
                      htmp2 = re * z[3].im + -im * z[3].re;
                      z[3].re = re * z[3].re - -im * z[3].im;
                      z[3].im = htmp2;
                      htmp2 = re * z[4].im + -im * z[4].re;
                      z[4].re = re * z[4].re - -im * z[4].im;
                      z[4].im = htmp2;
                      htmp2 = re * z[5].im + -im * z[5].re;
                      z[5].re = re * z[5].re - -im * z[5].im;
                      z[5].im = htmp2;
                    }
                  }
                }
              }

              knt = i + 3 * (i - 1);
              sc = h[knt];
              if (h[knt].im != 0.0) {
                htmp2 = rt_hypotd_snf(h[knt].re, h[knt].im);
                h[knt].re = htmp2;
                h[knt].im = 0.0;
                if (sc.im == 0.0) {
                  re = sc.re / htmp2;
                  im = 0.0;
                } else if (sc.re == 0.0) {
                  re = 0.0;
                  im = sc.im / htmp2;
                } else {
                  re = sc.re / htmp2;
                  im = sc.im / htmp2;
                }

                if (3 > i + 1) {
                  htmp2 = re * h[7].im + -im * h[7].re;
                  h[7].re = re * h[7].re - -im * h[7].im;
                  h[7].im = htmp2;
                }

                knt = i * 3;
                c_k = i * 3 + 1;
                t_re_tmp = knt + i;
                for (k = c_k; k <= t_re_tmp; k++) {
                  htmp2 = re * h[k - 1].im + im * h[k - 1].re;
                  h[k - 1].re = re * h[k - 1].re - im * h[k - 1].im;
                  h[k - 1].im = htmp2;
                }

                t_re_tmp = knt + 3;
                for (k = c_k; k <= t_re_tmp; k++) {
                  htmp2 = re * z[k - 1].im + im * z[k - 1].re;
                  z[k - 1].re = re * z[k - 1].re - im * z[k - 1].im;
                  z[k - 1].im = htmp2;
                }
              }

              its++;
            }
          }

          if (!goto140) {
            info = i + 1;
            exitg1 = true;
          } else {
            i = L;
          }
        }

        return info;
      }
    }
  }
}

//
// File trailer for xhseqr.cpp
//
// [EOF]
//
