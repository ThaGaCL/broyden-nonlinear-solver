#ifndef LINEAR_H
#define LINEAR_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "utils.h"

#define MAX_IT_GAUSS_SEIDEL 10000

typedef struct {
    real_t *s;
    real_t *p;
    real_t *i;
    real_t *x;
    real_t *b;
} matrizSOA;

void solveLinearSystem(matrizSOA* A, lint_t n);
void gaussSeidelSOA(matrizSOA* A, lint_t n);
matrizSOA *alocaMatrizSOA(lint_t n);
void liberaMatrizSOA(matrizSOA *T);

#endif