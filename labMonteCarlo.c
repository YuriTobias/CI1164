#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#define DIFF 0.0

#define NRAND ((double)rand() / RAND_MAX)  // drand48()
#define SRAND(a) srand(a)                  // srand48(a)

// Integral Monte Carlo da função Styblinski-Tang de 2 variáveis
double styblinskiTang(double a, double b, int namostras) {
    double resultado;
    double soma = 0.0;

    printf("Metodo de Monte Carlo (x, y).\n");
    printf("a = (%f), b = (%f), n = (%d), variaveis = 2\n", a, b, namostras);

    double t_inicial = timestamp();

    for (int i = 0; i < namostras; i++) {
        double x[2];
        for (int j = 0; j < 2; j++) {
            x[j] = ((double)rand() / RAND_MAX) * (b - a) + a;
        }
        soma += (x[0] * x[0] * x[0] * x[0] - 16 * x[0] * x[0] + 5 * x[0] + x[1] * x[1] * x[1] * x[1] - 16 * x[1] * x[1] + 5 * x[1]) / 2;
    }
    resultado = (b - a) * (b - a) * soma / namostras;

    double t_final = timestamp();
    printf("Tempo decorrido: %f seg.\n", t_final - t_inicial);

    return resultado;
}

double retangulos_xy(double a, double b, int npontos) {
    double h = (b - a) / npontos;
    double resultado;
    double soma = 0;

    printf("Metodo dos Retangulos (x, y).\n");
    printf("a = (%f), b = (%f), n = (%d), h = (%lg)\n", a, b, npontos, h);

    double t_inicial = timestamp();

    for (double i = 0; i <= npontos; i++) {
        for (double j = 0; j <= npontos; j++) {
            double x = a + i * h;
            double y = a + j * h;
            soma += h * h * (x * x * x * x - 16 * x * x + 5 * x + y * y * y * y - 16 * y * y + 5 * y) / 2;
        }
    }
    resultado = soma;

    double t_final = timestamp();
    printf("Tempo decorrido: %f seg.\n", t_final - t_inicial);

    return resultado;
}

int main(int argc, char **argv) {
    if (argc < 5) {
        printf("Utilização: %s inicial final n_amostras n_pontos\n", argv[0]);
        return 1;
    }

    srand(20232);

    double resultado;
    resultado = styblinskiTang(atof(argv[1]), atof(argv[2]), atoi(argv[3]));
    printf("Resultado: %f\n", resultado);
    resultado = retangulos_xy(atof(argv[1]), atof(argv[2]), atoi(argv[4]));
    printf("Resultado: %f\n", resultado);

    return 0;
}
