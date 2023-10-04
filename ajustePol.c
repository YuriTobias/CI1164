#include <fenv.h>
#include <stdio.h>
#include <stdlib.h>

#include "linear_ops.h"
#include "polynomial_ops.h"

int main(int argc, char *argv[]) {
    fesetround(FE_DOWNWARD);
    Interval_t **points, **coeffs, **cpycoeffs, *terms, *cpyterms, *powers;
    int n, k;
    perfSquare(&points, &powers, &coeffs, &terms, &k, &n);
    copyMatrixInterval(coeffs, &cpycoeffs, n + 1);
    copyVectorInterval(terms, &cpyterms, n + 1);
    // for(int i = 0; i < n + 1; i++) {
    //     for (int j = 0; j < n + 1; j++) {
    //         printf("[%lf , %lf] ", cpycoeffs[i][j].min, cpycoeffs[i][j].max);
    //     }
    //     printf("[%lf , %lf]\n", cpyterms[i].min, cpyterms[i].max);
    // }
    gaussElimPivot(cpycoeffs, cpyterms, n + 1);
    // backSubstitution();
    // calcResidue();
    freeIntervalMatrix(&cpycoeffs, n + 1);
    freeIntervalMatrix(&points, k);
    freeIntervalMatrix(&coeffs, n + 1);
    free(cpyterms);
    free(powers);
    free(terms);
    return 0;
}