
#ifndef BROYDEN_H
#define BROYDEN_H

#include "linear.h"
#include <stdlib.h>

double norm(double* Fx, int n);

void newton(void (*F)(double*, double*, long long int), void (*J)(double**, double*, long long int), double* X, double epsilon1, double epsilon2, long long int max_it, long long int n);


#endif