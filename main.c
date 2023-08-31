#include <stdio.h>
#include <stdlib.h>

// #include "likwid.h"

#define N 10000

void retrossubs(double **A, double *b, double *x, int n) {
    for (int i = n - 1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= A[i][j] * x[j];
        }
        x[i] /= A[i][i];
    }
}

void eliminacaoGauss(double **A, double *b, int n) {
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

void eliminacaoGaussSemPivoteamento(double **A, double *b, int n) {
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

int encontraMax(double **A, int i, int n) {
    int max = i;
    for (int j = i + 1; j < n; j++) {
        if (A[j][i] > A[max][i]) {
            max = j;
        }
    }
    return max;
}

void trocaLinha(double **A, double *b, int i, int iPivo) {
    double *aux = A[i];
    A[i] = A[iPivo];
    A[iPivo] = aux;
    double auxB = b[i];
    b[i] = b[iPivo];
    b[iPivo] = auxB;
}

void eliminacaoGaussPivoteamento(double **A, double *b, int n) {
    for (int i = 0; i < n; i++) {
        int iPivo = encontraMax(A, i, n);
        if (iPivo != i) {
            trocaLinha(A, b, i, iPivo);
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

void eliminacaoGaussSemMultiplicador(double **A, double *b, int n) {
    for (int i = 0; i < n; i++) {
        int iPivo = encontraMax(A, i, n);
        if (iPivo != i) {
            trocaLinha(A, b, i, iPivo);
        }

        double aii = A[i][i];
        for (int k = i + 1; k < n; k++) {
            double aki = A[k][i];
            for (int j = 0; j < n; j++) {
                A[k][j] = A[k][j] * aii - A[i][j] * aki;  // eqk = eqk*Aii - eqi*Aki
            }
            b[k] = b[k] * aii - b[i] * aki;  // bk = bk*Aii - bi*Aki
        }
    }
}

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

    double **A = NULL, *b = NULL, *x = NULL;
    int n;

    initializeArrays(&A, &b, &x, &n);
    printf("Step 1:\n");
    printInputs(A, b, n);
    eliminacaoGaussSemPivoteamento(A, b, n);
    printf("Step 2:\n");
    printInputs(A, b, n);
    retrossubs(A, b, x, n);
    printResults(x, n);

    return 0;
}
