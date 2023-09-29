#ifndef POLYNOMIAL_OPS_H
#define POLYNOMIAL_OPS_H

enum OPERATIONS { SUM, SUB, MULT, DIV };

typedef struct {
    double min;
    double max;
} Interval_t;

float minf(float a, float b, float c, float d);

float maxf(float a, float b, float c, float d);

void calcInterval(double number, Interval_t *interval);

void calcIntervalOperation(Interval_t *operandA, Interval_t *operandB, enum OPERATIONS operation, Interval_t *result);

void mallocMatrix(double ***matrix, int rows, int cols);

void freeMatrix(double ***matrix, int size);

void printSystem(double **a, double *b, int n);

void perfSquare(double ***points, int *n, int *k);

#endif // POLYNOMIAL_OPS_H