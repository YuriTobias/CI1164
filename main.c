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
        intervals[i][0].i = numbers[i].i - 1;
        intervals[i][1].i = numbers[i].i + 1;
    }
}

Float_t *calcIntervalSum(Float_t *operandA, Float_t *operandB) {
    Float_t *result = malloc(sizeof(Float_t) * 2);
    result[0].f = operandA[0].f + operandB[0].f;
    result[1].f = operandA[1].f + operandB[1].f;
    return result;
}

Float_t *calcIntervalSub(Float_t *operandA, Float_t *operandB) {
    Float_t *result = malloc(sizeof(Float_t) * 2);
    result[0].f = operandA[0].f - operandB[1].f;
    result[1].f = operandA[1].f - operandB[0].f;
    return result;
}

Float_t *calcIntervalMult(Float_t *operandA, Float_t *operandB) {
    Float_t *result = malloc(sizeof(Float_t) * 2);
    result[0].f = operandA[0].f * operandB[0].f;
    result[1].f = operandA[1].f * operandB[1].f;
    return result;
}

Float_t *calcIntervalDiv(Float_t *operandA, Float_t *operandB) {
    Float_t *result = malloc(sizeof(Float_t) * 2);
    result[0].f = operandA[0].f / operandB[1].f;
    result[1].f = operandA[1].f / operandB[0].f;
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

// void printInterval(Float_t *interval) { printf("[%f, %f]\n", interval[0].f, interval[1].f); }

// void printIntervals(Float_t **intervals) {
//     for (int i = 0; i < QUANT; i++) printInterval(intervals[i]);
// }

int main() {
    Float_t *inputNumbers = malloc(sizeof(Float_t) * QUANT);
    Float_t **inputIntervals = malloc(sizeof(Float_t *) * QUANT);
    for (int i = 0; i < QUANT; i++) inputIntervals[i] = malloc(sizeof(Float_t) * 2);
    char inputOperators[QUANT - 1];

    handleInput(inputNumbers, inputOperators);
    calcIntervals(inputNumbers, inputIntervals);
    printOutput(inputNumbers, inputOperators);
    printIntervals(inputIntervals);

    return 0;
}