#include "linear_ops.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void copyMatrixInterval(Interval_t **src, Interval_t ***dest, int rows, int cols) {
    mallocIntervalMatrix(dest, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            (*dest)[i][j] = src[i][j];
        }
    }
}

void copyVectorInterval(Interval_t *src, Interval_t **dest, int size) {
    *dest = (Interval_t *)malloc((size) * sizeof(Interval_t));
    for (int i = 0; i < size; i++) {
        (*dest)[i] = src[i];
    }
}

int findPivotLine(Interval_t **coeffs, int i, int size) {
    int bigger = i;
    for (int j = i + 1; j < size; j++) {
        if (((coeffs[j][i].upper + coeffs[j][i].lower) / 2) > ((coeffs[bigger][i].upper + coeffs[bigger][i].lower) / 2)) {
            bigger = j;
        }
    }
    return bigger;
}

void swapSystemLines(Interval_t **coeffs, Interval_t *terms, int i, int iPivot) {
    Interval_t *aux = coeffs[i];
    coeffs[i] = coeffs[iPivot];
    coeffs[iPivot] = aux;
    Interval_t auxB = terms[i];
    terms[i] = terms[iPivot];
    terms[iPivot] = auxB;
}

void gaussElimPivot(Interval_t **coeffs, Interval_t *terms, int size) {
    Interval_t m, result;

    for (int i = 0; i < size; i++) {
        int iPivot = findPivotLine(coeffs, i, size);
        if (iPivot != i) {
            swapSystemLines(coeffs, terms, i, iPivot);
        }
        for (int k = i + 1; k < size; k++) {
            intervalOperation(&(coeffs)[k][i], &(coeffs)[i][i], 0, DIV, &m);  // m = A[k][i] / A[i][i]
            initInterval("0", &(coeffs)[k][i]);                               // A[k][i] = 0
            for (int j = i + 1; j < size; j++) {
                intervalOperation(&m, &(coeffs)[i][j], 0, MULT, &result);              // result = m * A[i][j]
                intervalOperation(&(coeffs)[k][j], &result, 0, SUB, &(coeffs)[k][j]);  // A[k][j] -= result
            }
            intervalOperation(&m, &(terms)[i], 0, MULT, &result);          // result = m * b[i]
            intervalOperation(&(terms)[k], &result, 0, SUB, &(terms)[k]);  // b[k] -= result
        }
    }
}

void backSubstitution(Interval_t **coeffs, Interval_t *terms, Interval_t **solution, int size) {
    Interval_t result;

    *solution = (Interval_t *)malloc((size) * sizeof(Interval_t));
    for (int i = size - 1; i >= 0; i--) {
        (*solution)[i] = terms[i];
        for (int j = i + 1; j < size; j++) {
            intervalOperation(&(coeffs)[i][j], &(*solution)[j], 0, MULT, &result);
            intervalOperation(&(*solution)[i], &result, 0, SUB, &(*solution)[i]);
        }
        intervalOperation(&(*solution)[i], &(coeffs)[i][i], 0, DIV, &(*solution)[i]);
    }
}

void calcResidue(Interval_t **coeffs, Interval_t *terms, Interval_t *solution, Interval_t **residue, int size) {
    Interval_t result;

    *residue = (Interval_t *)malloc((size) * sizeof(Interval_t));
    for (int i = 0; i < size; i++) {
        (*residue)[i].lower = terms[i].lower * -1;
        (*residue)[i].upper = terms[i].upper * -1;
        for (int j = 0; j < size; j++) {
            intervalOperation(&(coeffs)[i][j], &(solution)[j], 0, MULT, &result);
            intervalOperation(&(*residue)[i], &result, 0, SUM, &(*residue)[i]);
        }
    }
}

void printResults(Interval_t *solution, Interval_t *residues, int size, double leastSquaresTs, double systemSolutionTs) {
    for (int i = 0; i < size; i++) {
        printf("[%.8e , %.8e] ", solution[i].lower, solution[i].upper);
    }
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("[%.8e , %.8e] ", residues[i].lower, residues[i].upper);
    }
    printf("\n");
    printf("%.8e\n", leastSquaresTs);
    printf("%.8e\n", systemSolutionTs);
}