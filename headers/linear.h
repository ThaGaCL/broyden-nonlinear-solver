#ifndef LINEAR_H
#define LINEAR_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "utils.h"

#define MAX_IT_GAUSS_SEIDEL 100

typedef struct {
    real_t *restrict s;
    real_t *restrict p;
    real_t *restrict i;
    real_t *restrict x;
    real_t *restrict b;
} matrizSOA;

void gaussSeidelSOA(matrizSOA* restrict A, lint_t n, real_t* restrict X, real_t* restrict delta_norm);
matrizSOA *alocaMatrizSOA(lint_t n);
void liberaMatrizSOA(matrizSOA *restrict T);

#endif