#include "system_ops.h"

#include <fenv.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UF 4

double minDouble4(double a, double b, double c, double d) {
    double lower = a;
    if (b < lower) lower = b;
    if (c < lower) lower = c;
    if (d < lower) lower = d;
    return lower;
}

double maxDouble4(double a, double b, double c, double d) {
    double upper = a;
    if (b > upper) upper = b;
    if (c > upper) upper = c;
    if (d > upper) upper = d;
    return upper;
}

double maxDouble2(double a, double b) { return a > b ? a : b; }

void printSystem(Interval_t *coeffs, Interval_t *terms, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("[%.8e, %.8e] ", coeffs[i * (size) + j].lower, coeffs[i * (size) + j].upper);
        }
        printf("| [%.8e, %.8e]\n", terms[i].lower, terms[i].upper);
    }
}

void printPoints(Interval_t *points, int size) {
    for (int i = 0; i < size; i++) {
        printf("[%.8e, %.8e] = [%.8e, %.8e]\n", points[2 * i].lower, points[2 * i].upper, points[2 * i + 1].lower, points[2 * i + 1].upper);
    }
}

void initData(Point_t *points, Interval_t **powers, Interval_t **coeffs, Interval_t **terms, int *nPoints, int *degree) {
    char inputAux[100];
    int trueDegree;

    if (scanf("%d", degree) == 0) {
        exit(1);
    }
    if (scanf("%d", nPoints) == 0) {
        exit(1);
    }
    getchar();

    trueDegree = *degree + 1;

    points->x = (Interval_t *)malloc((*nPoints) * sizeof(Interval_t));
    points->y = (Interval_t *)malloc((*nPoints) * sizeof(Interval_t));
    *coeffs = (Interval_t *)malloc((trueDegree * trueDegree) * sizeof(Interval_t));
    *powers = (Interval_t *)malloc((2 * (*degree) + 1) * sizeof(Interval_t));
    *terms = (Interval_t *)malloc(trueDegree * sizeof(Interval_t));

    for (int i = 0; i < *nPoints; i++) {
        if (fgets(inputAux, 100, stdin) == NULL) {
            exit(1);
        }
        char *token = strtok(inputAux, " ");
        initInterval(token, &(points->x[i]));
        token = strtok(NULL, " ");
        initInterval(token, &(points->y[i]));
    }
}

void leastSquaresSystem(Point_t points, Interval_t *restrict powers, Interval_t *restrict coeffs, Interval_t *restrict terms, int k,
                        int degree) {
    Interval_t aux;
    int coeffsIndex, powersIndex;

    // Unrolled on i
    for (int i = 0; i < (degree + degree) - (degree + degree) % UF; i += UF) {
        initInterval("0", &powers[i]);
        initInterval("0", &(powers)[i + 1]);
        initInterval("0", &(powers)[i + 2]);
        initInterval("0", &(powers)[i + 3]);
        for (int j = 0; j < k; j++) {
            intervalOperation(&(points.x[j]), NULL, i, POW, &aux);                // points->x[j] ^ i
            intervalOperation(&aux, &(powers)[i], 0, SUM, &(powers)[i]);          // powers[i] += points->x[j] ^ i
            intervalOperation(&(points.x[j]), NULL, i + 1, POW, &aux);            // points->x[j] ^ (i + 1)
            intervalOperation(&aux, &(powers)[i + 1], 0, SUM, &(powers)[i + 1]);  // powers[i + 1] += points->x[j] ^ (i + 1)
            intervalOperation(&(points.x[j]), NULL, i + 2, POW, &aux);            // points->x[j] ^ (i + 2)
            intervalOperation(&aux, &(powers)[i + 2], 0, SUM, &(powers)[i + 2]);  // powers[i + 2] += points->x[j] ^ (i + 2)
            intervalOperation(&(points.x[j]), NULL, i + 3, POW, &aux);            // points->x[j] ^ (i + 3)
            intervalOperation(&aux, &(powers)[i + 3], 0, SUM, &(powers)[i + 3]);  // powers[i + 3] += points->x[j] ^ (i + 3)
        }
    }
    // Remainder loop
    for (int i = (degree + degree) - (degree + degree) % UF; i <= (degree + degree); i++) {
        initInterval("0", &(powers)[i]);
        for (int j = 0; j < k; j++) {
            intervalOperation(&(points.x[j]), NULL, i, POW, &aux);        // points->x[j] ^ i
            intervalOperation(&aux, &(powers)[i], 0, SUM, &(powers)[i]);  // powers[i] += points->x[j] ^ i
        }
    }

    // Unrolled on i
    for (int i = 0; i < degree - degree % UF; i += UF) {
        initInterval("0", &(terms)[i]);
        initInterval("0", &(terms)[i + 1]);
        initInterval("0", &(terms)[i + 2]);
        initInterval("0", &(terms)[i + 3]);
        for (int j = 0; j < k; j++) {
            intervalOperation(&(points.x[j]), NULL, i, POW, &aux);      // points->x[j] ^ i
            intervalOperation(&aux, &(points.y[j]), 0, MULT, &aux);     // points->x[j] ^ i * points->y[j]
            intervalOperation(&aux, &(terms)[i], 0, SUM, &(terms)[i]);  // terms[i] += points->x[j] ^ i * points->y[j]

            intervalOperation(&(points.x[j]), NULL, i + 1, POW, &aux);          // points->x[j] ^ (i + 1)
            intervalOperation(&aux, &(points.y[j]), 0, MULT, &aux);             // points->x[j] ^ (i + 1) * points->y[j]
            intervalOperation(&aux, &(terms)[i + 1], 0, SUM, &(terms)[i + 1]);  // terms[i + 1] += points->x[j] ^ (i + 1) * points->y[j]

            intervalOperation(&(points.x[j]), NULL, i + 2, POW, &aux);          // points->x[j] ^ (i + 2)
            intervalOperation(&aux, &(points.y[j]), 0, MULT, &aux);             // points->x[j] ^ (i + 2) * points->y[j]
            intervalOperation(&aux, &(terms)[i + 2], 0, SUM, &(terms)[i + 2]);  // terms[i + 2] += points->x[j] ^ (i + 2) * points->y[j]

            intervalOperation(&(points.x[j]), NULL, i + 3, POW, &aux);          // points->x[j] ^ (i + 3)
            intervalOperation(&aux, &(points.y[j]), 0, MULT, &aux);             // points->x[j] ^ (i + 3) * points->y[j]
            intervalOperation(&aux, &(terms)[i + 3], 0, SUM, &(terms)[i + 3]);  // terms[i + 3] += points->x[j] ^ (i + 3) * points->y[j]
        }
    }
    // Remainder loop
    for (int i = degree - degree % UF; i <= degree; i++) {
        initInterval("0", &(terms)[i]);
        for (int j = 0; j < k; j++) {
            intervalOperation(&(points.x[j]), NULL, i, POW, &aux);      // points->x[j] ^ i
            intervalOperation(&aux, &(points.y[j]), 0, MULT, &aux);     // points->x[j] ^ i * points->y[j]
            intervalOperation(&aux, &(terms)[i], 0, SUM, &(terms)[i]);  // terms[i] += points->x[j] ^ i * points->y[j]
        }
    }

    // Unrolled on j
    for (int i = 0; i <= degree; i++) {
        for (int j = 0; j < degree - degree % UF; j += UF) {
            coeffsIndex = i * (degree + 1) + j;
            powersIndex = i + j;
            memcpy(&(coeffs)[coeffsIndex], &(powers)[powersIndex], sizeof(Interval_t));  // coeffs[i][j] = powers[i + j]
            memcpy(&(coeffs)[coeffsIndex + 1], &(powers)[powersIndex + 1], sizeof(Interval_t));
            memcpy(&(coeffs)[coeffsIndex + 2], &(powers)[powersIndex + 2], sizeof(Interval_t));
            memcpy(&(coeffs)[coeffsIndex + 3], &(powers)[powersIndex + 3], sizeof(Interval_t));
        }
        // Remainder loop
        for (int j = degree - degree % UF; j <= degree; j++) {
            memcpy(&(coeffs)[i * (degree + 1) + j], &(powers)[i + j], sizeof(Interval_t));  // coeffs[i][j] = powers[i + j]
        }
    }
}

int findPivotLine(Interval_t *coeffs, int i, int size) {
    int bigger = i;
    // Unroll not possible due of dependency
    for (int j = i + 1; j < size; j++) {
        if (((coeffs[j * (size) + i].upper + coeffs[j * (size) + i].lower) / 2) >
            ((coeffs[bigger * (size) + i].upper + coeffs[bigger * (size) + i].lower) / 2)) {
            bigger = j;
        }
    }
    return bigger;
}

void swapSystemLines(Interval_t *coeffs, Interval_t *terms, int i, int iPivot, int n) {
    Interval_t *aux = (Interval_t *)malloc(n * sizeof(Interval_t));
    memcpy(aux, &(coeffs)[i * (n)], n * sizeof(Interval_t));
    memcpy(&(coeffs)[i * (n)], &(coeffs)[iPivot * (n)], n * sizeof(Interval_t));
    memcpy(&(coeffs)[iPivot * (n)], aux, n * sizeof(Interval_t));
    Interval_t auxB = terms[i];
    terms[i] = terms[iPivot];
    terms[iPivot] = auxB;
    free(aux);
}

void gaussElimPivot(Interval_t *restrict coeffs, Interval_t *restrict terms, int size) {
    Interval_t m, result, result2, result3, result4;

    for (int i = 0; i < size; i++) {
        int iPivot = findPivotLine(coeffs, i, size);
        if (iPivot != i) {
            swapSystemLines(coeffs, terms, i, iPivot, size);
        }
        for (int k = i + 1; k < size; k++) {
            intervalOperation(&(coeffs)[k * (size) + i], &(coeffs)[i * (size) + i], 0, DIV, &m);  // m = A[k][i] / A[i][i]
            initInterval("0", &(coeffs)[k * (size) + i]);                                         // A[k][i] = 0
            // Remainder loop
            for (int j = i + 1; j < (i + 1) - ((i + 1) % UF) + UF && j < size - size % UF; j++) {
                intervalOperation(&m, &(coeffs)[i * (size) + j], 0, MULT, &result);                        // result = m * A[i][j]
                intervalOperation(&(coeffs)[k * (size) + j], &result, 0, SUB, &(coeffs)[k * (size) + j]);  // A[k][j] -= result
            }
            // Unrolled on j
            for (int j = (i + 1) - ((i + 1) % UF) + UF; j < size - size % UF; j += UF) {
                intervalOperation(&m, &(coeffs)[i * (size) + j], 0, MULT, &result);                        // result = m * A[i][j]
                intervalOperation(&(coeffs)[k * (size) + j], &result, 0, SUB, &(coeffs)[k * (size) + j]);  // A[k][j] -= result
                intervalOperation(&m, &(coeffs)[i * (size) + j + 1], 0, MULT, &result2);                   // result = m * A[i][j + 1]
                intervalOperation(&(coeffs)[k * (size) + j + 1], &result2, 0, SUB, &(coeffs)[k * (size) + j + 1]);  // A[k][j+1] -= result2
                intervalOperation(&m, &(coeffs)[i * (size) + j + 2], 0, MULT, &result3);  // result = m * A[i][j + 2]
                intervalOperation(&(coeffs)[k * (size) + j + 2], &result3, 0, SUB, &(coeffs)[k * (size) + j + 2]);  // A[k][j+2] -= result3
                intervalOperation(&m, &(coeffs)[i * (size) + j + 3], 0, MULT, &result4);  // result = m * A[i][j + 3]
                intervalOperation(&(coeffs)[k * (size) + j + 3], &result4, 0, SUB, &(coeffs)[k * (size) + j + 3]);  // A[k][j+3] -= result4
            }
            // Remainder loop
            for (int j = size - size % UF; j < size; j++) {
                intervalOperation(&m, &(coeffs)[i * (size) + j], 0, MULT, &result);                        // result = m * A[i][j]
                intervalOperation(&(coeffs)[k * (size) + j], &result, 0, SUB, &(coeffs)[k * (size) + j]);  // A[k][j] -= result
            }
            intervalOperation(&m, &(terms)[i], 0, MULT, &result);          // result = m * b[i]
            intervalOperation(&(terms)[k], &result, 0, SUB, &(terms)[k]);  // b[k] -= result
        }
    }
}

void backSubstitution(Interval_t *coeffs, Interval_t *terms, Interval_t **solution, int size) {
    Interval_t result;
    *solution = (Interval_t *)malloc((size) * sizeof(Interval_t));
    // Unroll not possible due of dependency
    for (int i = size - 1; i >= 0; i--) {
        (*solution)[i] = terms[i];
        for (int j = i + 1; j < size; j++) {
            intervalOperation(&(coeffs)[i * (size) + j], &(*solution)[j], 0, MULT, &result);
            intervalOperation(&(*solution)[i], &result, 0, SUB, &(*solution)[i]);
        }
        intervalOperation(&(*solution)[i], &(coeffs)[i * (size) + i], 0, DIV, &(*solution)[i]);
    }
}

void calcResidual(Point_t points, Interval_t *restrict solution, Interval_t **restrict residuals, int degree, int npoints) {
    Interval_t result, result2, result3, result4, mult;
    *residuals = (Interval_t *)malloc((npoints) * sizeof(Interval_t));
    // Unrolled on i
    for (int i = 0; i < npoints - npoints % UF; i += UF) {
        initInterval("0", &result);
        initInterval("0", &result2);
        initInterval("0", &result3);
        initInterval("0", &result4);
        for (int j = 0; j <= degree; j++) {
            intervalOperation(&(points.x[i]), NULL, j, POW, &mult);      // mult = points[i][0] ^ j
            intervalOperation(&mult, &(solution)[j], 0, MULT, &mult);    // mult = points[i][0] ^ j * solution[j]
            intervalOperation(&result, &mult, 0, SUM, &result);          // result += points[i][0] ^ j * solution[j]
            intervalOperation(&(points.x[i + 1]), NULL, j, POW, &mult);  // mult = points[i][0] ^ j
            intervalOperation(&mult, &(solution)[j], 0, MULT, &mult);    // mult = points[i][0] ^ j * solution[j]
            intervalOperation(&result2, &mult, 0, SUM, &result2);        // result += points[i][0] ^ j * solution[j]
            intervalOperation(&(points.x[i + 2]), NULL, j, POW, &mult);  // mult = points[i][0] ^ j
            intervalOperation(&mult, &(solution)[j], 0, MULT, &mult);    // mult = points[i][0] ^ j * solution[j]
            intervalOperation(&result3, &mult, 0, SUM, &result3);        // result += points[i][0] ^ j * solution[j]
            intervalOperation(&(points.x[i + 3]), NULL, j, POW, &mult);  // mult = points[i][0] ^ j
            intervalOperation(&mult, &(solution)[j], 0, MULT, &mult);    // mult = points[i][0] ^ j * solution[j]
            intervalOperation(&result4, &mult, 0, SUM, &result4);        // result += points[i][0] ^ j * solution[j]
        }
        intervalOperation(&(points.y[i]), &result, 0, SUB, &(*residuals)[i]);  // residuals[i] = points[i][1] - result
        intervalOperation(&(points.y[i + 1]), &result2, 0, SUB, &(*residuals)[i + 1]);
        intervalOperation(&(points.y[i + 2]), &result3, 0, SUB, &(*residuals)[i + 2]);
        intervalOperation(&(points.y[i + 3]), &result4, 0, SUB, &(*residuals)[i + 3]);
    }
    // Remainder loop
    for (int i = npoints - npoints % UF; i < npoints; i++) {
        initInterval("0", &result);
        for (int j = 0; j <= degree; j++) {
            intervalOperation(&(points.x[i]), NULL, j, POW, &mult);    // mult = points[i][0] ^ j
            intervalOperation(&mult, &(solution)[j], 0, MULT, &mult);  // mult = points[i][0] ^ j * solution[j]
            intervalOperation(&result, &mult, 0, SUM, &result);        // result += points[i][0] ^ j * solution[j]
        }
        intervalOperation(&(points.y[i]), &result, 0, SUB, &(*residuals)[i]);  // residuals[i] = points[i][1] - result
    }
}

void printResults(Interval_t *solution, Interval_t *residuals, int npoints, int degree, double leastSquaresTs, double systemSolutionTs) {
    for (int i = 0; i <= degree; i++) {
        printf("[%.8e , %.8e] ", solution[i].lower, solution[i].upper);
    }
    printf("\n");
    for (int i = 0; i < npoints; i++) {
        printf("[%.8e , %.8e] ", residuals[i].lower, residuals[i].upper);
    }
    printf("\n");
    // printf("%.8e\n", leastSquaresTs);
    printf("OpTime: %.8e\n", systemSolutionTs);
}