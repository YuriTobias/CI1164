#include <fenv.h>
#include <stdio.h>
#include <stdlib.h>

#include "polynomial_ops.h"
#include "linear_ops.h"

int main(int argc, char *argv[]) {
    fesetround(FE_DOWNWARD);
    double **points;
    int n, k;
    perfSquare(&points, &n, &k);
    //gaussElimPivot();
    //backSubstitution();
    //calcResidue();
}