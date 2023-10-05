#include "linear_ops.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Interval_t multInterval(Interval_t A, Interval_t B) {
    Interval_t aux;
    aux.min = A.min * B.min;
    if((A.min * B.max) < aux.min)
        aux.min = A.min * B.max;
    if((A.max * B.min) < aux.min)
        aux.min = A.max * B.min;
    if((A.max * B.max) < aux.min)
        aux.min = A.max * B.max;
    aux.max = A.min * B.min;
    if((A.min * B.max) > aux.max)
        aux.max = A.min * B.max;
    if((A.max * B.min) > aux.max)
        aux.max = A.max * B.min;
    if((A.max * B.max) > aux.max)
        aux.max = A.max * B.max;

    return aux;
}

Interval_t divInterval(Interval_t A, Interval_t B) {
    Interval_t aux;
    aux.min = A.min / B.max;    // a / d
    if((A.min / B.min) < aux.min)   // a / c
        aux.min = A.min / B.min;
    if((A.max / B.max) < aux.min)   // b / d
        aux.min = A.max / B.max;
    if((A.max / B.min) < aux.min)   // b / c
        aux.min = A.max / B.min;
    aux.max = A.min / B.max;    // a / d
    if((A.min / B.min) > aux.max)   // a / c
        aux.max = A.min / B.min;
    if((A.max / B.max) > aux.max)   // b / d
        aux.max = A.max / B.max;
    if((A.max / B.min) > aux.max)   // b / c
        aux.max = A.max / B.min;
    if((B.min < 0) & (B.max > 0)) {
        aux.min = -INFINITY;
        aux.max = INFINITY;
    }

    return aux;
}

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
        for (int k = i + 1; k < n; k++) {
            m = divInterval(A[k][i], A[i][i]);
            A[k][i].min = 0;
            A[k][i].max = 0;
            for (int j = i + 1; j < n; j++) {
                A[k][j].min -= multInterval(m, A[i][j]).max;
                A[k][j].max -= multInterval(m, A[i][j]).min;
            }
            b[k].min -= multInterval(m, b[i]).max;
            b[k].max -= multInterval(m, b[i]).min;
        }
    }
}

void backSubstitution(Interval_t **A, Interval_t *b, Interval_t **x, int n) {
    *x = (Interval_t *)malloc((n) * sizeof(Interval_t));
    for (int i = n - 1; i >= 0; i--) {
        (*x)[i] = b[i];
        for (int j = i + 1; j < n; j++) {
            (*x)[i].min -= multInterval(A[i][j], (*x)[j]).max;
            (*x)[i].max -= multInterval(A[i][j], (*x)[j]).min;
        }
        (*x)[i] = divInterval((*x)[i], A[i][i]);
    }
}

void calcResidue(Interval_t **A, Interval_t *b, Interval_t *x, Interval_t **r, int n) {
    *r = (Interval_t *)malloc((n) * sizeof(Interval_t));
    for (int i = 0; i < n; i++) {
        (*r)[i].min = b[i].min * -1;
        (*r)[i].max = b[i].max * -1;
        for (int j = 0; j < n; j++) {
            (*r)[i].min += multInterval(A[i][j], x[j]).min;
            (*r)[i].max += multInterval(A[i][j], x[j]).max;
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