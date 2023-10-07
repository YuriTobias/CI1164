#ifndef POLYNOMIAL_OPS_H
#define POLYNOMIAL_OPS_H

enum OPERATIONS { SUM, SUB, MULT, DIV, POW };

typedef struct {
    double lower;
    double upper;
} Interval_t;

/**
 * Identifies the smallest of four double numbers
 * @param a first number
 * @param b second number
 * @param c third number
 * @param d fourth number
 * @return smallest number
 */
double minDouble4(double a, double b, double c, double d);

/**
 * Identifies the largest of four double numbers
 * @param a first number
 * @param b second number
 * @param c third number
 * @param d fourth number
 * @return largest number
 */
double maxDouble4(double a, double b, double c, double d);

/**
 * Identifies the largest of two double numbers
 * @param a first number
 * @param b second number
 * @return largest number
 */
double maxDouble2(double a, double b);

/**
 * Processes a string with a number to obtain the smallest interval between two doubles that contain that value
 * @param number string with the number
 * @param interval pointer to the interval to be initialized
 * @return void
 */
void initInterval(char *number, Interval_t *interval);

/**
 * Calculate an interval arithmetic operation between two intervals
 * @param operandA first operand
 * @param operandB second operand (can be ANY if the operation is unary)
 * @param exp exponent for the power operation (can be ANY if the operation is not power)
 * @param operation operation to be performed (SUM, SUB, MULT, DIV, POW)
 * @param result pointer to the interval that will store the result
 * @return void
 */
void intervalOperation(Interval_t *operandA, Interval_t *operandB, int exp, enum OPERATIONS operation, Interval_t *result);

/**
 * Allocates memory for a matrix of intervals
 * @param matrix pointer to the matrix
 * @param rows number of rows
 * @param cols number of columns
 * @return void
 */
void mallocIntervalMatrix(Interval_t ***matrix, int rows, int cols);

/**
 * Frees memory allocated for a matrix of intervals
 * @param matrix pointer to the matrix
 * @param size number of rows
 * @return void
 */
void freeIntervalMatrix(Interval_t ***matrix, int size);

/**
 * Prints a formatted interval linear system
 * @param coeffs pointer to the matrix A of coefficients
 * @param terms pointer to the vector terms of independent terms
 * @param size size of the system
 * @return void
 */
void printSystem(Interval_t **coeffs, Interval_t *terms, int size);

/**
 * Prints a vector of points
 * @param points pointer to the vector of points
 * @param size size of the vector
 * @return void
 */
void printPoints(Interval_t **points, int size);

/**
 * Allocates memory for all necessary data structures related to the curves adjustment problem and initializes them with user inputs if
 * necessary
 * @param points pointer to the matrix of points
 * @param powers pointer to the vector of powers
 * @param coeffs pointer to the matrix of coefficients
 * @param terms pointer to the vector of independent terms
 * @param nPoints pointer to the number of points
 * @param degree pointer to the degree of the polynomial to be adjusted
 * @return void
 */
void initData(Interval_t ***points, Interval_t **powers, Interval_t ***coeffs, Interval_t **terms, int *nPoints, int *degree);

/**
 * Generate a linear system of equations to adjust a polynomial to a set of points
 * @param points pointer to the matrix of points
 * @param powers pointer to the vector of powers
 * @param coeffs pointer to the matrix of coefficients
 * @param terms pointer to the vector of independent terms
 * @param degree degree of the polynomial to be adjusted
 * @param nPoints number of points
 * @return void
 */
void leastSquaresSystem(Interval_t **points, Interval_t *powers, Interval_t **coeffs, Interval_t *terms, int degree, int nPoints);

#endif  // POLYNOMIAL_OPS_H