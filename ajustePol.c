#include <fenv.h>
#include <stdio.h>
#include <stdlib.h>

#include "linear_ops.h"
#include "polynomial_ops.h"

int main(int argc, char *argv[]) {
    fesetround(FE_DOWNWARD);
    Interval_t **points, **coeffs, *terms, *powers;
    int n, k;
    perfSquare(&points, &powers, &coeffs, &terms, &n, &k);
    // gaussElimPivot();
    // backSubstitution();
    // calcResidue();
    freeIntervalMatrix(&points, n);
    freeIntervalMatrix(&coeffs, k + 1);
    free(powers);
    free(terms);
    return 0;
}