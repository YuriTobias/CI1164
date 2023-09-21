#ifndef INTERPOLATION_OPS_H
#define INTERPOLATION_OPS_H

/**
 * Allocates memory for a matrix of size n x n
 * @param matrix pointer to the matrix
 * @param rows number of rows of the matrix
 * @param cols number of columns of the matrix
 * @return void
 */
void mallocMatrix(double ***matrix, int rows, int cols);

/**
 * Frees memory for a matrix of size n x n
 * @param matrix pointer to the matrix
 * @param size size of the matrix
 * @return void
*/
void freeMatrix(double ***matrix, int size);

/**
 * Copies the values of a linear system consisting of a matrix A with coefficients and a vector b with independent terms into another system
 * @param srcA pointer to the source matrix A of coefficients
 * @param destA pointer to the destination matrix A of coefficients
 * @param srcB pointer to the source vector b of independent terms
 * @param destB pointer to the destination vector b of independent terms
 * @param size size of the system
 * @return void
*/
void copySystem(double **srcA, double ***destA, double *srcB, double **destB, int size);

/**
 * Performs the back substitution method to solve a linear system and stores the solution in a vector x
 * @param A pointer to the matrix A of coefficients
 * @param b pointer to the vector b of independent terms
 * @param x pointer to the vector x of solutions
 * @param n size of the system
 * @return void
*/

/**
 * Calculate an interpolation using the Lagrange method
 * @param x value to be interpolated
 * @param points matrix of points to serve as interpolation base
 * @param n number of points in the matrix
 * @return the interpolated value
*/
double lagrangeInterpolation(double x, double **points, int n);

/**
 * Calculate an interpolation using the Newton method
 * @param x value to be interpolated
 * @param points matrix of points to serve as interpolation base
 * @param n number of points in the matrix
 * @return the interpolated value
*/
double newtonInterpolation(double x, double **points, int n);

/**
 * @param out vector with splitted differences
 * @param points matrix of points to serve as interpolation base
 * @param n number of points in the matrix
*/
void calcSplitDifferences(double *out, double **points, int n);

/**
 * Initialize the points to be used in the interpolation
 * @param A pointer to the matrix A where the points will be stored
 * @param n pointer to the variable that will store the number of points
 * @return void
*/
void initPoints(double ***A, int *n);

#endif