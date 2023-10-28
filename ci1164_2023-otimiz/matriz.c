#include "matriz.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // Para uso de função 'memset()'

/**
 * Função que gera valores para para ser usado em uma matriz
 * @param i,j coordenadas do elemento a ser calculado (0<=i,j<n)
 *  @return valor gerado para a posição i,j
 */
static inline real_t generateRandomA(unsigned int i, unsigned int j) {
    static real_t invRandMax = 1.0 / (real_t)RAND_MAX;
    return ((i == j) ? (real_t)(BASE << 1) : 1.0) * (real_t)random() * invRandMax;
}

/**
 * Função que gera valores aleatórios para ser usado em um vetor
 * @return valor gerado
 *
 */
static inline real_t generateRandomB() {
    static real_t invRandMax = 1.0 / (real_t)RAND_MAX;
    return (real_t)(BASE << 2) * (real_t)random() * invRandMax;
}

/* ----------- FUNÇÕES ---------------- */

/**
 *  Funcao geraMatRow: gera matriz como vetor único, 'row-oriented'
 *
 *  @param m     número de linhas da matriz
 *  @param n     número de colunas da matriz
 *  @param zerar se 0, matriz  tem valores aleatórios, caso contrário,
 *               matriz tem valores todos nulos
 *  @return  ponteiro para a matriz gerada
 *
 */

MatRow geraMatRow(int m, int n, int zerar) {
    MatRow matriz = (real_t *)malloc(m * n * sizeof(real_t));

    if (matriz) {
        if (zerar)
            memset(matriz, 0, m * n * sizeof(real_t));
        else
            for (int i = 0; i < m; ++i)
                for (int j = 0; j < n; ++j) matriz[i * n + j] = generateRandomA(i, j);
    }

    return (matriz);
}

/**
 *  Funcao geraVetor: gera vetor de tamanho 'n'
 *
 *  @param n  número de elementos do vetor
 *  @param zerar se 0, vetor  tem valores aleatórios, caso contrário,
 *               vetor tem valores todos nulos
 *  @return  ponteiro para vetor gerado
 *
 */

Vetor geraVetor(int n, int zerar) {
    Vetor vetor = (real_t *)malloc(n * sizeof(real_t));

    if (vetor) {
        if (zerar)
            memset(vetor, 0, n * sizeof(real_t));
        else
            for (int i = 0; i < n; ++i) vetor[i] = generateRandomB();
    }

    return (vetor);
}

/**
 *  \brief: libera vetor
 *
 *  @param  ponteiro para vetor
 *
 */
void liberaVetor(void *vet) { free(vet); }

/**
 *  Funcao multMatVet:  Efetua multiplicacao entre matriz 'mxn' por vetor
 *                       de 'n' elementos
 *  @param mat matriz 'mxn'
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *  @param res vetor que guarda o resultado. Deve estar previamente alocado e com
 *             seus elementos inicializados em 0.0 (zero)
 *  @return vetor de 'm' elementos
 *
 */

void multMatVet(MatRow mat, Vetor v, int m, int n, Vetor res) {
    /* Efetua a multiplicação */
    if (res) {
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j) res[i] += mat[n * i + j] * v[j];
    }
}

/**
 *  Funcao multMatVetUJB:  Efetua multiplicacao entre matriz 'mxn' por vetor
 *                       de 'n' elementos com otimizações de loop unrolling e blocking
 *  @param mat matriz 'mxn'
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *  @param res vetor que guarda o resultado. Deve estar previamente alocado e com
 *             seus elementos inicializados em 0.0 (zero)
 *  @return vetor de 'm' elementos
 *
 */
void multMatVetUJB(restrict MatRow mat, restrict Vetor v, int m, int n, restrict Vetor res) {
    /* Efetua a multiplicação */
    if (res) {
        for (int ii = 0; ii < m / BK; ++ii) {
            int istart = ii * BK;
            int iend = istart + BK;
            for (int jj = 0; jj < n / BK; ++jj) {
                int jstart = jj * BK;
                int jend = jstart + BK;
                for (int i = istart; i < (iend - iend % UF); i += UF) {
                    for (int j = jstart; j < jend; ++j) {
                        res[i] += mat[n * i + j] * v[j];
                        res[i + 1] += mat[n * (i + 1) + j] * v[j];
                        res[i + 2] += mat[n * (i + 2) + j] * v[j];
                        res[i + 3] += mat[n * (i + 3) + j] * v[j];
                    }
                }
                // Remainder loop
                for (int i = iend - iend % UF; i < iend; ++i) {
                    for (int j = jstart; j < jend; ++j) {
                        res[i] += mat[n * i + j] * v[j];
                    }
                }
            }
        }
    }
}

/**
 *  Funcao multMatMat: Efetua multiplicacao de duas matrizes 'n x n'
 *  @param A matriz 'n x n'
 *  @param B matriz 'n x n'
 *  @param n ordem da matriz quadrada
 *  @param C   matriz que guarda o resultado. Deve ser previamente gerada com 'geraMatPtr()'
 *             e com seus elementos inicializados em 0.0 (zero)
 *
 */

void multMatMat(MatRow A, MatRow B, int n, MatRow C) {
    /* Efetua a multiplicação */
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k) C[i * n + j] += A[i * n + k] * B[k * n + j];
}

/**
 *  Funcao multMatMatUJB: Efetua multiplicacao de duas matrizes 'n x n' com otimização de loop unrolling e blocking
 *  @param A matriz 'n x n'
 *  @param B matriz 'n x n'
 *  @param n ordem da matriz quadrada
 *  @param C   matriz que guarda o resultado. Deve ser previamente gerada com 'geraMatPtr()'
 *             e com seus elementos inicializados em 0.0 (zero)
 *
 */
void multMatMatUJB(restrict MatRow A, restrict MatRow B, int n, restrict MatRow C) {
    for (int ii = 0; ii < n / BK; ++ii) {
        int istart = ii * BK;
        int iend = istart + BK;
        for (int jj = 0; jj < n / BK; ++jj) {
            int jstart = jj * BK;
            int jend = jstart + BK;
            for (int kk = 0; kk < n / BK; ++kk) {
                int kstart = kk * BK;
                int kend = kstart + BK;
                // Efetua a multiplicação
                for (int i = istart; i < iend - iend % UF; i += UF) {
                    for (int j = jstart; j < jend; ++j) {
                        for (int k = kstart; k < kend; ++k) {
                            C[i * n + j] += A[i * n + k] * B[k * n + j];
                            C[(i + 1) * n + j] += A[(i + 1) * n + k] * B[k * n + j];
                            C[(i + 2) * n + j] += A[(i + 2) * n + k] * B[k * n + j];
                            C[(i + 3) * n + j] += A[(i + 3) * n + k] * B[k * n + j];
                        }
                    }
                }
                // Remainder loop
                for (int i = iend - iend % UF; i < iend; ++i) {
                    for (int j = jstart; j < jend; ++j) {
                        for (int k = kstart; k < kend; ++k) {
                            C[i * n + j] += A[i * n + k] * B[k * n + j];
                        }
                    }
                }
            }
        }
    }
}

/**
 *  Funcao prnMat:  Imprime o conteudo de uma matriz em stdout
 *  @param mat matriz
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *
 */

void prnMat(MatRow mat, int m, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) printf(DBL_FIELD, mat[n * i + j]);
        printf("\n");
    }
    printf(SEP_RES);
}

/**
 *  Funcao prnVetor:  Imprime o conteudo de vetor em stdout
 *  @param vet vetor com 'n' elementos
 *  @param n número de elementos do vetor
 *
 */

void prnVetor(Vetor vet, int n) {
    for (int i = 0; i < n; ++i) printf(DBL_FIELD, vet[i]);
    printf(SEP_RES);
}
