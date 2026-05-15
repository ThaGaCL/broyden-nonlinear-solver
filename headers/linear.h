#ifndef LINEAR_H
#define LINEAR_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "utils.h"

typedef struct {
    real_t s;
    real_t p;
    real_t i;
    real_t x;
    real_t b;
} tridiagonal;

void retrosubstitucao(real_t **A, real_t *b, real_t *x, lint_t n);
void eliminacaoGauss(real_t **A, real_t *b, lint_t n);
void solveLinearSystem(real_t **A, real_t *b, real_t *x, lint_t n);
void gaussSeidelSOA(real_t *ds, real_t *dp, real_t *di, real_t *b, real_t *x, lint_t n, lint_t max_it);
void gaussSeidelAOS(tridiagonal *A, lint_t n, lint_t max_it);

#endif