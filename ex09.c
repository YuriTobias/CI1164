#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 3
#define M_PI 3.14

int main() {
    double a[N][N], x[N], y[N], b[N], z[N];
    double cosR[8];
    double div = M_PI / 7.0;

    // Normal
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            a[j][i] = x[i] + y[j] * cos((i%8) * M_PI/7.0);
    for(int i = 0; i < N; i++)
        b[i] = 1.0/x[i] + z[i];

    /* Optimized
     * a) A versão a seguir deve ser considerada mais eficiente por reaproveitar melhor o acesso aos dados, vide princípio da localidade e
     * conforme exemplificado no segundo laço a seguir. Ademais, essa versão também faz menos operações de ponto flutuante e chamadas de
     * funções, o que também aumenta consideravelmente a eficiência do algoritmo. 
    */
    for(int i = 0; i < 8; i++)
        cosR[i] = cos((i%8) * div);
    for(int i = 0; i < N; i++) {
        a[0][i] = x[i] + y[0] * cosR[i % 8];
        b[i] = 1.0/x[i] + z[i];
    }
    for(int i = 1; i < N; i++) {
        for(int j = 0; j < N - N % 4; j+=4) {
            a[i][j] = x[j] + y[i] * cosR[j % 8];
            a[i][j+1] = x[j+1] + y[i] * cosR[(j+1) % 8];
            a[i][j+2] = x[j+2] + y[i] * cosR[(j+2) % 8];
            a[i][j+3] = x[j+3] + y[i] * cosR[(j+3) % 8];
        }
        for(int j = N - N % 2; j < N; j++)
            a[i][j] = x[j] + y[i] * cosR[j % 8];
    }
    /*
     *  b) ??
    */

    return 0;
}