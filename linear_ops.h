#ifndef LINEAR_OPS_H
#define LINEAR_OPS_H

/**
 * Allocates memory for a matrix of size n x n
 * @param matrix pointer to the matrix
 * @param size size of the matrix
 * @return void
 */
void mallocMatrix(double ***matrix, int size);

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
void backSubstitution(double **A, double *b, double *x, int n);

/**
 * Performs Gaussian elimination to put a linear system in triangular form
 * @param A pointer to the matrix A of coefficients
 * @param b pointer to the vector b of independent terms
 * @param n size of the system
 * @return void
*/
void gaussElim(double **A, double *b, int n);

/**
 * Performs an alternative Gaussian elimination to put a linear system in triangular form, making the pivot to be 1 in each iteration
 * @param A pointer to the matrix A of coefficients
 * @param b pointer to the vector b of independent terms
 * @param n size of the system
 * @return void
*/
void gaussElimAlt(double **A, double *b, int n);

/**
 * Iterates through the lines above the pivot line to find the line with the biggest pivot
 * @param A pointer to the matrix A of coefficients
 * @param i pivot line (also the pivot column)
 * @param n size of the system
 * @return index of the line with the biggest pivot
*/
int findPivotLine(double **A, int i, int n);

/**
 * Swaps two lines of a linear system consisting of a matrix A with coefficients and a vector b with independent terms
 * @param A pointer to the matrix A of coefficients
 * @param b pointer to the vector b of independent terms
 * @param i index of the first line
 * @param iPivo index of the second line
 * @return void
*/
void swapSystemLines(double **A, double *b, int i, int iPivo);

/**
 * Performs Gaussian elimination with partial pivoting to put a linear system in triangular form
 * @param A pointer to the matrix A of coefficients
 * @param b pointer to the vector b of independent terms
 * @param n size of the system
 * @return void
*/
void gaussElimPivot(double **A, double *b, int n);

/**
 * Performs Gaussian elimination with partial pivoting without using a multiplier to put a linear system in triangular form
 * @param A pointer to the matrix A of coefficients
 * @param b pointer to the vector b of independent terms
 * @param n size of the system
 * @return void
*/
void gaussElimPivotNoMult(double **A, double *b, int n);

/**
 * Calculates the residue for each equation according to the vector x of solutions
 * @param A pointer to the matrix A of coefficients
 * @param b pointer to the vector b of independent terms
 * @param x pointer to the vector x of solutions
 * @param r pointer to the vector r of residue
 * @param n size of the system
 * 
*/
void calcResidue(double **A, double *b, double *x, double *r, int n);

/**
 * Initializes a linear system consisting of a matrix A with coefficients and a vector b with independent terms (and a copy of each)
 * and a vector x of solutions, allocating memory for them and filling them with user input (except for the vector x)
 * @param A pointer to the matrix A of coefficients
 * @param Acpy pointer to the matrix A of coefficients (copy)
 * @param b pointer to the vector b of independent terms
 * @param bcpy pointer to the vector b of independent terms (copy)
 * @param x pointer to the vector x of solutions
 * @param r pointer to the vector r of residue
 * @param n size of the system
 * @return number of sucessful values read
*/
int initSystem(double ***A, double ***Acpy, double **b, double **bcpy, double **x, double **r, int *n);

/**
 * Prints the linear system consisting of a matrix A with coefficients and a vector b with independent terms
 * @param A pointer to the matrix A of coefficients
 * @param b pointer to the vector b of independent terms
 * @param n size of the system
 * @return void
*/
void printInputs(double **A, double *b, int n);

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