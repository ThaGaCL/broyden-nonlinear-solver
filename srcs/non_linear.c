#include "utils.h"
#include "non_linear.h"

/* Calcula a norma pelo máximo no vetor */
real_t norm(real_t* V, lint_t n)
{
    real_t max = fabs(V[0]);

    for(lint_t i = 1; i < n; ++i)
    {
        if(fabs(V[i]) > max)
        {
            max = fabs(V[i]);
        }
    }

    return max;
}

/*
f(x) = {
    f_1(x) = -2x_1² + 3x_1 - 2x_2 + 1
    f_i(x) = -2x_i² + 3x_i - x_(i - 1) - 2x_(i + 1) + 1, para i = 2, …, n-1
    f_n(x) = -2x_n² + 3x_n - x_(n - 1)
}

fx deve ser inicializado com n elementos antes de chamar a função
*/
void broyden(real_t* fx, real_t* x, lint_t n)
{
    if (n <= 0)
    {
        return; // Não faz nada caso o vetor seja vazio
    }

    // Primeira equação (i = 0)
    fx[0] = -2 * x[0] * x[0] + 3 * x[0] - 2 * x[1] + 1; // f_1(x)
    
    // Equações intermediárias (i = 1, …, n-2)
    for (lint_t i = 1; i < n - 1; i++)
    {
        fx[i] = (-2 * x[i] * x[i]) + (3 * x[i]) - x[i-1] - (2 * x[i+1]) + 1; // f_i(x)
    }

    // Ultima equação (i = n-1)
    if (n > 1)
    {
        fx[n-1] = (-2 * x[n-1] * x[n-1]) + (3 * x[n-1]) - x[n-2]; // f_n(x)
    }
}

/*
Como o sistema é tridiagonal, as únicas derivadas não nulas são:

J_1(x) = {
    df_1 / dx_1 = -4x_1 + 3
    df_1 / dx_2 = -2
}

J_i(x) = {
    df_i / dx_(i-1) = -1
    df_i / dx_i     = -4x_i + 3
    df_i / dx_(i+1) = -2
}, para i = 2, …, n-1

J_n(x) = {
    df_n / dx_(n-1) = -1
    df_n / dx_n     = -4x_n + 3
}

jac deve ser inicializada como uma matriz n x n preenchida com zerosantes de chamar a função
*/
void jacobiana(real_t** jac, real_t* x, lint_t n)
{
    if (n <= 0)
    {
        return; // Não faz nada caso o vetor seja vazio
    }

    // Primeira linha (i = 0)
    jac[0][0] = -4.0 * x[0] + 3.0; // df_1 / dx_1
    if (n > 1)
    {
        jac[0][1] = -2.0; // df_1 / dx_2  
    }
    
    // Linhas intermediárias (i = 1, …, n-2)
    for (lint_t i = 1; i < n - 1; i++)
    {
        jac[i][i - 1] = -1.0; // Subdiagonal, df_i / dx_(i-1)
        jac[i][i] = -4.0 * x[i] + 3.0; // Diagonal principal, df_i / dx_i
        jac[i][i + 1] = -2.0; // Superdiagonal, df_i / dx_(i+1)
    }

    // Ultima linha (i = n - 1)
    if (n > 1)
    {
        jac[n - 1][n - 2] = -1.0; // df_n / dx_(n-1)
        jac[n - 1][n - 1] = -4.0 * x[n - 1] + 3.0; // df_n / dx_n
    }
}

/*
Newton(F, J, X(0), 𝜺1, 𝜺2, max):
    Para i = 0 … max-1:
        Se || F(X(i)) || < 𝜺1 devolva X(i)
        𝚫(i) = resolva o sistema linear J(X(i))𝚫(i) = -F(X(i))
        X(i+1) = X(i) + 𝚫(i)
        Se || 𝚫(i) || < 𝜺2 devolva X(i+1)

𝜺1 == 𝜺2
*/
void newton(real_t* X, real_t epsilon, lint_t max_it, lint_t n, FILE* out_file)
{
    if (out_file == NULL) {
        out_file = stdout;
    }

    // Aloca vetores e matrizes auxiliares
    real_t* delta = alocaVetor(n);
    real_t* fx = alocaVetor(n);
    real_t** jac = alocaMatrizInicializada(n, n, 0);

    // Iteracao principal do metodo de Newton: Para i = 0 … max-1:
    for (lint_t i = 0; i < max_it; i++)
    {
        imprimeIteracao(X, n, out_file); // Imprime a iteracao atual
        fprintf(out_file, "#\n"); // Separador entre iteracoes

        // Calcula broyden: F(X(i))
        broyden(fx, X, n);

        // Solucao encontrada: Se || F(X(i)) || < 𝜺1 devolva X(i)
        if (norm(fx, n) < epsilon)
        {
            break; // Devolve X(i), X(i) é o vetor atual
        }
        
        // Inverte Fx: -F(X(i))
        for (lint_t j = 0; j < n; j++)
        {
            fx[j] = -fx[j];
        }

        // Calcula a jacobiana: J(X(i))
        jacobiana(jac, X, n);

        // Resolve o sistema linear: J(X(i))𝚫(i) = -F(X(i)) ==> Ax = b ==> J=A; -Fx=b; delta=x
        solveLinearSystem(jac, fx, delta, n);

        // Atualiza a solucao: X(i+1) = X(i) + 𝚫(i)
        for (lint_t j = 0; j < n; j++)
        {
            X[j] += delta[j];
        }

        // Solucao encontrada: Se || 𝚫(i) || < 𝜺2 devolva X(i+1)
        if (norm(delta, n) < epsilon)
        {
            break; // Devolve X(i+1), X ja foi atualizado
        }
    }

    imprimeIteracao(X, n, out_file); // Imprime a iteracao final

    // Libera a memoria alocada
    liberaVetor(delta);
    liberaVetor(fx);
    liberaMatriz(jac, n);

    // Não precisa retornar o X pois ele é modificado in-place
}