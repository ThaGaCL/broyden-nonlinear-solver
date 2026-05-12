#ifndef LINEAR_H
#define LINEAR_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "utils.h"

void retrosubstitucao(real_t **A, real_t *b, real_t *x, lint_t n);
void eliminacaoGauss(real_t **A, real_t *b, lint_t n);
void solveLinearSystem(real_t **A, real_t *b, real_t *x, lint_t n);

#endif