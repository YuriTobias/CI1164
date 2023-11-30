#include "system_ops.h"

#include <fenv.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UF 4

double maxDouble2(double a, double b) { return a > b ? a : b; }

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
    Interval_t aux[UF], auxB[UF];
    int coeffsIndex, powersIndex;

    // Initializing the vector of terms and powers
    for (int i = 0; i <= degree; i++) {
        initInterval("0", &terms[i]);
        initInterval("0", &powers[i]);
    }
    for (int i = degree + 1; i <= degree + degree; i++) {
        initInterval("0", &powers[i]);
    }

    // The first sum is x raised to 0, that is, 1 * k = k
    powers[0].lower = k;
    powers[0].upper = k;

    // Unroll em k, the biggest vector
    for (int i = 0; i < k - k % UF; i += UF) {
        // Variables that stores the last power
        aux[0].lower = 1.0;
        aux[0].upper = 1.0;
        aux[1].lower = 1.0;
        aux[1].upper = 1.0;
        aux[2].lower = 1.0;
        aux[2].upper = 1.0;
        aux[3].lower = 1.0;
        aux[3].upper = 1.0;
        // Calculates all necessary powers for the same x
        // Take the opportunity to calculate the independent term as well
        // It goes up to degree due to the terms
        for (int j = 1; j <= degree; j++) {
            // x
            intervalMult(&(points.x[i]), &aux[0], &aux[0]);             // aux[0] = x[i] ^ j
            intervalSum(&aux[0], &(powers)[j], &(powers)[j]);           // powers[j] += x[i] ^ j
            intervalMult(&(points.y[i]), &aux[0], &auxB[0]);            // auxB[0] = y[i] * x[i] ^ j
            intervalSum(&(terms)[j], &auxB[0], &(terms)[j]);            // terms[j] += y[i] * x[i] ^ j
            // x + 1
            intervalMult(&(points.x[i + 1]), &aux[1], &aux[1]);
            intervalSum(&aux[1], &(powers)[j], &(powers)[j]);
            intervalMult(&(points.y[i + 1]), &aux[1], &auxB[1]);
            intervalSum(&(terms)[j], &auxB[1], &(terms)[j]);
            // x + 2
            intervalMult(&(points.x[i + 2]), &aux[2], &aux[2]);
            intervalSum(&aux[2], &(powers)[j], &(powers)[j]);
            intervalMult(&(points.y[i + 2]), &aux[2], &auxB[2]);
            intervalSum(&(terms)[j], &auxB[2], &(terms)[j]);
            // x + 3
            intervalMult(&(points.x[i + 3]), &aux[3], &aux[3]);
            intervalSum(&aux[3], &(powers)[j], &(powers)[j]);
            intervalMult(&(points.y[i + 3]), &aux[3], &auxB[3]);
            intervalSum(&(terms)[j], &auxB[3], &(terms)[j]);
        }
        // Calculates the rest, that is, what wasn't calculated in the previous loop
        for (int j = degree + 1; j <= degree + degree; j++) {
            // x
            intervalMult(&(points.x[i]), &aux[0], &aux[0]);             // aux[0] = x[i] ^ j
            intervalSum(&aux[0], &(powers)[j], &(powers)[j]);           // powers[j] += x[i] ^ j
            // x + 1
            intervalMult(&(points.x[i + 1]), &aux[1], &aux[1]);
            intervalSum(&aux[1], &(powers)[j], &(powers)[j]);
            // x + 2
            intervalMult(&(points.x[i + 2]), &aux[2], &aux[2]);
            intervalSum(&aux[2], &(powers)[j], &(powers)[j]);
            // x + 3
            intervalMult(&(points.x[i + 3]), &aux[3], &aux[3]);
            intervalSum(&aux[3], &(powers)[j], &(powers)[j]);
        }
    }
    // Remainder loop
    for (int i = k - k % UF; i < k; i++) {
        aux[0].lower = 1.0;
        aux[0].upper = 1.0;
        for (int j = 1; j <= degree; j++) {
            intervalMult(&(points.x[i]), &aux[0], &aux[0]);             // aux[0] = x[i] ^ j
            intervalSum(&aux[0], &(powers)[j], &(powers)[j]);           // powers[j] += x[i] ^ j
            intervalMult(&(points.y[i]), &aux[0], &auxB[0]);            // auxB[0] = y[i] * x[i] ^ j
            intervalSum(&(terms)[j], &auxB[0], &(terms)[j]);            // terms[j] += y[i] * x[i] ^ j
        }
        for (int j = degree + 1; j <= degree + degree; j++) {
            intervalMult(&(points.x[i]), &aux[0], &aux[0]);             // aux[0] = x[i] ^ j
            intervalSum(&aux[0], &(powers)[j], &(powers)[j]);           // powers[j] += x[i] ^ j
        }
    }

    // The first sum of terms is only y
    for (int i = 0; i < k; i++) {
        intervalSum(&(terms)[0], &(points.y[i]), &(terms)[0]);          // terms[0] += y[i]
    }

    // Saves the value of the coefficients
    for (int i = 0; i <= degree; i++) {
        for (int j = 0; j <= degree; j++) {
            coeffsIndex = i * (degree + 1) + j;
            powersIndex = i + j;
            memcpy(&(coeffs)[coeffsIndex], &(powers)[powersIndex], sizeof(Interval_t));  // coeffs[i][j] = powers[i + j]
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
            intervalDiv(&(coeffs)[k * (size) + i], &(coeffs)[i * (size) + i], &m);                      // m = A[k][i] / A[i][i]
            initInterval("0", &(coeffs)[k * (size) + i]);                                               // A[k][i] = 0
            // Unrolled on j
            for (int j = i + 1; j < (i + 1) - ((i + 1) % UF) + UF && j < size - size % UF; j++) {
                intervalMult(&m, &(coeffs)[i * (size) + j], &result);                                   // result = m * A[i][j]
                intervalSub(&(coeffs)[k * (size) + j], &result, &(coeffs)[k * (size) + j]);             // A[k][j] -= result
            }
            // Unrolled on j
            for (int j = (i + 1) - ((i + 1) % UF) + UF; j < size - size % UF; j += UF) {
                intervalMult(&m, &(coeffs)[i * (size) + j], &result);                                   // result = m * A[i][j]
                intervalSub(&(coeffs)[k * (size) + j], &result, &(coeffs)[k * (size) + j]);             // A[k][j] -= result
                intervalMult(&m, &(coeffs)[i * (size) + j + 1], &result2);                              // result2 = m * A[i][j + 1]
                intervalSub(&(coeffs)[k * (size) + j + 1], &result2, &(coeffs)[k * (size) + j + 1]);    // A[k][j+1] -= result2
                intervalMult(&m, &(coeffs)[i * (size) + j + 2], &result3);                              // result = m * A[i][j + 2]
                intervalSub(&(coeffs)[k * (size) + j + 2], &result3, &(coeffs)[k * (size) + j + 2]);    // A[k][j+2] -= result3
                intervalMult(&m, &(coeffs)[i * (size) + j + 3], &result4);                              // result = m * A[i][j + 3]
                intervalSub(&(coeffs)[k * (size) + j + 3], &result4, &(coeffs)[k * (size) + j + 3]);    // A[k][j+3] -= result4 
            }
            // Remainder loop
            for (int j = size - size % UF; j < size; j++) {
                intervalMult(&m, &(coeffs)[i * (size) + j], &result);                                   // A[k][j] -= result
                intervalSub(&(coeffs)[k * (size) + j], &result, &(coeffs)[k * (size) + j]);             // result = m * A[i][j]
            }
            intervalMult(&m, &(terms)[i], &result);                                                     // result = m * b[i]
            intervalSub(&(terms)[k], &result, &(terms)[k]);                                             // b[k] -= result
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
            intervalMult(&(coeffs)[i * (size) + j], &(*solution)[j], &result);
            intervalSub(&(*solution)[i], &result, &(*solution)[i]);
        }
        intervalDiv(&(*solution)[i], &(coeffs)[i * (size) + i], &(*solution)[i]);
    }
}

void calcResidual(Point_t points, Interval_t *restrict solution, Interval_t **restrict residuals, int degree, int npoints) {
    Interval_t result, result2, result3, result4, mult, powers, powers2, powers3, powers4;
    *residuals = (Interval_t *)malloc((npoints) * sizeof(Interval_t));
    // Unrolled on i
    for (int i = 0; i < npoints - npoints % UF; i += UF) {
        initInterval("0", &result);
        initInterval("0", &result2);
        initInterval("0", &result3);
        initInterval("0", &result4);
        initInterval("1", &powers);
        initInterval("1", &powers2);
        initInterval("1", &powers3);
        initInterval("1", &powers4);
        for (int j = 0; j <= degree; j++) {
            intervalMult(&powers, &(solution)[j], &mult);                   // mult = points.x[i] ^ j * solution[j]
            intervalSum(&result, &mult, &result);                           // result += points.x[i] ^ j * solution[j]
            intervalMult(&powers, &(points.x[i]), &powers);                 // powers = points.x[i] ^ j
            intervalMult(&powers2, &(solution)[j], &mult);                  // mult = points.x[i+1] ^ j * solution[j]                    
            intervalSum(&result2, &mult, &result2);                         // result += points.x[i+1] ^ j * solution[j]                    
            intervalMult(&powers2, &(points.x[i + 1]), &powers2);           // powers2 = points.x[i+1] ^ j                    
            intervalMult(&powers3, &(solution)[j], &mult);                  // mult = points.x[i+2] ^ j * solution[j]                    
            intervalSum(&result3, &mult, &result3);                         // result += points.x[i+2] ^ j * solution[j]                    
            intervalMult(&powers3, &(points.x[i + 2]), &powers3);           // powers3 = points.x[i+2] ^ j                    
            intervalMult(&powers4, &(solution)[j], &mult);                  // mult = points.x[i+3] ^ j * solution[j]                    
            intervalSum(&result4, &mult, &result4);                         // result += points.x[i+3] ^ j * solution[j]              
            intervalMult(&powers4, &(points.x[i + 3]), &powers4);           // powers4 = points.x[i+3] ^ j
        }
        intervalSub(&(points.y[i]), &result, &(*residuals)[i]);             // residuals[i] = points[i][1] - result
        intervalSub(&(points.y[i + 1]), &result2, &(*residuals)[i + 1]);
        intervalSub(&(points.y[i + 2]), &result3, &(*residuals)[i + 2]);
        intervalSub(&(points.y[i + 3]), &result4, &(*residuals)[i + 3]);
    }
    // Remainder loop
    for (int i = npoints - npoints % UF; i < npoints; i++) {
        initInterval("0", &result);
        initInterval("1", &powers);
        for (int j = 0; j <= degree; j++) {
            intervalMult(&powers, &(solution)[j], &mult);               // mult = points.x[i] ^ j * solution[j]
            intervalSum(&result, &mult, &result);                       // result += points.x[i] ^ j * solution[j]      
            intervalMult(&powers, &(points.x[i]), &powers);             // powers = points.x[i] ^ j
        }
        intervalSub(&(points.y[i]), &result, &(*residuals)[i]);     // residuals[i] = points.y[i] - result
    }
}

void printPoints(Interval_t *points, int size) {
    for (int i = 0; i < size; i++) {
        printf("[%.8e, %.8e] = [%.8e, %.8e]\n", points[2 * i].lower, points[2 * i].upper, points[2 * i + 1].lower, points[2 * i + 1].upper);
    }
}

void printSystem(Interval_t *coeffs, Interval_t *terms, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("[%.8e, %.8e] ", coeffs[i * (size) + j].lower, coeffs[i * (size) + j].upper);
        }
        printf("| [%.8e, %.8e]\n", terms[i].lower, terms[i].upper);
    }
}

void printResults(Interval_t *solution, Interval_t *residuals, int npoints, int degree, double leastSquaresTs, double systemSolutionTs,
                  double residualTs) {
    for (int i = 0; i <= degree; i++) {
        printf("[%.8e , %.8e] ", solution[i].lower, solution[i].upper);
    }
    printf("\n");
    for (int i = 0; i < npoints; i++) {
        printf("[%.8e , %.8e] ", residuals[i].lower, residuals[i].upper);
    }
    printf("\n");
    printf("LeastSquares: %.8e\n", leastSquaresTs);
    printf("SystemSol: %.8e\n", systemSolutionTs);
    printf("ResidualSol: %.8e\n", residualTs);
}