#include "polynomial_ops.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float minf(float a, float b, float c, float d) {
    float min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    if (d < min) min = d;
    return min;
}

float maxf4(float a, float b, float c, float d) {
    float max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    if (d > max) max = d;
    return max;
}

float maxf2(float a, float b) { return a > b ? a : b; }

void calcInterval(double number, Interval_t *interval) {
    if (number == 0) {
        interval->min = 0;
        interval->max = 0;
    } else {
        interval->min = number;
        interval->max = nextafterf(number, INFINITY);
    }
}

void calcIntervalOperation(Interval_t *operandA, Interval_t *operandB, int exp, enum OPERATIONS operation, Interval_t *result) {
    Interval_t *copyA = (Interval_t *)malloc(sizeof(Interval_t));
    Interval_t *copyB = (Interval_t *)malloc(sizeof(Interval_t));
    memcpy(copyA, operandA, sizeof(Interval_t));
    if (operandB != NULL) memcpy(copyB, operandB, sizeof(Interval_t));

    switch (operation) {
        case SUM:
            result->min = copyA->min + copyB->min;
            result->max = copyA->max + copyB->max;
            break;
        case SUB:
            result->min = copyA->min - copyB->max;
            result->max = copyA->max - copyB->min;
            break;
        case MULT:
            result->min = minf(copyA->min * copyB->min, copyA->min * copyB->max, copyA->max * copyB->min, copyA->max * copyB->max);
            result->max = maxf4(copyA->min * copyB->min, copyA->min * copyB->max, copyA->max * copyB->min, copyA->max * copyB->max);
            break;
        case DIV:
            result->min = minf(copyA->min / copyB->min, copyA->min / copyB->max, copyA->max / copyB->min, copyA->max / copyB->max);
            result->max = maxf4(copyA->min / copyB->min, copyA->min / copyB->max, copyA->max / copyB->min, copyA->max / copyB->max);
            break;
        case POW:
            if (exp == 0) {
                result->min = 1;
                result->max = 1;
            } else if (exp % 2 != 0) {
                result->min = pow(copyA->min, exp);
                result->max = pow(copyA->max, exp);
            } else if (exp % 2 == 0 && copyA->min >= 0) {
                result->min = pow(copyA->min, exp);
                result->max = pow(copyA->max, exp);
            } else if (exp % 2 == 0 && copyA->max < 0) {
                result->min = pow(copyA->max, exp);
                result->max = pow(copyA->min, exp);
            } else {
                result->min = 0;
                result->max = maxf2(pow(copyA->min, exp), pow(copyA->max, exp));
            }
            break;
    }

    free(copyA);
    free(copyB);

    result->max = nextafterf(result->max, INFINITY);
}

void mallocMatrix(double ***matrix, int rows, int cols) {
    *matrix = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        (*matrix)[i] = (double *)malloc(cols * sizeof(double));
    }
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

void freeMatrix(double ***matrix, int size) {
    for (int i = 0; i < size; i++) {
        free((*matrix)[i]);
    }
    free(*matrix);
    *matrix = NULL;
}

void printSystem(Interval_t **a, Interval_t *b, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("[%lf, %lf] ", a[i][j].min, a[i][j].max);
        }
        printf("| [%lf, %lf]\n", b[i].min, b[i].max);
    }
}

void printPoints(Interval_t **points, int n) {
    for (int i = 0; i < n; i++) {
        printf("[%lf, %lf] = [%lf, %lf]\n", points[i][0].min, points[i][0].max, points[i][1].min, points[i][1].max);
    }
}

void perfSquare(Interval_t ***points, Interval_t **powers, Interval_t ***coeffs, Interval_t **terms, int *k, int *n) {
    double inputAux;
    Interval_t *aux = (Interval_t *)malloc(sizeof(Interval_t));

    scanf("%d", n);
    scanf("%d", k);

    mallocIntervalMatrix(points, *k, 2);
    mallocIntervalMatrix(coeffs, *n + 1, *n + 1);
    *powers = (Interval_t *)malloc((*n + *n + 1) * sizeof(Interval_t));
    *terms = (Interval_t *)malloc((*n + 1) * sizeof(Interval_t));

    for (int i = 0; i < *k; i++) {
        scanf("%lf", &inputAux);
        calcInterval(inputAux, &(*points)[i][0]);
        scanf("%lf", &inputAux);
        calcInterval(inputAux, &(*points)[i][1]);
    }

    for (int i = 0; i <= *n + *n; i++) {
        calcInterval(0, &(*powers)[i]);
        for (int j = 0; j < *k; j++) {
            calcIntervalOperation(&(*points)[j][0], NULL, i, POW, aux);        // points[j][0] ^ i
            calcIntervalOperation(aux, &(*powers)[i], 0, SUM, &(*powers)[i]);  // powers[i] += points[j][0] ^ i
        }
    }

    for (int i = 0; i <= *n; i++) {
        calcInterval(0, &(*terms)[i]);
        for (int j = 0; j < *k; j++) {
            calcIntervalOperation(&(*points)[j][0], NULL, i, POW, aux);      // points[j][0] ^ i
            calcIntervalOperation(aux, &(*points)[j][1], 0, MULT, aux);      // points[j][0] ^ i * points[j][1]
            calcIntervalOperation(aux, &(*terms)[i], 0, SUM, &(*terms)[i]);  // terms[i] += points[j][0] ^ i * points[j][1]
        }
    }

    for (int i = 0; i <= *n; i++) {
        for (int j = 0; j <= *n; j++) {
            memcpy(&(*coeffs)[i][j], &(*powers)[i + j], sizeof(Interval_t));  // coeffs[i][j] = powers[i + j]
        }
    }

    printSystem(*coeffs, *terms, *n + 1);
    free(aux);
}