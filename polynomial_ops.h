#ifndef POLYNOMIAL_OPS_H
#define POLYNOMIAL_OPS_H

enum OPERATIONS { SUM, SUB, MULT, DIV };

float minf(float a, float b, float c, float d);

float maxf(float a, float b, float c, float d);

void calcInterval(double number, double *interval);

void calcIntervalOperation(double *operandA, double *operandB, enum OPERATIONS operation, double *result);

void mallocMatrix(double ***matrix, int rows, int cols);

void freeMatrix(double ***matrix, int size);

void printSystem(double **a, double *b, int n);

void perfSquare(double ***points, int *n, int *k);

#endif // POLYNOMIAL_OPS_H