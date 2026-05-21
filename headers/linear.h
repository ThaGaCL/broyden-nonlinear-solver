#ifndef LINEAR_H
#define LINEAR_H
#define MAX_IT 25

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "utils.h"

typedef struct {
    real_t *s;
    real_t *p;
    real_t *i;
    real_t *x;
    real_t *b;
} tridiagonal;


void retrosubstitucao(real_t **A, real_t *b, real_t *x, lint_t n);
void retrosubstitucaoSOA(tridiagonal **A, lint_t n);
void eliminacaoGauss(real_t **A, real_t *b, lint_t n);
void solveLinearSystem(tridiagonal *A, real_t *b, real_t *x, lint_t n);
void gaussSeidelSOA(tridiagonal *A, lint_t n);
// void gaussSeidelAOS(tridiagonal *A, lint_t n, lint_t max_it);
tridiagonal *alocaTridiagonalSOA(lint_t n);
void liberaTridiagonalSOA(tridiagonal *T);

#endif