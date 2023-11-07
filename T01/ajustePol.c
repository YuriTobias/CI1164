#include <fenv.h>
#include <stdio.h>
#include <stdlib.h>

#include "interval_ops.h"
#include "likwid.h"
#include "system_ops.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    LIKWID_MARKER_INIT;
    fesetround(FE_DOWNWARD);

    Interval_t **points, **coeffs, *terms, *powers, *solution, *residuals;
    int degree, npoints;
    double systemSolutionTs;

    initData(&points, &powers, &coeffs, &terms, &npoints, &degree);

    LIKWID_MARKER_START("systemSolution");
    systemSolutionTs = timestamp();
    leastSquaresSystem(points, powers, coeffs, terms, npoints, degree);
    gaussElimPivot(coeffs, terms, degree + 1);
    backSubstitution(coeffs, terms, &solution, degree + 1);
    systemSolutionTs = timestamp() - systemSolutionTs;
    LIKWID_MARKER_STOP("systemSolution");

    calcResidual(points, solution, &residuals, degree, npoints);

    printResults(solution, residuals, npoints, degree, 0, systemSolutionTs);

    freeIntervalMatrix(&points, npoints);
    freeIntervalMatrix(&coeffs, degree + 1);
    free(residuals);
    free(solution);
    free(powers);
    free(terms);

    LIKWID_MARKER_CLOSE;
    return 0;
}