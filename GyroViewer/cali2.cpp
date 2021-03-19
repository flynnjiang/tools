#include <stdio.h>
#include <cstddef>
#include <cstdlib>

#include "matlab/rtwtypes.h"
#include "matlab/magmcali.h"
#include "matlab/magmcali_terminate.h"
#include "matlab/rt_nonfinite.h"
#include "matlab/coder_array.h"



int mag_cali(std::vector<double> &x,
             std::vector<double> &y,
             std::vector<double> &z,
             double *out,
             int out_size)
{
    unsigned int i = 0;
    coder::array<double, 1U> ax;
    coder::array<double, 1U> ay;
    coder::array<double, 1U> az;
    creal_T Winv[9];
    creal_T V[3];
    FILE *fp = NULL;
    struct timeval;
    static int file_id = 0;
    char path[128] = {0};

    if (NULL == out || out_size < 12) {
        return -1;
    }

    snprintf(path, sizeof(path), "magcali_%d.csv", file_id++);
    fp = fopen(path, "w");
    if (! fp) {
        return -1;
    }

    ax.set_size(x.size());
    ay.set_size(x.size());
    az.set_size(x.size());

    for (i = 0; i < x.size(); i++) {
        ax[i] = x[i];
        ay[i] = y[i];
        az[i] = z[i];
        fprintf(fp, "%.15lf,%.15lf,%.15lf\n", x[i], y[i], z[i]);
    }

    magmcali(ax, ay, az, Winv, V);

    for (i = 0; i < 9; i++) {
        out[i] = Winv[i].re;
        fprintf(fp, "%.15lf,", Winv[i].re);
    }

    for (i = 0; i < 3; i++) {
        out[9 + i] = V[i].re;
        fprintf(fp, "%.15lf,", V[i].re);
    }

    fclose(fp);

    magmcali_terminate();

    return 0;
}
