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

    double **A = NULL, **Acpy = NULL, *b = NULL, *bcpy = NULL, *x = NULL;
    int n;

    initSystem(&A, &Acpy, &b, &bcpy, &x, &n);

    gaussElimAlt(A, b, n);
    backSubstitution(A, b, x, n);
    printResults(x, n);

    copySystem(Acpy, &A, bcpy, &b, n);
    gaussElimPivot(A, b, n);
    backSubstitution(A, b, x, n);
    printResults(x, n);

    copySystem(Acpy, &A, bcpy, &b, n);
    gaussElimPivotNoMult(A, b, n);
    backSubstitution(A, b, x, n);
    printResults(x, n);

    freeMatrix(&A, n);
    freeMatrix(&Acpy, n);
    free(b);
    free(x);

    return 0;
}
/**
 * Resíduos
 * Script likwid
 */