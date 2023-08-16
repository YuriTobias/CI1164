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

float minf(float a, float b, float c, float d);

float maxf(float a, float b, float c, float d);

void calcInterval(Float_t number, Float_t *interval);

void calcIntervalOperation(Float_t *operandA, Float_t *operandB, enum OPERATIONS operation, Float_t *result);

void printInterval(Float_t *interval);

void handleInput(Float_t *numbers, char *operators);

void printResult(int i, Float_t *operandA, Float_t *operandB, Float_t *result, char operation);

void processExpression(Float_t *inputNumbers, char *operators);

#endif