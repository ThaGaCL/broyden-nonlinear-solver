#include "utils.h"
#include "non_linear.h"

/*
=== Broyden ===
f(x) = {
    f_1(x) = -2x_1² + 3x_1 - 2x_2 + 1
    f_i(x) = -2x_i² + 3x_i - x_(i - 1) - 2x_(i + 1) + 1, para i = 2, …, n-1
    f_n(x) = -2x_n² + 3x_n - x_(n - 1)
}

fx deve ser inicializado com n elementos antes de chamar a funcao
Calcula -fx e retona a norma maxima do vetor fx, ou seja, ||F(X(i))||
fx == A->b

=== Jacobiana ===
Como o sistema e tridiagonal, as unicas derivadas nao nulas sao:

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

jac == A->s, A->p, A->i
*/
void jacobiana_broyden(matrizSOA* A, real_t* x, lint_t n, real_t* broyden_norm)
{
    // Primeira equacao (i = 0)
    A->p[0] = -4.0 * x[0] + 3.0; // df_1 / dx_1
    A->b[0] = -(-2 * x[0] * x[0] + 3 * x[0] - 2 * x[1] + 1); // f_1(x)
    *broyden_norm = ABS(A->b[0]);
    
    // Equacoes intermediarias (i = 1, …, n-2)
    for (lint_t i = 1; i < n - 1; i++)
    {
        A->i[i] = -1.0; // Subdiagonal, df_i / dx_(i-1)
        A->p[i] = -4.0 * x[i] + 3.0; // Diagonal principal, df_i / dx_i
        A->s[i] = -2.0; // Superdiagonal, df_i / dx_(i+1)
        A->b[i] = -((-2 * x[i] * x[i]) + (3 * x[i]) - x[i-1] - (2 * x[i+1]) + 1); // f_i(x)
        *broyden_norm = (ABS(A->b[i]) > *broyden_norm) ? ABS(A->b[i]) : *broyden_norm;
    }

    // Ultima equacao (i = n-1)
    A->i[n - 1] = -1.0; // df_n / dx_(n-1)
    A->p[n - 1] = -4.0 * x[n - 1] + 3.0; // df_n / dx_n
    A->b[n-1] = -((-2 * x[n-1] * x[n-1]) + (3 * x[n-1]) - x[n-2]); // f_n(x)
    *broyden_norm = (ABS(A->b[n-1]) > *broyden_norm) ? ABS(A->b[n-1]) : *broyden_norm;
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
    if (n <= 1)
    {
        return;
    }

    rtime_t jac_total_elapsed_time = 0; // Variavel para acumular o tempo gasto nos calculos da jacobiana
    rtime_t linear_total_elapsed_time = 0; // Variavel para acumular o tempo gasto nos calculos do sistema linear

    string_t newton_marker = markerName("newton", (int)n);
    rtime_t newton_elapsed_time = MEDE_TRECHO(newton_marker, {
        string_t jac_marker = markerName("jacobiana", (int)n);
        string_t linear_marker = markerName("sistema_linear", (int)n);

        if (out_file == NULL) {
            out_file = stdout;
        }
        
        //A->b: fx; A->x: delta; Jacobiana: A->s, A->p, A->i
        matrizSOA* A = alocaMatrizSOA(n);
        real_t delta_norm = epsilon + 1;
        real_t broyden_norm = epsilon + 1;

        // Iteracao principal do metodo de Newton: Para i = 0 … max-1:
        for (lint_t i = 0; i < max_it; i++)
        {
            #ifdef ENTREGA
            imprimeIteracao(X, n, out_file); // Imprime a iteracao atual
            fprintf(out_file, "#\n"); // Separador entre iteracoes
            #endif

            // Solucao encontrada: Se || F(X(i)) || < 𝜺1 devolva X(i) ou Se || 𝚫(i) || < 𝜺2 devolva X(i+1)
            if (broyden_norm < epsilon || delta_norm < epsilon)
            {
                break; // Devolve X(i), X(i) e o vetor atual
            }

            // Calcula a jacobiana: J(X(i))
            jac_total_elapsed_time += MEDE_TRECHO(jac_marker, jacobiana_broyden(A, X, borders_n, &broyden_norm));

            // Resolve o sistema linear: J(X(i))𝚫(i) = -F(X(i)) ==> Ax = b ==> J=A; -Fx=b; delta=x
            linear_total_elapsed_time += MEDE_TRECHO(linear_marker, gaussSeidelSOA(A, borders_n, X, &delta_norm));
        }

        #ifdef ENTREGA
        imprimeIteracao(X, n, out_file); // Imprime a iteracao final
        #endif

        // Libera a memoria alocada
        liberaMatrizSOA(A);
    });

    imprimeTempos(newton_elapsed_time, jac_total_elapsed_time, linear_total_elapsed_time, out_file);

    // Nao precisa retornar o X pois ele e modificado in-place
}
