
#ifndef BROYDEN_H
#define BROYDEN_H

#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "linear.h"

void jacobiana_broyden(matrizSOA* restrict A, real_t* restrict x, lint_t n, real_t* restrict broyden_norm);
void newton(real_t* restrict X, real_t epsilon, lint_t max_it, lint_t n, FILE* restrict out_file);

#endif