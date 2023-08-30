#include <stdio.h>
#include <stdlib.h>

// #include "likwid.h"

#define N 10000

void initializeArrays(double ***A, double **b, double **x, int *n) {
    scanf("%d", n);

    *A = (double **)malloc((*n) * sizeof(double *));
    for (int i = 0; i < *n; i++) {
        (*A)[i] = (double *)malloc((*n) * sizeof(double));
    }
    *b = (double *)malloc((*n) * sizeof(double));
    *x = (double *)malloc((*n) * sizeof(double));

    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *n; j++) {
            scanf("%lf", &(*A)[i][j]);
        }
        scanf("%lf", &(*b)[i]);
    }
}

void printInputs(double **A, double *b, int n) {
    for (int i = 0; i < n; i++) {
        printf("| ");
        for (int j = 0; j < n; j++) {
            printf("%lf ", A[i][j]);
        }
        printf("| %lf\n", b[i]);
    }
}

int main(int argc, char *argv[]) {
    // LIKWID_MARKER_INIT;

    // LIKWID_MARKER_START("Teste_LIKWID");
    // //
    // LIKWID_MARKER_STOP("Teste_LIKWID");

    // LIKWID_MARKER_CLOSE;

    double **A = NULL, *b = NULL, *x = NULL;
    int n;

    initializeArrays(&A, &b, &x, &n);
    printInputs(A, b, n);

    return 0;
}
