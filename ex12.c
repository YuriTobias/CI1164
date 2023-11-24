#include <stdio.h>
#include <stdlib.h>

#define N 256

int main() {
    double A[N][N], b[N], c[N];

    // Normal
    for(int i = 0; i < N; i++) 
        for(int j = 0; j < N; j++) 
            c[i] = c[i] + A[i][j] * b[j];

    /* Optimized
     * a) Reimplementação da versão original utilizando loop unroll com tamanho 4
     * b) Essa segunda versão com certeza é mais eficiente do que a primeira em uma arquitetura x64. Em especial pelo fato de que
     * assim estamos fazendo menos acessos a memória principal
    */
    for(int i = 0; i < N - N % 4; i+=4) {
        for(int j = 0; j < N; j++) {
            c[i] = c[i] + A[i][j] * b[j];
            c[i+1] = c[i+1] + A[i+1][j] * b[j];
            c[i+2] = c[i+2] + A[i+2][j] * b[j];
            c[i+3] = c[i+3] + A[i+3][j] * b[j];
        }
    }
    for(int i = N - N % 4; i < N; i++) 
        for(int j = 0; j < N; j++)
            c[i] = c[i] + A[i][j] * b[j];

    return 0;
}