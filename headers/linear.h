#ifndef LINEAR_H
#define LINEAR_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "utils.h"

#define MAX_IT_GAUSS_SEIDEL 1000

typedef struct {
    real_t i; 
    real_t p; 
    real_t s; 
    real_t x; 
    real_t b; 
} tri_AOS;

void retrosubstitucao(real_t **A, real_t *b, real_t *x, lint_t n);
void retrosubstituicaoAOS(tri_AOS *A, real_t *b, real_t *x, lint_t n);
void eliminacaoGauss(real_t **A, real_t *b, lint_t n);
void solveLinearSystem(tri_AOS *A, real_t *b, real_t *x, lint_t n);
void gaussSeidelSOA(real_t *ds, real_t *dp, real_t *di, real_t *b, real_t *x, lint_t n);
void gaussSeidelAOS(tri_AOS *A, lint_t n);


tri_AOS *alocaTridiagonalAOS(lint_t n);

#endif