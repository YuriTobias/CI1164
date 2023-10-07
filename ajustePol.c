#include <fenv.h>
#include <stdio.h>
#include <stdlib.h>

#include "likwid.h"
#include "linear_ops.h"
#include "polynomial_ops.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    LIKWID_MARKER_INIT;
    fesetround(FE_DOWNWARD);

    Interval_t **points, **coeffs, **cpycoeffs, *terms, *cpyterms, *powers, *alphas, *residues;
    int n, k;
    double tgeraSL, tsolSL;
    initInputs(&points, &powers, &coeffs, &terms, &k, &n);
    LIKWID_MARKER_START("tgeraSL");
    tgeraSL = timestamp();
    perfSquare(points, powers, coeffs, terms, k, n);
    tgeraSL = timestamp() - tgeraSL;
    LIKWID_MARKER_STOP("tgeraSL");

    copyMatrixInterval(coeffs, &cpycoeffs, n + 1);
    copyVectorInterval(terms, &cpyterms, n + 1);
    LIKWID_MARKER_START("tsolSL");
    tsolSL = timestamp();
    gaussElimPivot(cpycoeffs, cpyterms, n + 1);
    backSubstitution(cpycoeffs, cpyterms, &alphas, n + 1);
    tsolSL = timestamp() - tsolSL;
    LIKWID_MARKER_STOP("tsolSL");

    calcResidue(coeffs, terms, alphas, &residues, n + 1);

    for (int i = 0; i < n + 1; i++) {
        printf("[%e , %e] ", alphas[i].min, alphas[i].max);
    }
    printf("\n");
    for (int i = 0; i < n + 1; i++) {
        printf("[%e , %e] ", residues[i].min, residues[i].max);
    }
    printf("\n");
    printf("%e\n", tgeraSL);
    printf("%e\n", tsolSL);

    freeIntervalMatrix(&cpycoeffs, n + 1);
    freeIntervalMatrix(&points, k);
    freeIntervalMatrix(&coeffs, n + 1);
    free(residues);
    free(alphas);
    free(cpyterms);
    free(powers);
    free(terms);

    LIKWID_MARKER_CLOSE;
    return 0;
}