#ifndef INTERVAL_OPERATIONS_H
#define INTERVAL_OPERATIONS_H

#include <fenv.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUANT 5

typedef union {
    int32_t i;
    float f;
    struct {                     // Bitfields for exploration (32 bits = sign|exponent|mantissa)
        uint32_t mantissa : 23;  // primeiros 23 bits (da direita para a esquerda)
        uint32_t exponent : 8;   // próximos 8 bits
        uint32_t sign : 1;       // proximo bit (ou seja, o bit mais a esquerda)
    } parts;
} Float_t;

enum OPERATIONS { SUM, SUB, MULT, DIV };

/**
 * Returns the minimum value among four floating-point numbers.
 *
 * @param a The first floating-point number to compare.
 * @param b The second floating-point number to compare.
 * @param c The third floating-point number to compare.
 * @param d The fourth floating-point number to compare.
 * @return The smallest value among the four input floating-point numbers.
 */
float minf(float a, float b, float c, float d);

/**
 * Returns the maximum value among four floating-point numbers.
 *
 * @param a The first floating-point number to compare.
 * @param b The second floating-point number to compare.
 * @param c The third floating-point number to compare.
 * @param d The fourth floating-point number to compare.
 * @return The largest value among the four input floating-point numbers.
 */
float maxf(float a, float b, float c, float d);

/**
 * Calculate a minimal interval that contains a floating-point number.
 * 
 * @param number The floating-point number to be contained in the interval.
 * @param interval The interval that will contain the floating-point number.
 * @return void
*/
void calcInterval(Float_t number, Float_t *interval);

/**
 * Calculate an arithmetic operation between two intervals, returning the result interval.
 * 
 * @param operandA The first interval operand.
 * @param operandB The second interval operand.
 * @param operation The arithmetic operation to be performed.
 * @param result The interval that will contain the result of the arithmetic operation.
 * @return void
*/
void calcIntervalOperation(Float_t *operandA, Float_t *operandB, enum OPERATIONS operation, Float_t *result);

/**
 * Print an interval.
 * 
 * @param interval The interval to be printed.
 * @return void
*/
void printInterval(Float_t *interval);

/**
 * Handle the input of the user, processing the operands and operators of the expression given.
 * 
 * @param numbers The array that will contain the operands of the expression.
 * @param operators The array that will contain the operators of the expression.
 * @return void
*/
void handleInput(Float_t *numbers, char *operators);

/**
 * Print the result of an arithmetic operation between two intervals.
 * 
 * @param i The index of the operation.
 * @param operandA The first interval operand.
 * @param operandB The second interval operand.
 * @param result The interval that will contain the result of the arithmetic operation.
 * @param operation The arithmetic operation to be performed.
 * @return void
*/
void printResult(int i, Float_t *operandA, Float_t *operandB, Float_t *result, char operation);

/**
 * Process the expression given by the user, calculating and printing the result of each arithmetic operation performed.
 * 
 * @param inputNumbers The array containing the operands of the expression.
 * @param operators The array containing the operators of the expression.
 * @return void
*/
void processExpression(Float_t *inputNumbers, char *operators);

#endif