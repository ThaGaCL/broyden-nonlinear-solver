#include "broyden.h"

double norm(double* Fx, int n)
{

    double max = Fx[0];

    for(int i = 1; i < n; ++i){
        if(Fx[i] > max)
            max = Fx[i];
    }

    return max;
}
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
        solve_linear_system(jacobiana, fx, delta, n); // J(X(i))𝚫(i) = -F(X(i)) ==> Ax = b ==> J=A; -Fx=b; delta=x

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

/*
f(x) = {
    f_1(x) = -2x_1² + 3x_1 - 2x_2 + 1
    f_i(x) = -2x_i² + 3x_i - x_(i - 1) - 2x_(i + 1) + 1, para i = 2, …, n-1
    f_n(x) = -2x_n² + 3x_n - x_(n - 1)
}
*/

void broyden(double* fx, double* x, long long int n) {
    fx[0] = -2*x[0]*x[0] + 3*x[0] - 2*x[1] + 1; // f_1(x)
    
    for (long long int i = 1; i < n-1; i++) {
        fx[i] = -2*x[i]*x[i] + 3*x[i] - x[i-1] - 2*x[i+1] + 1; // f_i(x)
    }

    fx[n-1] = -2*x[n-1]*x[n-1] + 3*x[n-1] - x[n-2]; // f_n(x)
}

// Tem que inicializar Jac na main tudo com 0.0
void calc_jacobiana(double** Jac, double* x, long long int n) {

    // Primeira linha (i = 0)
    Jac[0][0] = -4.0 * x[0] + 3.0; 
    if (n > 1) {
        Jac[0][1] = -2.0;          
    }
    
    for (long long int i = 1; i < n - 1; i++) {
        Jac[i][i - 1] = -1.0;              // Subdiagonal
        Jac[i][i] = -4.0 * x[i] + 3.0; // Diagonal principal
        Jac[i][i + 1] = -2.0;              // Superdiagonal
    }

    // Ultima linha (i = n - 1)
    if (n > 1) {
        Jac[n - 1][n - 2] = -1.0;                  
        Jac[n - 1][n - 1] = -4.0 * x[n - 1] + 3.0; 
    }
}