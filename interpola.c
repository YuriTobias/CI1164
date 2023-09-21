#include <stdio.h>
#include <stdlib.h>

#include "interpolation_ops.h"
// #include "likwid.h"
// #include "utils.h"

#define N 10000

int main(int argc, char *argv[]) {
    double **A;
    int n;
    initPoints(&A, &n);
    printf("%lf\n", lagrangeInterpolation(atof(argv[1]), A, n));
    freeMatrix(&A, n);
}