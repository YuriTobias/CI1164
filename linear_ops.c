#include "linear_ops.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void copyMatrixInterval(Interval_t **A, Interval_t ***B, int n) {
    mallocIntervalMatrix(B, n, n);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            (*B)[i][j] = A[i][j];
        }
    }
}

void copyVectorInterval(Interval_t *A, Interval_t **B, int n) {
    *B = (Interval_t *)malloc((n) * sizeof(Interval_t));
    for(int i = 0; i < n; i++) {
        (*B)[i] = A[i];
    }
}

int findPivotLine(Interval_t **A, int i, int n) {
    int bigger = i;
    for (int j = i + 1; j < n; j++) {
        if (((A[j][i].max + A[j][i].min) / 2) > ((A[bigger][i].max + A[bigger][i].min) / 2)) {
            bigger = j;
        }
    }
    return bigger;
}

void swapSystemLines(Interval_t **A, Interval_t *b, int i, int iPivo) {
    Interval_t *aux = A[i];
    A[i] = A[iPivo];
    A[iPivo] = aux;
    Interval_t auxB = b[i];
    b[i] = b[iPivo];
    b[iPivo] = auxB;
}

void gaussElimPivot(Interval_t **A, Interval_t *b, int n) {
    Interval_t m;
    
    for (int i = 0; i < n; i++) {
        int iPivo = findPivotLine(A, i, n);
        if (iPivo != i) {
            swapSystemLines(A, b, i, iPivo);
        }
        // for (int k = i + 1; k < n; k++) {
        //     m.min = A[k][i].min / A[i][i].max;
        //     m.max = A[k][i].max / A[i][i].min;
        //     if(A[i][i].min < 0 && A[i][i].max > 0) {
        //         m.min = -INFINITY;
        //         m.max = INFINITY;
        //     }
        //     A[k][i].min = 0;
        //     A[k][i].max = 0;
        //     for (int j = i + 1; j < n; j++) {
        //         A[k][j] -= m * A[i][j];
        //     }
        //     b[k] -= m * *b[i];
        // }
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

void calcResidue(double **A, double *b, double *x, double *r, int n) {
    for (int i = 0; i < n; i++) {
        r[i] = b[i] * -1;
        for (int j = 0; j < n; j++) {
            r[i] += A[i][j] * x[j];
        }
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

void printResults(double *x, double *r, double t, int n) {
    printf("X=[");
    for (int i = 0; i < n; i++) {
        printf("%lf%c", x[i], i == n - 1 ? ']' : ' ');
    }
    printf("\n");
    printf("R=[");
    for (int i = 0; i < n; i++) {
        printf("%lf%c", r[i], i == n - 1 ? ']' : ' ');
    }
    printf("\nAVERAGE TIME: %lf ms\n", t);
}