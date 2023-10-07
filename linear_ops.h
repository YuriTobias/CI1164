#ifndef LINEAR_OPS_H
#define LINEAR_OPS_H

#include "polynomial_ops.h"

void copyMatrixInterval(Interval_t **A, Interval_t ***B, int n);

void copyVectorInterval(Interval_t *A, Interval_t **B, int n);

/**
 * Iterates through the lines above the pivot line to find the line with the biggest pivot
 * @param A pointer to the matrix A of coefficients
 * @param i pivot line (also the pivot column)
 * @param n size of the system
 * @return index of the line with the biggest pivot
 */
int findPivotLine(Interval_t **A, int i, int n);

/**
 * Swaps two lines of a linear system consisting of a matrix A with coefficients and a vector b with independent terms
 * @param A pointer to the matrix A of coefficients
 * @param b pointer to the vector b of independent terms
 * @param i index of the first line
 * @param iPivo index of the second line
 * @return void
 */
void swapSystemLines(Interval_t **A, Interval_t *b, int i, int iPivo);

/**
 * Performs Gaussian elimination with partial pivoting to put a linear system in triangular form
 * @param A pointer to the matrix A of coefficients
 * @param b pointer to the vector b of independent terms
 * @param n size of the system
 * @return void
 */
void gaussElimPivot(Interval_t **A, Interval_t *b, int n);

/**
 * Performs the back substitution method to solve a linear system and stores the solution in a vector x
 * @param A pointer to the matrix A of coefficients
 * @param b pointer to the vector b of independent terms
 * @param x pointer to the vector x of solutions
 * @param n size of the system
 * @return void
 */
void backSubstitution(Interval_t **A, Interval_t *b, Interval_t **x, int n);

/**
 * Calculates the residue for each equation according to the vector x of solutions
 * @param A pointer to the matrix A of coefficients
 * @param b pointer to the vector b of independent terms
 * @param x pointer to the vector x of solutions
 * @param r pointer to the vector r of residue
 * @param n size of the system
 *
 */
void calcResidue(Interval_t **A, Interval_t *b, Interval_t *x, Interval_t **r, int n);

/**
 * Prints the linear system consisting of a matrix A with coefficients and a vector b with independent terms
 * @param A pointer to the matrix A of coefficients
 * @param b pointer to the vector b of independent terms
 * @param n size of the system
 * @return void
 */
void printInputs(Interval_t **A, Interval_t *b, int n);

/**
 * Prints the results of the linear system from a vector x of solutions and the residue obtained for each equation from a vector r of
 * residue
 * @param x pointer to the vector x of solutions
 * @param r pointer to the vector r of residue
 * @param t average solution calculation time
 * @param n size of the system
 * @return void
 */
void printResults(double *x, double *r, double t, int n);

#endif