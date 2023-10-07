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

    Interval_t **points, **coeffs, **cpycoeffs, *terms, *cpyterms, *powers, *solution, *residues;
    int degree, npoints;
    double leastSquaresTs, systemSolutionTs;

    initData(&points, &powers, &coeffs, &terms, &npoints, &degree);

    LIKWID_MARKER_START("leastSquares");
    leastSquaresTs = timestamp();
    leastSquaresSystem(points, powers, coeffs, terms, npoints, degree);
    leastSquaresTs = timestamp() - leastSquaresTs;
    LIKWID_MARKER_STOP("leastSquares");

    copyMatrixInterval(coeffs, &cpycoeffs, degree + 1, degree + 1);
    copyVectorInterval(terms, &cpyterms, degree + 1);

    LIKWID_MARKER_START("systemSolution");
    systemSolutionTs = timestamp();
    gaussElimPivot(cpycoeffs, cpyterms, degree + 1);
    backSubstitution(cpycoeffs, cpyterms, &solution, degree + 1);
    systemSolutionTs = timestamp() - systemSolutionTs;
    LIKWID_MARKER_STOP("systemSolution");

    calcResidue(coeffs, terms, solution, &residues, degree + 1);

    printResults(solution, residues, degree + 1, leastSquaresTs, systemSolutionTs);

    freeIntervalMatrix(&cpycoeffs, degree + 1);
    freeIntervalMatrix(&points, npoints);
    freeIntervalMatrix(&coeffs, degree + 1);
    free(residues);
    free(solution);
    free(cpyterms);
    free(powers);
    free(terms);

    LIKWID_MARKER_CLOSE;
    return 0;
}