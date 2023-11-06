#ifndef INTERVAL_OPS_H
#define INTERVAL_OPS_H

enum OPERATIONS { SUM, SUB, MULT, DIV, POW };

typedef struct {
    double lower;
    double upper;
} Interval_t;

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

#endif  // INTERVAL_OPS_H