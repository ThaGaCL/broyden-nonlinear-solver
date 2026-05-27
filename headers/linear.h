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
} tridiagonal;


void solveLinearSystem(tridiagonal *A, lint_t n);
void gaussSeidelSOA(tridiagonal *A, lint_t n);
tridiagonal *alocaTridiagonalSOA(lint_t n);
void liberaTridiagonalSOA(tridiagonal *T);

#endif