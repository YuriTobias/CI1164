#include <fenv.h>
#include <stdio.h>
#include <stdlib.h>

#include "interval_operations.h"

int main() {
    fesetround(FE_DOWNWARD);

    Float_t *inputNumbers = malloc(sizeof(Float_t) * QUANT);
    char *inputOperators = malloc(sizeof(char) * QUANT - 1);

    handleInput(inputNumbers, inputOperators);
    processExpression(inputNumbers, inputOperators);

    free(inputNumbers);
    free(inputOperators);

    return 0;
}