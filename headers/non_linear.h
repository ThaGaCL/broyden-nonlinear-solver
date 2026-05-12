
#ifndef BROYDEN_H
#define BROYDEN_H

#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "linear.h"

real_t norm(real_t* V, lint_t n);
void broyden(real_t* fx, real_t* x, lint_t n);
void jacobiana(real_t** jac, real_t* x, lint_t n);
void newton(real_t* X, real_t epsilon, lint_t max_it, lint_t n, FILE* out_file);

#endif