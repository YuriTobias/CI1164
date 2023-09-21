#include <stdio.h>
#include <stdlib.h>

#include "interpolation_ops.h"
// #include "likwid.h"
#include "utils.h"

#define N 10000

int main(int argc, char *argv[]) {
    LIKWID_MARKER_INIT;
    double **A, tLagrange, tNewton;
    int n;
    // Read the points from stdin
    initPoints(&A, &n);
    // Calculate interpolation through Lagrange method
    tLagrange = timestamp();
    LIKWID_MARKER_START("lagrange");
    printf("%lf\n", lagrangeInterpolation(atof(argv[1]), A, n));
    LIKWID_MARKER_STOP("lagrange");
    tLagrange = timestamp() - tLagrange;
    // Calculate interpolation through Newton method
    tNewton = timestamp();
    LIKWID_MARKER_START("newton");
    // printf("%lf\n", newtonInterpolation(atof(argv[1]), A, n));
    LIKWID_MARKER_STOP("newton");
    tNewton = timestamp() - tNewton;
    // Print the time taken by each method
    printf("%lf\n", tLagrange);
    printf("%lf\n", tNewton);
    // Free the memory allocated for the points
    freeMatrix(&A, n);
    LIKWID_MARKER_CLOSE;
}