
#include "interval_operations.h"

float minf(float a, float b, float c, float d) {
    float min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    if (d < min) min = d;
    return min;
}

float maxf(float a, float b, float c, float d) {
    float max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    if (d > max) max = d;
    return max;
}

void calcInterval(Float_t number, Float_t *interval) {
    if (number.f == 0) {
        interval[0].f = 0;
        interval[1].f = 0;
    } else {
        interval[0].f = number.f;
        interval[1].f = nextafterf(number.f, INFINITY);
    }
}

void calcIntervalOperation(Float_t *operandA, Float_t *operandB, enum OPERATIONS operation, Float_t *result) {
    switch (operation) {
        case SUM:
            result[0].f = operandA[0].f + operandB[0].f;
            result[1].f = operandA[1].f + operandB[1].f;
            break;
        case SUB:
            result[0].f = operandA[0].f - operandB[1].f;
            result[1].f = operandA[1].f - operandB[0].f;
            break;
        case MULT:
            result[0].f = minf(operandA[0].f * operandB[0].f, operandA[0].f * operandB[1].f, operandA[1].f * operandB[0].f,
                               operandA[1].f * operandB[1].f);
            result[1].f = maxf(operandA[0].f * operandB[0].f, operandA[0].f * operandB[1].f, operandA[1].f * operandB[0].f,
                               operandA[1].f * operandB[1].f);

            break;
        case DIV:
            result[0].f = operandA[0].f / operandB[1].f;
            result[1].f = operandA[1].f / operandB[0].f;
            break;
    }

    // result[0].f = nextafterf(result[0].f, -INFINITY);
    result[1].f = nextafterf(result[1].f, INFINITY);
}

void printInterval(Float_t *interval) { printf("[%1.8e, %1.8e]", interval[0].f, interval[1].f); }

void handleInput(Float_t *numbers, char *operators) {
    for (int i = 0; i < QUANT; i++) {
        if (i == (QUANT - 1))
            scanf("%f", &numbers[QUANT - 1].f);
        else
            scanf("%f%s", &numbers[i].f, &operators[i]);
    }

    return;
}

void printResult(int i, Float_t *operandA, Float_t *operandB, Float_t *result, char operation) {
    printf("%d:\n", i);
    printInterval(operandA);
    printf(" %c ", operation);
    printInterval(operandB);
    printf(" =\n");
    printInterval(result);
    printf("\nEA: %1.8e; ER: %1.8e; ULPs: %d\n\n", fabs(result[1].f - result[0].f), fabs((result[1].f - result[0].f) / result[0].f),
           abs(result[1].i - result[0].i - 1));
}

void processExpression(Float_t *inputNumbers, char *operators) {
    Float_t *result = malloc(sizeof(Float_t) * 2);
    Float_t *operandA = malloc(sizeof(Float_t) * 2);
    Float_t *operandB = malloc(sizeof(Float_t) * 2);

    calcInterval(inputNumbers[0], operandA);

    for (int i = 0; i < QUANT - 1; i++) {
        calcInterval(inputNumbers[i + 1], operandB);

        switch (operators[i]) {
            case '+':
                calcIntervalOperation(operandA, operandB, SUM, result);
                break;
            case '-':
                calcIntervalOperation(operandA, operandB, SUB, result);
                break;
            case '*':
                calcIntervalOperation(operandA, operandB, MULT, result);
                break;
            case '/':
                if (operandB[1].f == 0 || operandB[0].f == 0) {
                    result[0].f = -INFINITY;
                    result[1].f = INFINITY;
                } else {
                    calcIntervalOperation(operandA, operandB, DIV, result);
                }
                break;
        }
        printResult(i + 1, operandA, operandB, result, operators[i]);
        memcpy(operandA, result, sizeof(Float_t) * 2);
    }
    free(operandA);
    free(operandB);
    free(result);
}