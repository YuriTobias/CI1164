#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int n = 4, n2;
    double A = 1, B = 2, C = 3, D = 4, E = 5;
    double F[n*n], G[n*n], R[n*n];
    clock_t begin, end;

    n2 = n * n;
    for(int i = 0; i < n2; i++) {
        F[i] = 1;
        G[i] = 2;
        R[i] = 3;
    }

    // Optimized
    // Pontos principais: reduzi o número de comparações e juntei algumas operações de modo a realizar menos escritas em R[h]
    begin = clock();
    R[0] = G[0] - A * F[0] - C * F[1];
    for(int h = 1; h < n2 - 2; h++) {
        R[h] = G[h] - A * F[h] - B * F[h - 1] - C * F[h + 1];
    }
    R[n2 - 2] = G[n2 - 2] - A * F[n2 - 2] - B * F[n2 - 3];
    R[n2 - 1] = G[n2 - 1] - A * F[n2 - 1] - B * F[n2 - 2] - D * F[n2 - n - 1];
    for(int h = n; h < n2 - 1; h++) {
        R[h] -= D * F[h - n];
        R[h - n] -= E * F[h];
    }
    end = clock();
    
    
    // "Normal"
    //begin = clock();
    // for(int h = 0; h < n2; h++) {
    //     R[h] = G[h] - A * F[h];
    //         if(h-1 >= 0)
    //             R[h] -= B * F[h - 1];
    //         if(h+1 < n2 - 1)
    //             R[h] -= C * F[h + 1];
    //         if(h - n >= 0)
    //             R[h] -= D * F[h - n];
    //         if(h + n < n2 - 1)
    //             R[h] -= E * F[h + n];
    // }
    //end = clock();

    for(int i = 0; i < n2; i++)
        printf("%.2lf\n", R[i]);
    printf("Tempo: %.8lf segundos\n", ((double) (end - begin)) / CLOCKS_PER_SEC);

    return 0;
}