//
// File: xzlarf.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 03-Mar-2021 16:17:11
//

// Include Files
#include "xzlarf.h"
#include "rt_nonfinite.h"
#include <cstring>

// Function Definitions
//
// Arguments    : int m
//                int n
//                int iv0
//                double tau
//                double C[100]
//                int ic0
//                double work[10]
// Return Type  : void
//
namespace coder
{
  namespace internal
  {
    namespace reflapack
    {
      void xzlarf(int m, int n, int iv0, double tau, double C[100], int ic0,
                  double work[10])
      {
        double c;
        int i;
        int ia;
        int iac;
        int lastc;
        int lastv;
        if (tau != 0.0) {
          boolean_T exitg2;
          lastv = m;
          i = iv0 + m;
          while ((lastv > 0) && (C[i - 2] == 0.0)) {
            lastv--;
            i--;
          }

          lastc = n - 1;
          exitg2 = false;
          while ((!exitg2) && (lastc + 1 > 0)) {
            int exitg1;
            i = ic0 + lastc * 10;
            ia = i;
            do {
              exitg1 = 0;
              if (ia <= (i + lastv) - 1) {
                if (C[ia - 1] != 0.0) {
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
          lastc = -1;
        }

        if (lastv > 0) {
          int b_i;
          int ix;
          int jy;
          if (lastc + 1 != 0) {
            if (0 <= lastc) {
              std::memset(&work[0], 0, (lastc + 1) * sizeof(double));
            }

            i = 0;
            b_i = ic0 + 10 * lastc;
            for (iac = ic0; iac <= b_i; iac += 10) {
              ix = iv0;
              c = 0.0;
              jy = (iac + lastv) - 1;
              for (ia = iac; ia <= jy; ia++) {
                c += C[ia - 1] * C[ix - 1];
                ix++;
              }

              work[i] += c;
              i++;
            }
          }

          if (!(-tau == 0.0)) {
            i = ic0;
            jy = 0;
            for (iac = 0; iac <= lastc; iac++) {
              if (work[jy] != 0.0) {
                c = work[jy] * -tau;
                ix = iv0;
                b_i = lastv + i;
                for (ia = i; ia < b_i; ia++) {
                  C[ia - 1] += C[ix - 1] * c;
                  ix++;
                }
              }

              jy++;
              i += 10;
            }
          }
        }
      }
    }
  }
}

//
// File trailer for xzlarf.cpp
//
// [EOF]
//
