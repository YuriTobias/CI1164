#include <stdio.h>
#include <stdlib.h>

// #include "likwid.h"
#include "linear_ops.h"

#define N 10000

int main(int argc, char *argv[]) {
    // LIKWID_MARKER_INIT;

    // LIKWID_MARKER_START("Teste_LIKWID");
    // //
    // LIKWID_MARKER_STOP("Teste_LIKWID");

    // LIKWID_MARKER_CLOSE;

    double **A = NULL, **Acpy = NULL, *b = NULL, *bcpy = NULL, *x = NULL, *r = NULL;
    int n;

    initSystem(&A, &Acpy, &b, &bcpy, &x, &r, &n);

    gaussElimAlt(A, b, n);
    backSubstitution(A, b, x, n);
    calcResidue(Acpy, bcpy, x, r, n);
    printResults(x, r, n);

    copySystem(Acpy, &A, bcpy, &b, n);
    gaussElimPivot(A, b, n);
    backSubstitution(A, b, x, n);
    calcResidue(Acpy, bcpy, x, r, n);
    printResults(x, r, n);

    copySystem(Acpy, &A, bcpy, &b, n);
    gaussElimPivotNoMult(A, b, n);
    backSubstitution(A, b, x, n);
    calcResidue(Acpy, bcpy, x, r, n);
    printResults(x, r, n);

    freeMatrix(&A, n);
    freeMatrix(&Acpy, n);
    free(b);
    free(x);
    free(r);

    return 0;
}
/**
 * Resíduos
 * Script likwid
 */