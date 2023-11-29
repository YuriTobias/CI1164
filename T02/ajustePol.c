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

    Point_t points;
    Interval_t *coeffs = NULL, *terms = NULL, *powers = NULL, *solution, *residuals;
    int degree, npoints;
    double leastSquaresTs, systemSolTs, residualSolTs;

    initData(&points, &powers, &coeffs, &terms, &npoints, &degree);

    LIKWID_MARKER_START("leastSquares");
    leastSquaresTs = timestamp();
    leastSquaresSystem(points, powers, coeffs, terms, npoints, degree);
    leastSquaresTs = timestamp() - leastSquaresTs;
    LIKWID_MARKER_STOP("leastSquares");
    LIKWID_MARKER_START("systemSol");
    systemSolTs = timestamp();
    gaussElimPivot(coeffs, terms, degree + 1);
    backSubstitution(coeffs, terms, &solution, degree + 1);
    systemSolTs = timestamp() - systemSolTs;
    LIKWID_MARKER_STOP("systemSol");
    LIKWID_MARKER_START("residualSol");
    residualSolTs = timestamp();
    calcResidual(points, solution, &residuals, degree, npoints);
    residualSolTs = timestamp() - residualSolTs;
    LIKWID_MARKER_STOP("residualSol");

    printResults(solution, residuals, npoints, degree, leastSquaresTs, systemSolTs, residualSolTs);

    free(points.x);
    free(points.y);
    free(coeffs);
    free(residuals);
    free(solution);
    free(powers);
    free(terms);

    LIKWID_MARKER_CLOSE;
    return 0;
}