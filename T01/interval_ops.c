#include "interval_ops.h"

#include <fenv.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "system_ops.h"

void initInterval(char *number, Interval_t *interval) {
    fesetround(FE_DOWNWARD);
    interval->lower = atof(number);
    fesetround(FE_UPWARD);
    interval->upper = atof(number);
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

// void mallocIntervalMatrix(Interval_t ***matrix, int rows, int cols) {
//     *matrix = (Interval_t **)malloc(rows * sizeof(Interval_t *));
//     for (int i = 0; i < rows; i++) {
//         (*matrix)[i] = (Interval_t *)malloc(cols * sizeof(Interval_t));
//     }
// }

// void freeIntervalMatrix(Interval_t ***matrix, int size) {
//     for (int i = 0; i < size; i++) {
//         free((*matrix)[i]);
//     }
//     free(*matrix);
//     *matrix = NULL;
// }