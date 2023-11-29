#include "interval_ops.h"

#include <fenv.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "system_ops.h"

inline void initInterval(char *number, Interval_t *interval) {
    fesetround(FE_DOWNWARD);
    interval->lower = atof(number);
    fesetround(FE_UPWARD);
    interval->upper = atof(number);
}

inline void intervalMult(Interval_t *operandA, Interval_t *operandB, Interval_t *result) {
    fesetround(FE_DOWNWARD);
    result->lower = minDouble4(operandA->lower * operandB->lower, operandA->lower * operandB->upper, operandA->upper * operandB->lower,
                               operandA->upper * operandB->upper);
    fesetround(FE_UPWARD);
    result->upper = maxDouble4(operandA->lower * operandB->lower, operandA->lower * operandB->upper, operandA->upper * operandB->lower,
                               operandA->upper * operandB->upper);
}

inline void intervalSum(Interval_t *operandA, Interval_t *operandB, Interval_t *result) {
    fesetround(FE_DOWNWARD);
    result->lower = operandA->lower + operandB->lower;
    fesetround(FE_UPWARD);
    result->upper = operandA->upper + operandB->upper;
}

void intervalOperation(Interval_t *operandA, Interval_t *operandB, int exp, enum OPERATIONS operation, Interval_t *result) {
    Interval_t *copyA = (Interval_t *)malloc(sizeof(Interval_t));
    Interval_t *copyB = (Interval_t *)malloc(sizeof(Interval_t));
    memcpy(copyA, operandA, sizeof(Interval_t));
    if (operandB != NULL) memcpy(copyB, operandB, sizeof(Interval_t));

    switch (operation) {
        case SUM:
            fesetround(FE_DOWNWARD);
            result->lower = copyA->lower + copyB->lower;
            fesetround(FE_UPWARD);
            result->upper = copyA->upper + copyB->upper;
            break;
        case SUB:
            fesetround(FE_DOWNWARD);
            result->lower = copyA->lower - copyB->upper;
            fesetround(FE_UPWARD);
            result->upper = copyA->upper - copyB->lower;
            break;
        case MULT:
            fesetround(FE_DOWNWARD);
            result->lower = minDouble4(copyA->lower * copyB->lower, copyA->lower * copyB->upper, copyA->upper * copyB->lower,
                                       copyA->upper * copyB->upper);
            fesetround(FE_UPWARD);
            result->upper = maxDouble4(copyA->lower * copyB->lower, copyA->lower * copyB->upper, copyA->upper * copyB->lower,
                                       copyA->upper * copyB->upper);
            break;
        case DIV:
            if (copyB->lower <= 0 && copyB->upper >= 0) {
                result->lower = -INFINITY;
                result->upper = INFINITY;
                break;
            }
            fesetround(FE_DOWNWARD);
            result->lower = minDouble4(copyA->lower / copyB->lower, copyA->lower / copyB->upper, copyA->upper / copyB->lower,
                                       copyA->upper / copyB->upper);
            fesetround(FE_UPWARD);
            result->upper = maxDouble4(copyA->lower / copyB->lower, copyA->lower / copyB->upper, copyA->upper / copyB->lower,
                                       copyA->upper / copyB->upper);
            break;
        case POW:
            if (exp == 0) {
                result->lower = 1;
                result->upper = 1;
            } else if (exp % 2 != 0) {
                fesetround(FE_DOWNWARD);
                result->lower = pow(copyA->lower, exp);
                fesetround(FE_UPWARD);
                result->upper = pow(copyA->upper, exp);
            } else if (exp % 2 == 0 && copyA->lower >= 0) {
                fesetround(FE_DOWNWARD);
                result->lower = pow(copyA->lower, exp);
                fesetround(FE_UPWARD);
                result->upper = pow(copyA->upper, exp);
            } else if (exp % 2 == 0 && copyA->upper < 0) {
                fesetround(FE_DOWNWARD);
                result->lower = pow(copyA->upper, exp);
                fesetround(FE_UPWARD);
                result->upper = pow(copyA->lower, exp);
            } else {
                result->lower = 0;
                fesetround(FE_UPWARD);
                result->upper = maxDouble2(pow(copyA->lower, exp), pow(copyA->upper, exp));
            }
            break;
    }

    free(copyA);
    free(copyB);
}