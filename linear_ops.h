#ifndef LINEAR_OPS_H
#define LINEAR_OPS_H

void mallocMatrix(double ***matrix, int size);

void freeMatrix(double ***matrix, int size);

void copySystem(double **srcA, double ***destA, double *srcB, double **destB, int size);

void backSubstitution(double **A, double *b, double *x, int n);

void gaussElim(double **A, double *b, int n);

void gaussElimAlt(double **A, double *b, int n);

int findPivotLine(double **A, int i, int n);

void swapSystemLines(double **A, double *b, int i, int iPivo);

void gaussElimPivot(double **A, double *b, int n);

void gaussElimPivotNoMult(double **A, double *b, int n);

void initSystem(double ***A, double ***Acpy, double **b, double **bcpy, double **x, int *n);

void printInputs(double **A, double *b, int n);

void printResults(double *x, int n);

#endif