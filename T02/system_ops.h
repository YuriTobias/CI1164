#ifndef SYSTEM_OPS_H
#define SYSTEM_OPS_H

#include "interval_ops.h"

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
 * Prints a formatted interval linear system
 * @param coeffs pointer to the matrix A of coefficients
 * @param terms pointer to the vector terms of independent terms
 * @param size size of the system
 * @return void
 */
void printSystem(Interval_t *coeffs, Interval_t *terms, int size);

/**
 * Prints a vector of points
 * @param points pointer to the vector of points
 * @param size size of the vector
 * @return void
 */
void printPoints(Interval_t *points, int size);

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
void initData(Point_t *points, Interval_t **powers, Interval_t **coeffs, Interval_t **terms, int *nPoints, int *degree);

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
void leastSquaresSystem(Point_t points, Interval_t *restrict powers, Interval_t *restrict coeffs, Interval_t *restrict terms,
                        int k, int degree);

/**
 * Iterates through the lines above the pivot line to find the line with the biggest pivot
 * @param coeffs pointer to the matrix A of coefficients
 * @param i pivot line (also the pivot column)
 * @param size size of the system
 * @return index of the line with the biggest pivot
 */
int findPivotLine(Interval_t *coeffs, int i, int size);

/**
 * Swaps two lines of a linear system consisting of a matrix A with coefficients and a vector b with independent terms
 * @param coeffs pointer to the matrix A of coefficients
 * @param terms pointer to the vector b of independent terms
 * @param i index of the first line
 * @param iPivot index of the second line
 * @param size size of the system
 * @return void
 */
void swapSystemLines(Interval_t *coeffs, Interval_t *terms, int i, int iPivot, int size);

/**
 * Performs Gaussian elimination with partial pivoting to put a linear system in triangular form
 * @param coeffs pointer to the matrix A of coefficients
 * @param terms pointer to the vector b of independent terms
 * @param size size of the system
 * @return void
 */
void gaussElimPivot(Interval_t *restrict coeffs, Interval_t *restrict terms, int size);

/**
 * Performs the back substitution method to solve a linear system and stores the solution in a vector x
 * @param coeffs pointer to the matrix A of coefficients
 * @param terms pointer to the vector b of independent terms
 * @param solution pointer to the vector x of solutions
 * @param size size of the system
 * @return void
 */
void backSubstitution(Interval_t *coeffs, Interval_t *terms, Interval_t **solution, int size);

/**
 * Calculates the residual for each equation according to the vector x of solutions
 * @param points pointer to the matrix of points
 * @param solution pointer to the vector of solutions
 * @param residuals pointer to the vector of residuals (to be initialized)
 * @param degree degree of the polynomial
 * @param npoints number of points
 */
void calcResidual(Point_t points, Interval_t *restrict solution, Interval_t **restrict residuals, int degree, int npoints);

/**
 * Prints the results of the program, including the solution of the system, the residuals and the execution times
 * @param solution pointer to the vector solutions
 * @param residuals pointer to the vector residuals
 * @param npoints size of the vector of residuals, i.e., number of points
 * @param degree degree of the polynomial
 * @param leastSquaresTs execution time of the least squares method
 * @param systemSolutionTs execution time of the system solution method (Gaussian elimination with partial pivoting and back substitution)
 * @return void
 */
void printResults(Interval_t *solution, Interval_t *residuals, int npoints, int degree, double leastSquaresTs, double systemSolutionTs);

#endif  // SYSTEM_OPS_H