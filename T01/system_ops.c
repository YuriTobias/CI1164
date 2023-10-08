#include "system_ops.h"

#include <fenv.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void printSystem(Interval_t **coeffs, Interval_t *terms, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("[%.8e, %.8e] ", coeffs[i][j].lower, coeffs[i][j].upper);
        }
        printf("| [%.8e, %.8e]\n", terms[i].lower, terms[i].upper);
    }
}

void printPoints(Interval_t **points, int size) {
    for (int i = 0; i < size; i++) {
        printf("[%.8e, %.8e] = [%.8e, %.8e]\n", points[i][0].lower, points[i][0].upper, points[i][1].lower, points[i][1].upper);
    }
}

void initData(Interval_t ***points, Interval_t **powers, Interval_t ***coeffs, Interval_t **terms, int *nPoints, int *degree) {
    char inputAux[100];

    if (scanf("%d", degree) == 0) {
        exit(1);
    }
    if (scanf("%d", nPoints) == 0) {
        exit(1);
    }
    getchar();

    mallocIntervalMatrix(points, *nPoints, 2);
    mallocIntervalMatrix(coeffs, *degree + 1, *degree + 1);
    *powers = (Interval_t *)malloc((*degree + *degree + 1) * sizeof(Interval_t));
    *terms = (Interval_t *)malloc((*degree + 1) * sizeof(Interval_t));

    for (int i = 0; i < *nPoints; i++) {
        if (fgets(inputAux, 100, stdin) == NULL) {
            exit(1);
        }
        char *token = strtok(inputAux, " ");
        initInterval(token, &(*points)[i][0]);
        token = strtok(NULL, " ");
        initInterval(token, &(*points)[i][1]);
    }
}

void leastSquaresSystem(Interval_t **points, Interval_t *powers, Interval_t **coeffs, Interval_t *terms, int k, int n) {
    Interval_t *aux = (Interval_t *)malloc(sizeof(Interval_t));

    for (int i = 0; i <= n + n; i++) {
        initInterval("0", &(powers)[i]);
        for (int j = 0; j < k; j++) {
            intervalOperation(&(points)[j][0], NULL, i, POW, aux);       // points[j][0] ^ i
            intervalOperation(aux, &(powers)[i], 0, SUM, &(powers)[i]);  // powers[i] += points[j][0] ^ i
        }
    }

    for (int i = 0; i <= n; i++) {
        initInterval("0", &(terms)[i]);
        for (int j = 0; j < k; j++) {
            intervalOperation(&(points)[j][0], NULL, i, POW, aux);     // points[j][0] ^ i
            intervalOperation(aux, &(points)[j][1], 0, MULT, aux);     // points[j][0] ^ i * points[j][1]
            intervalOperation(aux, &(terms)[i], 0, SUM, &(terms)[i]);  // terms[i] += points[j][0] ^ i * points[j][1]
        }
    }

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            memcpy(&(coeffs)[i][j], &(powers)[i + j], sizeof(Interval_t));  // coeffs[i][j] = powers[i + j]
        }
    }

    free(aux);
}

int findPivotLine(Interval_t **coeffs, int i, int size) {
    int bigger = i;
    for (int j = i + 1; j < size; j++) {
        if (((coeffs[j][i].upper + coeffs[j][i].lower) / 2) > ((coeffs[bigger][i].upper + coeffs[bigger][i].lower) / 2)) {
            bigger = j;
        }
    }
    return bigger;
}

void swapSystemLines(Interval_t **coeffs, Interval_t *terms, int i, int iPivot) {
    Interval_t *aux = coeffs[i];
    coeffs[i] = coeffs[iPivot];
    coeffs[iPivot] = aux;
    Interval_t auxB = terms[i];
    terms[i] = terms[iPivot];
    terms[iPivot] = auxB;
}

void gaussElimPivot(Interval_t **coeffs, Interval_t *terms, int size) {
    Interval_t m, result;

    for (int i = 0; i < size; i++) {
        int iPivot = findPivotLine(coeffs, i, size);
        if (iPivot != i) {
            swapSystemLines(coeffs, terms, i, iPivot);
        }
        for (int k = i + 1; k < size; k++) {
            intervalOperation(&(coeffs)[k][i], &(coeffs)[i][i], 0, DIV, &m);  // m = A[k][i] / A[i][i]
            initInterval("0", &(coeffs)[k][i]);                               // A[k][i] = 0
            for (int j = i + 1; j < size; j++) {
                intervalOperation(&m, &(coeffs)[i][j], 0, MULT, &result);              // result = m * A[i][j]
                intervalOperation(&(coeffs)[k][j], &result, 0, SUB, &(coeffs)[k][j]);  // A[k][j] -= result
            }
            intervalOperation(&m, &(terms)[i], 0, MULT, &result);          // result = m * b[i]
            intervalOperation(&(terms)[k], &result, 0, SUB, &(terms)[k]);  // b[k] -= result
        }
    }
}

void backSubstitution(Interval_t **coeffs, Interval_t *terms, Interval_t **solution, int size) {
    Interval_t result;

    *solution = (Interval_t *)malloc((size) * sizeof(Interval_t));
    for (int i = size - 1; i >= 0; i--) {
        (*solution)[i] = terms[i];
        for (int j = i + 1; j < size; j++) {
            intervalOperation(&(coeffs)[i][j], &(*solution)[j], 0, MULT, &result);
            intervalOperation(&(*solution)[i], &result, 0, SUB, &(*solution)[i]);
        }
        intervalOperation(&(*solution)[i], &(coeffs)[i][i], 0, DIV, &(*solution)[i]);
    }
}

void calcResidual(Interval_t **points, Interval_t *solution, Interval_t **residuals, int degree, int npoints) {
    Interval_t result, mult;

    *residuals = (Interval_t *)malloc((npoints) * sizeof(Interval_t));
    for (int i = 0; i < npoints; i++) {
        initInterval("0", &result);
        for (int j = 0; j <= degree; j++) {
            intervalOperation(&(points)[i][0], NULL, j, POW, &mult);   // mult = points[i][0] ^ j
            intervalOperation(&mult, &(solution)[j], 0, MULT, &mult);  // mult = points[i][0] ^ j * solution[j]
            intervalOperation(&result, &mult, 0, SUM, &result);        // result += points[i][0] ^ j * solution[j]
        }
        intervalOperation(&(points)[i][1], &result, 0, SUB, &(*residuals)[i]);  // residuals[i] = points[i][1] - result
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
    printf("%.8e\n", leastSquaresTs);
    printf("%.8e\n", systemSolutionTs);
}