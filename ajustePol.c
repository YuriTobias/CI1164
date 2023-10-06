#include <fenv.h>
#include <stdio.h>
#include <stdlib.h>
#include "likwid.h"

#include "utils.h"
#include "linear_ops.h"
#include "polynomial_ops.h"

int main(int argc, char *argv[]) {
    fesetround(FE_DOWNWARD);
    Interval_t **points, **coeffs, **cpycoeffs, *terms, *cpyterms, *powers, *alphas, *residues;
    int n, k;
    double tgeraSL, tsolSL;

    tgeraSL = timestamp();
    perfSquare(&points, &powers, &coeffs, &terms, &k, &n);
    tgeraSL = timestamp() - tgeraSL;

    copyMatrixInterval(coeffs, &cpycoeffs, n + 1);
    copyVectorInterval(terms, &cpyterms, n + 1);
    tsolSL = timestamp();
    gaussElimPivot(cpycoeffs, cpyterms, n + 1);
    backSubstitution(cpycoeffs, cpyterms, &alphas, n + 1);
    tsolSL = timestamp() - tsolSL;
    
    calcResidue(coeffs, terms, alphas, &residues, n + 1);
    
    for(int i = 0; i < n + 1; i++) {
        printf("[%lf , %lf] ", alphas[i].min, alphas[i].max);
    }
    printf("\n");
    for(int i = 0; i < n + 1; i++) {
        printf("[%lf , %lf] ", residues[i].min, residues[i].max);
    }
    printf("\n");
    printf("%lf\n", tgeraSL);
    printf("%lf\n", tsolSL);

    freeIntervalMatrix(&cpycoeffs, n + 1);
    freeIntervalMatrix(&points, k);
    freeIntervalMatrix(&coeffs, n + 1);
    free(residues);
    free(alphas);
    free(cpyterms);
    free(powers);
    free(terms);
    return 0;
}