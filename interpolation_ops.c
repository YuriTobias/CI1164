#include "interpolation_ops.h"

#include <stdio.h>
#include <stdlib.h>

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

void copySystem(double **srcA, double ***destA, double *srcB, double **destB, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            (*destA)[i][j] = srcA[i][j];
        }
        (*destB)[i] = srcB[i];
    }
}

double lagrangeInterpolation(double x, double **points, int n) {
    double res = 0, divisor, dividend = 1;

    for (int i = 0; i < n; i++) {
        dividend *= (x - points[i][0]);
    }

    for (int i = 0; i < n; i++) {
        divisor = 1;
        for (int j = 0; j < i; j++) {
            divisor *= (points[i][0] - points[j][0]);
        }
        divisor *= (x - points[i][0]);
        for (int j = i + 1; j < n; j++) {
            divisor *= (points[i][0] - points[j][0]);
        }
        res += points[i][1] * (dividend / divisor);
    }
    return res;
}

void initPoints(double ***A, int *n) {
    scanf("%d", n);
    mallocMatrix(A, *n, 2);
    for (int i = 0; i < *n; i++) {
        scanf("%lf %lf", &(*A)[i][0], &(*A)[i][1]);
    }
}