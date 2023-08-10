#include <stdio.h>
#define QUANT 5

void handleInput(float *numbers, char *operators) {
    for(int i = 0; i < QUANT; i++) {
        if(i == (QUANT - 1))
            scanf("%f", &numbers[QUANT - 1]);
        else 
            scanf("%f%s", &numbers[i], &operators[i]);
    }

    return;
}

void printOutput(float *numbers, char *operators) {
    for(int i = 0; i < QUANT-1; i++)
        printf("%.2f %c ", numbers[i], operators[i]);
    printf("%.2f\n", numbers[QUANT-1]);
}

int main() {
    float inputNumbers[QUANT];
    char inputOperators[QUANT-1];

    handleInput(inputNumbers, inputOperators);
    printOutput(inputNumbers, inputOperators);

    return 0;
}