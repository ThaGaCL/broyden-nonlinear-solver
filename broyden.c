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

void newton(void (*F)(double*, double*, long long int), void (*J)(double**, double*, long long int), double* X, double epsilon1, double epsilon2, long long int max_it, long long int n) {
    double* delta = calloc(n, sizeof(double)); // Vetor de variação
    double* fx = calloc(n, sizeof(double)); // Vetor de F(X)
    double** jacobiana = calloc(n, sizeof(double*)); // Matriz Jacobiana
    for (long long int i = 0; i < n; i++) {
        jacobiana[i] = calloc(n, sizeof(double));
    }

    for (long long int i = 0; i < max_it; i++) {
        F(fx, X, n);
        if (norm(fx, n) < epsilon1) break; // Solução encontrada
        
        for (long long int j = 0; j < n; j++) fx[j] = -fx[j]; // Inverter Fx

        J(jacobiana, X, n); // Calcular a Jacobiana
        solve_linear_system(delta, jacobiana, fx, n); // Resolver o sistema linear

        for (long long int j = 0; j < n; j++) X[j] += delta[j]; // Atualizar a solução

        if (norm(delta, n) < epsilon2) break; // Solução encontrada
    }

    free(delta);
    free(fx);
    for (long long int i = 0; i < n; i++) {
        free(jacobiana[i]);
    }
    free(jacobiana);

    // Não precisa retornar o X pois ele é modificado in-place
}