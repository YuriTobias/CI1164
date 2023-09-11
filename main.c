#include <stdio.h>
#include <stdlib.h>

#include "likwid.h"
#include "linear_ops.h"
#include "utils.h"

#define N 10000

int main(int argc, char *argv[]) {
    LIKWID_MARKER_INIT;
    double **A = NULL, **Acpy = NULL, *b = NULL, *bcpy = NULL, *x = NULL, *r = NULL, t;
    int n;

    initSystem(&A, &Acpy, &b, &bcpy, &x, &r, &n);

    LIKWID_MARKER_START("Elim_pivot");
    t = timestamp();
    gaussElimPivot(A, b, n);
    backSubstitution(A, b, x, n);
    t = timestamp() - t;
    calcResidue(Acpy, bcpy, x, r, n);
    printResults(x, r, t, n);
    LIKWID_MARKER_STOP("Elim_pivot");

    LIKWID_MARKER_START("Elim_pivot_no_mult");
    copySystem(Acpy, &A, bcpy, &b, n);
    t = timestamp();
    gaussElimPivotNoMult(A, b, n);
    backSubstitution(A, b, x, n);
    t = timestamp() - t;
    calcResidue(Acpy, bcpy, x, r, n);
    printResults(x, r, t, n);
    LIKWID_MARKER_STOP("Elim_pivot_no_mult");

    LIKWID_MARKER_START("Elim_alt");
    copySystem(Acpy, &A, bcpy, &b, n);
    t = timestamp();
    gaussElimAlt(A, b, n);
    backSubstitution(A, b, x, n);
    t = timestamp() - t;
    calcResidue(Acpy, bcpy, x, r, n);
    printResults(x, r, t, n);
    LIKWID_MARKER_STOP("Elim_alt");
    
    LIKWID_MARKER_CLOSE;

    freeMatrix(&A, n);
    freeMatrix(&Acpy, n);
    free(b);
    free(x);
    free(r);

    return 0;
}