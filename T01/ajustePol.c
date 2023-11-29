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
    double leastSquaresTs, systemSolTs, residualSolTs;

    initData(&points, &powers, &coeffs, &terms, &npoints, &degree);

    LIKWID_MARKER_START("leastSquaresTs");
    leastSquaresTs = timestamp();
    leastSquaresSystem(points, powers, coeffs, terms, npoints, degree);
    leastSquaresTs = timestamp() - leastSquaresTs;
    LIKWID_MARKER_STOP("leastSquaresTs");
    LIKWID_MARKER_START("systemSolTs");
    systemSolTs = timestamp();
    gaussElimPivot(coeffs, terms, degree + 1);
    backSubstitution(coeffs, terms, &solution, degree + 1);
    systemSolTs = timestamp() - systemSolTs;
    LIKWID_MARKER_STOP("systemSolTs");
    LIKWID_MARKER_START("residualSolTs");
    residualSolTs = timestamp();
    calcResidual(points, solution, &residuals, degree, npoints);
    residualSolTs = timestamp() - residualSolTs;
    LIKWID_MARKER_STOP("residualSolTs");
    printResults(solution, residuals, npoints, degree, leastSquaresTs, systemSolTs, residualSolTs);

    freeIntervalMatrix(&points, npoints);
    freeIntervalMatrix(&coeffs, degree + 1);
    free(residuals);
    free(solution);
    free(powers);
    free(terms);

    LIKWID_MARKER_CLOSE;
    return 0;
}