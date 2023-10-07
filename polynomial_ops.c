#include "polynomial_ops.h"

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

void initInterval(char *number, Interval_t *interval) {
    fesetround(FE_DOWNWARD);
    interval->lower = atof(number);
    fesetround(FE_UPWARD);
    interval->upper = atof(number);
}

void intervalOperation(Interval_t *operandA, Interval_t *operandB, int exp, enum OPERATIONS operation, Interval_t *result) {
    Interval_t *copyA = (Interval_t *)malloc(sizeof(Interval_t));
    Interval_t *copyB = (Interval_t *)malloc(sizeof(Interval_t));
    memcpy(copyA, operandA, sizeof(Interval_t));
    if (operandB != NULL) memcpy(copyB, operandB, sizeof(Interval_t));

    switch (operation) {
        case SUM:
            result->lower = copyA->lower + copyB->lower;
            result->upper = copyA->upper + copyB->upper;
            break;
        case SUB:
            result->lower = copyA->lower - copyB->upper;
            result->upper = copyA->upper - copyB->lower;
            break;
        case MULT:
            result->lower = minDouble4(copyA->lower * copyB->lower, copyA->lower * copyB->upper, copyA->upper * copyB->lower,
                                       copyA->upper * copyB->upper);
            result->upper = maxDouble4(copyA->lower * copyB->lower, copyA->lower * copyB->upper, copyA->upper * copyB->lower,
                                       copyA->upper * copyB->upper);
            break;
        case DIV:
            if (copyB->lower <= 0 && copyB->upper >= 0) {
                result->lower = -INFINITY;
                result->upper = INFINITY;
                break;
            }
            result->lower = minDouble4(copyA->lower / copyB->lower, copyA->lower / copyB->upper, copyA->upper / copyB->lower,
                                       copyA->upper / copyB->upper);
            result->upper = maxDouble4(copyA->lower / copyB->lower, copyA->lower / copyB->upper, copyA->upper / copyB->lower,
                                       copyA->upper / copyB->upper);
            break;
        case POW:
            if (exp == 0) {
                result->lower = 1;
                result->upper = 1;
            } else if (exp % 2 != 0) {
                result->lower = pow(copyA->lower, exp);
                result->upper = pow(copyA->upper, exp);
            } else if (exp % 2 == 0 && copyA->lower >= 0) {
                result->lower = pow(copyA->lower, exp);
                result->upper = pow(copyA->upper, exp);
            } else if (exp % 2 == 0 && copyA->upper < 0) {
                result->lower = pow(copyA->upper, exp);
                result->upper = pow(copyA->lower, exp);
            } else {
                result->lower = 0;
                result->upper = maxDouble2(pow(copyA->lower, exp), pow(copyA->upper, exp));
            }
            break;
    }

    free(copyA);
    free(copyB);
}

void mallocIntervalMatrix(Interval_t ***matrix, int rows, int cols) {
    *matrix = (Interval_t **)malloc(rows * sizeof(Interval_t *));
    for (int i = 0; i < rows; i++) {
        (*matrix)[i] = (Interval_t *)malloc(cols * sizeof(Interval_t));
    }
}

void freeIntervalMatrix(Interval_t ***matrix, int size) {
    for (int i = 0; i < size; i++) {
        free((*matrix)[i]);
    }
    free(*matrix);
    *matrix = NULL;
}

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