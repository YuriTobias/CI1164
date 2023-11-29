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

inline void intervalSum(Interval_t *operandA, Interval_t *operandB, Interval_t *result) {
    Interval_t copyA, copyB;
    memcpy(&copyA, operandA, sizeof(Interval_t));
    memcpy(&copyB, operandB, sizeof(Interval_t)); 

    fesetround(FE_DOWNWARD);
    result->lower = copyA.lower + copyB.lower;
    fesetround(FE_UPWARD);
    result->upper = copyA.upper + copyB.upper;
}

inline void intervalSub(Interval_t *operandA, Interval_t *operandB, Interval_t *result) {
    Interval_t copyA, copyB;
    memcpy(&copyA, operandA, sizeof(Interval_t));
    memcpy(&copyB, operandB, sizeof(Interval_t)); 
    
    fesetround(FE_DOWNWARD);
    result->lower = copyA.lower - copyB.upper;
    fesetround(FE_UPWARD);
    result->upper = copyA.upper - copyB.lower;
}

inline void intervalMult(Interval_t *operandA, Interval_t *operandB, Interval_t *result) {
    Interval_t copyA, copyB;
    memcpy(&copyA, operandA, sizeof(Interval_t));
    memcpy(&copyB, operandB, sizeof(Interval_t)); 
    
    fesetround(FE_DOWNWARD);
    result->lower = minDouble4(copyA.lower * copyB.lower, copyA.lower * copyB.upper, copyA.upper * copyB.lower,
                               copyA.upper * copyB.upper);
    fesetround(FE_UPWARD);
    result->upper = maxDouble4(copyA.lower * copyB.lower, copyA.lower * copyB.upper, copyA.upper * copyB.lower,
                               copyA.upper * copyB.upper);
}

inline void intervalDiv(Interval_t *operandA, Interval_t *operandB, Interval_t *result) {
    Interval_t copyA, copyB;
    memcpy(&copyA, operandA, sizeof(Interval_t));
    memcpy(&copyB, operandB, sizeof(Interval_t)); 

    if (operandB->lower <= 0 && operandB->upper >= 0) {
        result->lower = -INFINITY;
        result->upper = INFINITY;
        return;
    }
    fesetround(FE_DOWNWARD);
    result->lower = minDouble4(copyA.lower / copyB.lower, copyA.lower / copyB.upper, copyA.upper / copyB.lower,
                                       copyA.upper / copyB.upper);
    fesetround(FE_UPWARD);
    result->upper = maxDouble4(copyA.lower / copyB.lower, copyA.lower / copyB.upper, copyA.upper / copyB.lower,
                                       copyA.upper / copyB.upper);
}

inline void intervalPow(Interval_t *operandA, int exp, Interval_t *result) {
    Interval_t copyA;
    memcpy(&copyA, operandA, sizeof(Interval_t));
     
    if (exp == 0) {
        result->lower = 1;
        result->upper = 1;
    } else if (exp % 2 != 0) {
        fesetround(FE_DOWNWARD);
        result->lower = pow(copyA.lower, exp);
        fesetround(FE_UPWARD);
        result->upper = pow(copyA.upper, exp);
    } else if (exp % 2 == 0 && copyA.lower >= 0) {
        fesetround(FE_DOWNWARD);
        result->lower = pow(copyA.lower, exp);
        fesetround(FE_UPWARD);
        result->upper = pow(copyA.upper, exp);
    } else if (exp % 2 == 0 && copyA.upper < 0) {
        fesetround(FE_DOWNWARD);
        result->lower = pow(copyA.upper, exp);
        fesetround(FE_UPWARD);
        result->upper = pow(copyA.lower, exp);
    } else {
        result->lower = 0;
        fesetround(FE_UPWARD);
        result->upper = maxDouble2(pow(copyA.lower, exp), pow(copyA.upper, exp));
    }
}