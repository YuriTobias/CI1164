#ifndef LINEAR_OPS_H
#define LINEAR_OPS_H

#include "polynomial_ops.h"

/**
 * Copies a matrix of intervals to another matrix of intervals
 * @param src pointer to the matrix to be copied
 * @param dest pointer to the matrix that will store the copy
 * @param rows number of rows of the matrix
 * @param cols number of columns of the matrix
 * @return void
 */
void copyMatrixInterval(Interval_t **src, Interval_t ***dest, int rows, int cols);

/**
 * Copies a vector of intervals to another vector of intervals
 * @param src pointer to the vector to be copied
 * @param dest pointer to the vector that will store the copy
 * @param n size of the vector
 * @return void
 */
void copyVectorInterval(Interval_t *src, Interval_t **dest, int size);

/**
 * Iterates through the lines above the pivot line to find the line with the biggest pivot
 * @param coeffs pointer to the matrix A of coefficients
 * @param i pivot line (also the pivot column)
 * @param size size of the system
 * @return index of the line with the biggest pivot
 */
int findPivotLine(Interval_t **coeffs, int i, int size);

/**
 * Swaps two lines of a linear system consisting of a matrix A with coefficients and a vector b with independent terms
 * @param coeffs pointer to the matrix A of coefficients
 * @param terms pointer to the vector b of independent terms
 * @param i index of the first line
 * @param iPivot index of the second line
 * @return void
 */
void swapSystemLines(Interval_t **coeffs, Interval_t *terms, int i, int iPivot);

/**
 * Performs Gaussian elimination with partial pivoting to put a linear system in triangular form
 * @param coeffs pointer to the matrix A of coefficients
 * @param terms pointer to the vector b of independent terms
 * @param size size of the system
 * @return void
 */
void gaussElimPivot(Interval_t **coeffs, Interval_t *terms, int size);

/**
 * Performs the back substitution method to solve a linear system and stores the solution in a vector x
 * @param coeffs pointer to the matrix A of coefficients
 * @param terms pointer to the vector b of independent terms
 * @param solution pointer to the vector x of solutions
 * @param size size of the system
 * @return void
 */
void backSubstitution(Interval_t **coeffs, Interval_t *terms, Interval_t **solution, int size);

/**
 * Calculates the residue for each equation according to the vector x of solutions
 * @param coeffs pointer to the matrix A of coefficients
 * @param terms pointer to the vector b of independent terms
 * @param solution pointer to the vector x of solutions
 * @param residue pointer to the vector r of residue
 * @param size size of the system
 *
 */
void calcResidue(Interval_t **coeffs, Interval_t *terms, Interval_t *solution, Interval_t **residue, int size);

/**
 * Prints the results of the program, including the solution of the system, the residues and the execution times
 * @param solution pointer to the vector x of solutions
 * @param residues pointer to the vector r of residues
 * @param size size of the vectors of solutions and residues, i.e., size of the system
 * @param leastSquaresTs execution time of the least squares method
 * @param systemSolutionTs execution time of the system solution method (Gaussian elimination with partial pivoting and back substitution)
 * @return void
 */
void printResults(Interval_t *solution, Interval_t *residues, int size, double leastSquaresTs, double systemSolutionTs);

#endif