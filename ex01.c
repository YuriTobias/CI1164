#include <stdio.h>
#include <stdlib.h>

void updCell(double *s, double *A, double *B, double *d, long size) {
    double *aux = malloc(sizeof(double) * size);

    // B * s
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            aux[i] += B[(size *  i) + j] * s[j];
        }
    }
    // A * aux
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            d[i] += A[(size *  i) + j] * aux[j];
        }
    }

    free(aux);
}

int main() {
    int size;
    double *A, *B, *s, *d;

    scanf("%d", &size);

    A = malloc(sizeof(double) * size * size);
    B = malloc(sizeof(double) * size * size);
    s = malloc(sizeof(double) * size);
    d = malloc(sizeof(double) * size);

    for(int i = 0; i < size * size; i++)
        scanf("%lf", &A[i]);
    for(int i = 0; i < size * size; i++)
        scanf("%lf", &B[i]);
    for(int i = 0; i < size; i++)
        scanf("%lf", &s[i]);

    updCell(s, A, B, d, size);

    for(int i = 0; i < size; i++)
        printf("%.2lf\n", d[i]);

    free(A);
    free(B);
    free(s);
    free(d);

    return 0;
}