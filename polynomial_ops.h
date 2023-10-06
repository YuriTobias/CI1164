#ifndef POLYNOMIAL_OPS_H
#define POLYNOMIAL_OPS_H

enum OPERATIONS { SUM, SUB, MULT, DIV, POW };

typedef struct {
    double min;
    double max;
} Interval_t;

double minf(double a, double b, double c, double d);

double maxf4(double a, double b, double c, double d);

double maxf2(double a, double b);

void calcInterval(double number, Interval_t *interval);

void calcIntervalOperation(Interval_t *operandA, Interval_t *operandB, int exp, enum OPERATIONS operation, Interval_t *result);

void mallocIntervalMatrix(Interval_t ***matrix, int rows, int cols);

void freeIntervalMatrix(Interval_t ***matrix, int size);

void mallocMatrix(double ***matrix, int rows, int cols);

void freeMatrix(double ***matrix, int size);

void printSystem(Interval_t **a, Interval_t *b, int n);

void initInputs(Interval_t ***points, Interval_t **powers, Interval_t ***coeffs, Interval_t **terms, int *k, int *n);

void perfSquare(Interval_t **points, Interval_t *powers, Interval_t **coeffs, Interval_t *terms, int n, int k);

#endif  // POLYNOMIAL_OPS_H