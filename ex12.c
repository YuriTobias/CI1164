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
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N - N % 4; j+=4) {
            c[i] = c[i] + A[i][j] * b[j];
            c[i] = c[i] + A[i][j+1] * b[j+1];
            c[i] = c[i] + A[i][j+2] * b[j+2];
            c[i] = c[i] + A[i][j+3] * b[j+3];
        }
        for(int j = N - N % 4; j < N; j++)
            c[i] = c[i] + A[i][j] * b[j];
    }

    return 0;
}