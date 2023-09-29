#include "polynomial_ops.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float minf(float a, float b, float c, float d) {
    float min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    if (d < min) min = d;
    return min;
}

float maxf(float a, float b, float c, float d) {
    float max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    if (d > max) max = d;
    return max;
}

void calcInterval(double number, double *interval) {
    if (number == 0) {
        interval[0] = 0;
        interval[1] = 0;
    } else {
        interval[0] = number;
        interval[1] = nextafterf(number, INFINITY);
    }
}

void calcIntervalOperation(double *operandA, double *operandB, enum OPERATIONS operation, double *result) {
    switch (operation) {
        case SUM:
            result[0] = operandA[0] + operandB[0];
            result[1] = operandA[1] + operandB[1];
            break;
        case SUB:
            result[0] = operandA[0] - operandB[1];
            result[1] = operandA[1] - operandB[0];
            break;
        case MULT:
            result[0] = minf(operandA[0] * operandB[0], operandA[0] * operandB[1], operandA[1] * operandB[0], operandA[1] * operandB[1]);
            result[1] = maxf(operandA[0] * operandB[0], operandA[0] * operandB[1], operandA[1] * operandB[0], operandA[1] * operandB[1]);

            break;
        case DIV:
            result[0] = operandA[0] / operandB[1];
            result[1] = operandA[1] / operandB[0];
            break;
    }

    // result[0] = nextafterf(result[0], -INFINITY);
    result[1] = nextafterf(result[1], INFINITY);
}

void mallocMatrix(double ***matrix, int rows, int cols) {
    *matrix = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        (*matrix)[i] = (double *)malloc(cols * sizeof(double));
    }
}

void freeMatrix(double ***matrix, int size) {
    for (int i = 0; i < size; i++) {
        free((*matrix)[i]);
    }
    free(*matrix);
    *matrix = NULL;
}

void printSystem(double **a, double *b, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%lf ", a[i][j]);
        }
        printf("| %lf\n", b[i]);
    }
}

void perfSquare(double ***points, int *n, int *k) {
    double *powers, **coeffs, *terms;
    scanf("%d", n);
    scanf("%d", k);

    mallocMatrix(points, *n, 2);
    mallocMatrix(&coeffs, *k + 1, *k + 1);
    powers = (double *)malloc((*k + *k + 1) * sizeof(double));
    terms = (double *)malloc((*k + 1) * sizeof(double));

    for (int i = 0; i < *n; i++) {
        scanf("%lf", &(*points)[i][0]);
        scanf("%lf", &(*points)[i][1]);
    }

    for (int i = 0; i <= *k + *k; i++) {
        powers[i] = 0;
        for (int j = 0; j < *n; j++) {
            powers[i] += pow((*points)[j][0], i);
        }
    }

    for (int i = 0; i <= *k; i++) {
        terms[i] = 0;
        for (int j = 0; j < *n; j++) {
            terms[i] += (*points)[j][1] * pow((*points)[j][0], i);
        }
    }

    for (int i = 0; i <= *k; i++) {
        for (int j = 0; j <= *k; j++) {
            coeffs[i][j] = powers[i + j];
        }
    }

    printSystem(coeffs, terms, *k + 1);
}