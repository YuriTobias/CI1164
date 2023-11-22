#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#define DIFF 0.0

#define NRAND ((double)rand() / RAND_MAX)  // drand48()
#define SRAND(a) srand(a)                  // srand48(a)

// Integral Monte Carlo da função Styblinski-Tang de 2 variáveis
double styblinskiTang2(double a, double b, long long int namostras) {
    double resultado;
    double soma = 0.0;

    printf("Metodo de Monte Carlo (x, y).\n");
    printf("a = (%f), b = (%f), n = (%lld), variaveis = 2\n", a, b, namostras);

    double t_inicial = timestamp();

    for (long long int i = 0; i < namostras; i++) {
        double x[2];
        for (int j = 0; j < 2; j++) {
            x[j] = ((double)rand() / RAND_MAX) * (b - a) + a;
        }
        soma += (x[0] * x[0] * x[0] * x[0] - 16 * x[0] * x[0] + 5 * x[0] + x[1] * x[1] * x[1] * x[1] - 16 * x[1] * x[1] + 5 * x[1]);
    }
    resultado = (b - a) * (b - a) * soma / (namostras * 2);

    double t_final = timestamp();
    printf("Tempo decorrido: %f seg.\n", t_final - t_inicial);

    return resultado;
}

// Integral Monte Carlo da função Styblinski-Tang de 4 variáveis
double styblinskiTang4(double a, double b, long long int namostras) {
    double resultado;
    double soma = 0.0;

    printf("Metodo de Monte Carlo (x, y).\n");
    printf("a = (%f), b = (%f), n = (%lld), variaveis = 4\n", a, b, namostras);

    double t_inicial = timestamp();

    for (long long int i = 0; i < namostras; i++) {
        double x[4];
        for (int j = 0; j < 4; j++) {
            x[j] = ((double)rand() / RAND_MAX) * (b - a) + a;
        }
        soma += (x[0] * x[0] * x[0] * x[0] - 16 * x[0] * x[0] + 5 * x[0] + x[1] * x[1] * x[1] * x[1] - 16 * x[1] * x[1] + 5 * x[1] +
                 x[2] * x[2] * x[2] * x[2] - 16 * x[2] * x[2] + 5 * x[2] + x[3] * x[3] * x[3] * x[3] - 16 * x[3] * x[3] + 5 * x[3]);
    }
    resultado = (b - a) * (b - a) * (b - a) * (b - a) * soma / (namostras * 2);

    double t_final = timestamp();
    printf("Tempo decorrido: %f seg.\n", t_final - t_inicial);

    return resultado;
}

// Integral Monte Carlo da função Styblinski-Tang de 8 variáveis
double styblinskiTang8(double a, double b, long long int namostras) {
    double resultado;
    double soma = 0.0;

    printf("Metodo de Monte Carlo (x, y).\n");
    printf("a = (%f), b = (%f), n = (%lld), variaveis = 8\n", a, b, namostras);

    double t_inicial = timestamp();

    for (long long int i = 0; i < namostras; i++) {
        double x[8];
        for (int j = 0; j < 8; j++) {
            x[j] = ((double)rand() / RAND_MAX) * (b - a) + a;
        }
        soma += (x[0] * x[0] * x[0] * x[0] - 16 * x[0] * x[0] + 5 * x[0] + x[1] * x[1] * x[1] * x[1] - 16 * x[1] * x[1] + 5 * x[1] +
                 x[2] * x[2] * x[2] * x[2] - 16 * x[2] * x[2] + 5 * x[2] + x[3] * x[3] * x[3] * x[3] - 16 * x[3] * x[3] + 5 * x[3] +
                 x[4] * x[4] * x[4] * x[4] - 16 * x[4] * x[4] + 5 * x[4] + x[5] * x[5] * x[5] * x[5] - 16 * x[5] * x[5] + 5 * x[5] +
                 x[6] * x[6] * x[6] * x[6] - 16 * x[6] * x[6] + 5 * x[6] + x[7] * x[7] * x[7] * x[7] - 16 * x[7] * x[7] + 5 * x[7]);
    }
    resultado = (b - a) * (b - a) * (b - a) * (b - a) * (b - a) * (b - a) * (b - a) * (b - a) * soma / (namostras * 2);

    double t_final = timestamp();
    printf("Tempo decorrido: %f seg.\n", t_final - t_inicial);

    return resultado;
}

double retangulos_xy(double a, double b, long long int npontos) {
    long long int dim = ceil(sqrt(npontos));
    double h = (b - a) / dim;
    double resultado;
    double soma = 0;
    double x = a, y;

    printf("Metodo dos Retangulos (x, y).\n");
    printf("a = (%f), b = (%f), n = (%lld), h = (%lg)\n", a, b, dim * dim, h);

    double t_inicial = timestamp();

    for (long long int i = 0; i <= dim; i++) {
        y = a;
        for (long long int j = 0; j <= dim; j++) {
            soma += (x * x * x * x - 16 * x * x + 5 * x + y * y * y * y - 16 * y * y + 5 * y);
            y += h;
        }
        x += h;
    }
    resultado = soma * h * h / 2;

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
    resultado = styblinskiTang2(atoi(argv[1]), atof(argv[2]), atoll(argv[3]));
    printf("Resultado: %f\n", resultado);
    resultado = styblinskiTang4(atoi(argv[1]), atof(argv[2]), atoll(argv[3]));
    printf("Resultado: %f\n", resultado);
    resultado = styblinskiTang8(atoi(argv[1]), atof(argv[2]), atoll(argv[3]));
    printf("Resultado: %f\n", resultado);
    resultado = retangulos_xy(atof(argv[1]), atof(argv[2]), atoll(argv[4]));
    printf("Resultado: %f\n", resultado);

    return 0;
}
