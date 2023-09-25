#include "interpolation_ops.h"

#include <math.h>
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

double newtonInterpolation(double x, double **points, int n) {
    double res = 0, mult = 1;
    double splitDifferences[n];

    calcSplitDifferences(splitDifferences, points, n);

    for (int i = 0; i < n; i++) {
        res += mult * splitDifferences[i];
        mult *= (x - points[i][0]);
    }

    return res;
}

void calcSplitDifferences(double *out, double **points, int n) {
    int cont;
    double splitDiffs[((n * n) + n) / 2];

    for (int i = 0; i < n; i++) splitDiffs[i] = points[i][1];

    cont = n;
    for (int i = 1; i < n; i++) {
        for (int j = i; j < n; j++) {
            splitDiffs[cont] = (splitDiffs[(cont - n + i)] - splitDiffs[cont - n + i - 1]) / (points[j][0] - points[j - i][0]);
            cont++;
        }
    }

    cont = 0;
    for (int i = 0; i < n; i++) {
        out[i] = splitDiffs[cont];
        cont += (n - i);
    }
}

void initPoints(double ***A, int *n, int x) {
    int min = INFINITY;
    int max = -INFINITY;
    if (scanf("%d", n) != 1) printf("Failed to read integer.\n");

    mallocMatrix(A, *n, 2);
    for (int i = 0; i < *n; i++) {
        if (scanf("%lf %lf", &(*A)[i][0], &(*A)[i][1]) != 2) printf("Failed to read matrix.\n");
        if ((*A)[i][0] < min) min = (*A)[i][0];
        if ((*A)[i][0] > max) max = (*A)[i][0];
    }
    if (x < min || x > max) {
        perror("xe is out of range.\n");
        exit(1);
    }
}