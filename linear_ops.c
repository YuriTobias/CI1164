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
    Interval_t m, result;
    
    for (int i = 0; i < n; i++) {
        int iPivo = findPivotLine(A, i, n);
        if (iPivo != i) {
            swapSystemLines(A, b, i, iPivo);
        }
        for (int k = i + 1; k < n; k++) {
            calcIntervalOperation(&(A)[k][i], &(A)[i][i], 0, DIV, &result);
            m = result;
            calcInterval(0, &(A)[k][i]);
            for (int j = i + 1; j < n; j++) {
                calcIntervalOperation(&m, &(A)[i][j], 0, MULT, &result);
                calcIntervalOperation(&(A)[k][j], &result, 0, SUB, &(A)[k][j]);
            }
            calcIntervalOperation(&m, &(b)[i], 0, MULT, &result);
            calcIntervalOperation(&(b)[k], &result, 0, SUB, &(b)[k]);
        }
    }
}

void backSubstitution(Interval_t **A, Interval_t *b, Interval_t **x, int n) {
    Interval_t result;
    
    *x = (Interval_t *)malloc((n) * sizeof(Interval_t));
    for (int i = n - 1; i >= 0; i--) {
        (*x)[i] = b[i];
        for (int j = i + 1; j < n; j++) {
            calcIntervalOperation(&(A)[i][j], &(*x)[j], 0, MULT, &result);
            calcIntervalOperation(&(*x)[i], &result, 0, SUB, &(*x)[i]);
        }
        calcIntervalOperation(&(*x)[i], &(A)[i][i], 0, DIV, &(*x)[i]);
    }
}

void calcResidue(Interval_t **A, Interval_t *b, Interval_t *x, Interval_t **r, int n) {
    Interval_t result;
    
    *r = (Interval_t *)malloc((n) * sizeof(Interval_t));
    for (int i = 0; i < n; i++) {
        (*r)[i].min = b[i].min * -1;
        (*r)[i].max = b[i].max * -1;
        for (int j = 0; j < n; j++) {
            calcIntervalOperation(&(A)[i][j], &(x)[j], 0, MULT, &result);
            calcIntervalOperation(&(*r)[i], &result, 0, SUM, &(*r)[i]);
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