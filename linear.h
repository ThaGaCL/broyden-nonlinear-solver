#ifndef LINEAR_H
#define LINEAR_H

#include <stdio.h>
#include <string.h>
#include <math.h>

void retrosubstitucao(double **A, double *b, double *x, int n);

void eliminacao_gauss(double **A, double *b, int n);

void solve_linear_system(double **A, double *b, double *x, int n);

#endif