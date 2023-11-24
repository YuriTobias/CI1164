#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
    A ideia principal foi reduzir o número de operações, para tal foram adicionadas mais variáveis que guardam valores que são
    calculados mais de uma vez. Além disso, chamadas de funções também geram gargalos na execução, para resolver tal questão
    foram substituidas as chamadas de pow() por multiplicações "diretas"
*/ 

double calc_integral_mc(int n, double a, double b) { 
    double sum, x, y, x2SumY2, interval;
    double bLessA = b - a;

    interval = (double) 1.0 / (RAND_MAX * (bLessA));
    for(int i=0; i < n; i++) {
        x = a + (double) rand() * interval;
        y = a + (double) rand() * interval;
        x = x * x;  // x^2
        y = y * y;  // y^2
        x2SumY2 = x + y;    // (x^2 + y^2)^2
        sum += 1e5 * x + y - x2SumY2 + 1e-5 * x2SumY2 * x2SumY2;
    }
    
    return (bLessA)*(bLessA) * sum / n;
}

int main() {
    printf("%.2lf\n", calc_integral_mc(100, 10, 20));

    return 0;
}