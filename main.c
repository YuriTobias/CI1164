#include <stdio.h>
#include <stdlib.h>

// #include "likwid.h"

#define N 10000

void mallocMatrix(double ***matrix, int size) {
    *matrix = (double **)malloc(size * sizeof(double *));
    for (int i = 0; i < size; i++) {
        (*matrix)[i] = (double *)malloc(size * sizeof(double));
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

void backSubstitution(double **A, double *b, double *x, int n) {
    for (int i = n - 1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= A[i][j] * x[j];
        }
        x[i] /= A[i][i];
    }
}

void gaussElim(double **A, double *b, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = i + 1; k < n; k++) {
            double m = A[k][i] / A[i][i];
            A[k][i] = 0;
            for (int j = i + 1; j < n; j++) {
                A[k][j] -= m * A[i][j];
            }
            b[k] -= m * b[i];
        }
    }
}

void gaussElimAlt(double **A, double *b, int n) {
    for (int i = 0; i < n; i++) {
        double aii = A[i][i];
        for (int j = 0; j < n; j++) {
            A[i][j] /= aii;
        }
        b[i] /= aii;

        for (int k = i + 1; k < n; k++) {
            for (int j = i + 1; j < n; j++) {
                A[k][j] -= A[k][i] * A[i][j];
            }
            b[k] -= A[k][i] * b[i];
            A[k][i] = 0;
        }
    }
}

int findPivotLine(double **A, int i, int n) {
    int max = i;
    for (int j = i + 1; j < n; j++) {
        if (A[j][i] > A[max][i]) {
            max = j;
        }
    }
    return max;
}

void swapSystemLines(double **A, double *b, int i, int iPivo) {
    double *aux = A[i];
    A[i] = A[iPivo];
    A[iPivo] = aux;
    double auxB = b[i];
    b[i] = b[iPivo];
    b[iPivo] = auxB;
}

void gaussElimPivot(double **A, double *b, int n) {
    for (int i = 0; i < n; i++) {
        int iPivo = findPivotLine(A, i, n);
        if (iPivo != i) {
            swapSystemLines(A, b, i, iPivo);
        }
        for (int k = i + 1; k < n; k++) {
            double m = A[k][i] / A[i][i];
            A[k][i] = 0;
            for (int j = i + 1; j < n; j++) {
                A[k][j] -= m * A[i][j];
            }
            b[k] -= m * b[i];
        }
    }
}

void gaussElimPivotNoMult(double **A, double *b, int n) {
    for (int i = 0; i < n; i++) {
        int iPivo = findPivotLine(A, i, n);
        if (iPivo != i) {
            swapSystemLines(A, b, i, iPivo);
        }

        double aii = A[i][i];
        for (int k = i + 1; k < n; k++) {
            double aki = A[k][i];
            for (int j = 0; j < n; j++) {
                A[k][j] = A[k][j] * aii - A[i][j] * aki;
            }
            b[k] = b[k] * aii - b[i] * aki;
        }
    }
}

void initSystem(double ***A, double ***Acpy, double **b, double **bcpy, double **x, int *n) {
    scanf("%d", n);

    mallocMatrix(A, *n);
    mallocMatrix(Acpy, *n);
    *b = (double *)malloc((*n) * sizeof(double));
    *bcpy = (double *)malloc((*n) * sizeof(double));
    *x = (double *)malloc((*n) * sizeof(double));

    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *n; j++) {
            scanf("%lf", &(*A)[i][j]);
        }
        scanf("%lf", &(*b)[i]);
    }

    copySystem(*A, Acpy, *b, bcpy, *n);
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

void printResults(double *x, int n) {
    printf("X=[");
    for (int i = 0; i < n; i++) {
        printf("%lf%c", x[i], i == n - 1 ? ']' : ' ');
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    // LIKWID_MARKER_INIT;

    // LIKWID_MARKER_START("Teste_LIKWID");
    // //
    // LIKWID_MARKER_STOP("Teste_LIKWID");

    // LIKWID_MARKER_CLOSE;

    double **A = NULL, **Acpy = NULL, *b = NULL, *bcpy = NULL, *x = NULL;
    int n;

    initSystem(&A, &Acpy, &b, &bcpy, &x, &n);

    gaussElimAlt(A, b, n);
    backSubstitution(A, b, x, n);
    printResults(x, n);

    copySystem(Acpy, &A, bcpy, &b, n);
    gaussElimPivot(A, b, n);
    backSubstitution(A, b, x, n);
    printResults(x, n);

    copySystem(Acpy, &A, bcpy, &b, n);
    gaussElimPivotNoMult(A, b, n);
    backSubstitution(A, b, x, n);
    printResults(x, n);

    freeMatrix(&A, n);
    freeMatrix(&Acpy, n);
    free(b);
    free(x);

    return 0;
}
/*
 * Cópia das matrizes
 * Resíduos
 * Script likwid
 */