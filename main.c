#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

void calcIntervals(Float_t *numbers, Float_t **intervals) {
    for (int i = 0; i < QUANT; i++) {
        if (numbers[i].f == 0) {
            intervals[i][0].f = 0;
            intervals[i][1].f = 0;
        } else {
            intervals[i][0].i = numbers[i].i - 1;
            intervals[i][1].i = numbers[i].i + 1;
        }
    }
}

Float_t *calcIntervalSum(Float_t *operandA, Float_t *operandB) {
    Float_t *result = malloc(sizeof(Float_t) * 2);
    result[0].f = operandA[0].f + operandB[0].f;
    result[1].f = operandA[1].f + operandB[1].f;
    result[0].i = result[0].i-1;
    result[1].i = result[1].i+1;
    return result;
}

Float_t *calcIntervalSub(Float_t *operandA, Float_t *operandB) {
    Float_t *result = malloc(sizeof(Float_t) * 2);
    result[0].f = operandA[0].f - operandB[1].f;
    result[1].f = operandA[1].f - operandB[0].f;
    result[0].i = result[0].i-1;
    result[1].i = result[1].i+1;
    return result;
}

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

void printInterval(Float_t *interval) { printf("[%1.8e, %1.8e]", interval[0].f, interval[1].f); }

Float_t *calcIntervalMult(Float_t *operandA, Float_t *operandB) {
    Float_t *result = malloc(sizeof(Float_t) * 2);
    result[0].f =
        minf(operandA[0].f * operandB[0].f, operandA[0].f * operandB[1].f, operandA[1].f * operandB[0].f, operandA[1].f * operandB[1].f);
    result[1].f =
        maxf(operandA[0].f * operandB[0].f, operandA[0].f * operandB[1].f, operandA[1].f * operandB[0].f, operandA[1].f * operandB[1].f);
    
    result[0].i = result[0].i-1;
    result[1].i = result[1].i+1;
    
    return result;
}

Float_t *calcIntervalDiv(Float_t *operandA, Float_t *operandB) {
    Float_t *result = malloc(sizeof(Float_t) * 2);
    result[0].f = operandA[0].f / operandB[1].f;
    result[1].f = operandA[1].f / operandB[0].f;
    result[0].i = result[0].i-1;
    result[1].i = result[1].i+1;
    return result;
}

void handleInput(Float_t *numbers, char *operators) {
    for (int i = 0; i < QUANT; i++) {
        if (i == (QUANT - 1))
            scanf("%f", &numbers[QUANT - 1].f);
        else
            scanf("%f%s", &numbers[i].f, &operators[i]);
    }

    return;
}

void printOutput(Float_t *numbers, char *operators) {
    for (int i = 0; i < QUANT - 1; i++) printf("%.2f %c ", numbers[i].f, operators[i]);
    printf("%.2f\n", numbers[QUANT - 1].f);
}

void printIntervals(Float_t **intervals) {
    for (int i = 0; i < QUANT; i++) printInterval(intervals[i]);
}

void printResult(int i, Float_t *operandA, Float_t *operandB, Float_t *result, char operator) {
    printf("%d:\n", i);
    printInterval(operandA);
    printf(" %c ", operator);
    printInterval(operandB);
    printf(" =\n");
    printInterval(result);
    printf("\nEA: %1.8e; ER: %1.8e; ULPs: %d\n\n", fabs(result[1].f - result[0].f), fabs(result[1].f - result[0].f) / result[0].f,
           result[1].i-result[0].i-1);
}

Float_t *calcExpression(Float_t **intervals, char *operators) {
    Float_t *result = malloc(sizeof(Float_t) * 2);
    Float_t *prevResult = malloc(sizeof(Float_t) * 2);
    result[0].f = intervals[0][0].f;
    result[1].f = intervals[0][1].f;

    for (int i = 0; i < QUANT - 1; i++) {
        prevResult[0] = result[0];
        prevResult[1] = result[1];
        switch (operators[i]) {
            case '+':
                result = calcIntervalSum(result, intervals[i + 1]);
                break;
            case '-':
                result = calcIntervalSub(result, intervals[i + 1]);
                break;
            case '*':
                result = calcIntervalMult(result, intervals[i + 1]);
                break;
            case '/':
                if (intervals[i + 1][1].f == 0 || intervals[i + 1][0].f == 0) {
                    result[0].f = -INFINITY;
                    result[1].f = INFINITY;
                } else {
                    result = calcIntervalDiv(result, intervals[i + 1]);
                }
                break;
        }
        printResult(i + 1, prevResult, intervals[i + 1], result, operators[i]);
    }

    return result;
}

int main() {
    Float_t *inputNumbers = malloc(sizeof(Float_t) * QUANT);
    Float_t **inputIntervals = malloc(sizeof(Float_t *) * QUANT);
    for (int i = 0; i < QUANT; i++) inputIntervals[i] = malloc(sizeof(Float_t) * 2);
    char inputOperators[QUANT - 1];

    handleInput(inputNumbers, inputOperators);
    calcIntervals(inputNumbers, inputIntervals);
    calcExpression(inputIntervals, inputOperators);

    return 0;
}