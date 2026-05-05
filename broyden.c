#include "broyden.h"
#include <stdlib.h>

/*
Newton(F, J, X(0), 𝜺1, 𝜺2, max):
    Para i = 0 … max-1:
        Se || F(X(i)) || < 𝜺1 devolva X(i)
        𝚫(i) = resolva o sistema linear J(X(i))𝚫(i) = -F(X(i))
        X(i+1) = X(i) + 𝚫(i)
        Se || 𝚫(i) || < 𝜺2 devolva X(i+1)
*/

double* newton(double* (*F)(double*), double** (*J)(double*), double* X, double epsilon1, double epsilon2, long long int max_it) {
    double* delta = NULL; // Vetor de variação
    for (long long int i = 0; i < max_it; i++) {
        if (norm(F(X)) < epsilon1) {
            break;
        }
        // Zerar Fx
        double* minus_fx = F(X);
        for (int j = 0; j < sizeof(minus_fx) / sizeof(minus_fx[0]); j++) {
            minus_fx[j] = -minus_fx[j];
        }
        delta = solve_linear_system(J(X), minus_fx); // Resolver o sistema linear
        // Atualizar a solução
        for (int j = 0; j < sizeof(X) / sizeof(X[0]); j++) {
            X[j] += delta[j];
        }
        if (norm(delta) < epsilon2) {
            break;
        }
    }
    if (delta != NULL) {
        free(delta);
    }
    return X;
}