#ifndef INTERVAL_OPS_H
#define INTERVAL_OPS_H

enum OPERATIONS { SUM, SUB, MULT, DIV, POW };

typedef struct {
    double lower;
    double upper;
} Interval_t;

typedef struct {
    Interval_t *x;
    Interval_t *y;
} Point_t;

/**
 * Processes a string with a number to obtain the smallest interval between two doubles that contain that value
 * @param number string with the number
 * @param interval pointer to the interval to be initialized
 * @return void
 */
void initInterval(char *number, Interval_t *interval);

/**
 * Calculate an interval arithmetic sum operation between two intervals
 * @param operandA first operand
 * @param operandB second operand
 * @param result pointer to the interval that will store the result
 * @return void
 */
void intervalSum(Interval_t *operandA, Interval_t *operandB, Interval_t *result);

/**
 * Calculate an interval arithmetic operation of subtraction between two intervals
 * @param operandA first operand
 * @param operandB second operand 
 * @param result pointer to the interval that will store the result
 * @return void
 */
void intervalSub(Interval_t *operandA, Interval_t *operandB, Interval_t *result);

/**
 * Calculate an interval arithmetic multiplication operation between two intervals
 * @param operandA first operand
 * @param operandB second operand
 * @param result pointer to the interval that will store the result
 * @return void
 */
void intervalMult(Interval_t *operandA, Interval_t *operandB, Interval_t *result);

/**
 * Calculate an interval arithmetic operation of division between two intervals
 * @param operandA first operand
 * @param operandB second operand 
 * @param result pointer to the interval that will store the result
 * @return void
 */
void intervalDiv(Interval_t *operandA, Interval_t *operandB, Interval_t *result);

/**
 * Calculate a power interval arithmetic operation between two intervals
 * @param operandA first operand
 * @param exp exponent for the power operation (can be ANY if the operation is not power)
 * @param result pointer to the interval that will store the result
 * @return void
 */
void intervalPow(Interval_t *operandA, int exp, Interval_t *result);

#endif  // INTERVAL_OPS_H