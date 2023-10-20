#include <getopt.h> /* getopt */
#include <stdio.h>
#include <stdlib.h> /* exit, malloc, calloc, etc. */
#include <string.h>
#include <time.h>

#include "likwid.h"
#include "matriz.h"
#include "utils.h"

/**
 * Exibe mensagem de erro indicando forma de uso do programa e termina
 * o programa.
 */

static void usage(char *progname) {
    fprintf(stderr, "Forma de uso: %s [ <ordem> ] \n", progname);
    exit(1);
}

/**
 * Programa principal
 * Forma de uso: matmult [ -n <ordem> ]
 * -n <ordem>: ordem da matriz quadrada e dos vetores
 *
 */

int main(int argc, char *argv[]) {
    int n = DEF_SIZE;

    LIKWID_MARKER_INIT;
    MatRow mRow_1, mRow_2, resMat;
    Vetor vet, res;

    /* =============== TRATAMENTO DE LINHA DE COMANDO =============== */

    if (argc < 2) usage(argv[0]);

    n = atoi(argv[1]);

    /* ================ FIM DO TRATAMENTO DE LINHA DE COMANDO ========= */

    srandom(20232);

    res = geraVetor(n, 1);  // (real_t *) malloc (n*sizeof(real_t));
    resMat = geraMatRow(n, n, 1);

    mRow_1 = geraMatRow(n, n, 0);
    mRow_2 = geraMatRow(n, n, 0);

    vet = geraVetor(n, 0);

    if (!res || !resMat || !mRow_1 || !mRow_2 || !vet) {
        fprintf(stderr, "Falha em alocação de memória !!\n");
        liberaVetor((void *)mRow_1);
        liberaVetor((void *)mRow_2);
        liberaVetor((void *)resMat);
        liberaVetor((void *)vet);
        liberaVetor((void *)res);
        exit(2);
    }

#ifdef _DEBUG_
    prnMat(mRow_1, n, n);
    prnMat(mRow_2, n, n);
    prnVetor(vet, n);
    printf("=================================\n\n");
#endif /* _DEBUG_ */

    rtime_t multMatVetTime, multMatMatTime, multMatVetTimeOpt, multMatMatTimeOpt;
    multMatVetTime = timestamp();
    LIKWID_MARKER_START("multMatVet");
    multMatVet(mRow_1, vet, n, n, res);
    LIKWID_MARKER_STOP("multMatVet");
    multMatVetTime = timestamp() - multMatVetTime;

    multMatMatTime = timestamp();
    LIKWID_MARKER_START("multMatMat");
    multMatMat(mRow_1, mRow_2, n, resMat);
    LIKWID_MARKER_STOP("multMatMat");
    multMatMatTime = timestamp() - multMatMatTime;

    printf("multMatVetTime: %lf\n", multMatVetTime);
    printf("multMatMatTime: %lf\n", multMatMatTime);

    liberaVetor((void *)res);
    liberaVetor((void *)resMat);
    res = geraVetor(n, 1);
    resMat = geraMatRow(n, n, 1);

    multMatVetTimeOpt = timestamp();
    LIKWID_MARKER_START("multMatVetOpt");
    multMatVetOpt(mRow_1, vet, n, n, res);
    LIKWID_MARKER_STOP("multMatVetOpt");
    multMatVetTimeOpt = timestamp() - multMatVetTimeOpt;

    multMatMatTimeOpt = timestamp();
    LIKWID_MARKER_START("multMatMatOpt");
    multMatMatOpt(mRow_1, mRow_2, n, resMat);
    LIKWID_MARKER_STOP("multMatMatOpt");
    multMatMatTimeOpt = timestamp() - multMatMatTimeOpt;

    printf("multMatVetTimeOpt: %lf\n", multMatVetTimeOpt);
    printf("multMatMatTimeOpt: %lf\n\n", multMatMatTimeOpt);

#ifdef _DEBUG_
    prnVetor(res, n);
    prnMat(resMat, n, n);
#endif /* _DEBUG_ */

    liberaVetor((void *)mRow_1);
    liberaVetor((void *)mRow_2);
    liberaVetor((void *)resMat);
    liberaVetor((void *)vet);
    liberaVetor((void *)res);

    LIKWID_MARKER_CLOSE;
    return 0;
}
