#include "utils.h"
#include "linear.h"

/*
Implementacao utilizando a estrategia "struct-of-arrays"

real_t dp[n],
real_t ds[n],
real_t di[n],
real_t x[n],
real_t b[n]

dp0 ds0 0   0   x0 b0
di0 dp1 ds1 0   x1 b1
0   di1 dp2 ds2 x2 b2
0   0   di2 dp2 x3 b3

Incrementa o vetor x a cada iteracao, ou seja, x(i+1) = x(i) + delta
*/
void gaussSeidelSOA(matrizSOA *restrict A, lint_t n, real_t *restrict X, real_t *restrict delta_norm)
{
    lint_t limite = n-1;
    lint_t i;

    for (lint_t j = 0; j < MAX_IT_GAUSS_SEIDEL - 1; ++j)
    {
        // Primeira linha
        A->x[0] = (A->b[0] - A->s[0] * A->x[1]) / A->p[0];

        for (i = 1; i <= limite - 4; i += 4)
        {
            
            real_t sup_1 = A->s[i] * A->x[i + 1];
            real_t inf_1 = A->i[i - 1] * A->x[i - 1];
            A->x[i] = (A->b[i] - sup_1 - inf_1) / A->p[i];

            real_t sup_2 = A->s[i + 1] * A->x[i + 2];
            real_t inf_2 = A->i[i] * A->x[i];
            A->x[i + 1] = (A->b[i + 1] - sup_2 - inf_2) / A->p[i + 1];

            real_t sup_3 = A->s[i + 2] * A->x[i + 3];
            real_t inf_3 = A->i[i + 1] * A->x[i + 1];
            A->x[i + 2] = (A->b[i + 2] - sup_3 - inf_3) / A->p[i + 2];

            real_t sup_4 = A->s[i + 3] * A->x[i + 4];
            real_t inf_4 = A->i[i + 2] * A->x[i + 2];
            A->x[i + 3] = (A->b[i + 3] - sup_4 - inf_4) / A->p[i + 3];
        }

        for(; i < limite; i ++)
        {
            real_t sup = A->s[i] * A->x[i + 1];
            real_t inf = A->i[i - 1] * A->x[i - 1];
        
            A->x[i] = (A->b[i] - sup - inf) / A->p[i];
        }

        A->x[n - 1] = (A->b[n - 1] - A->i[n - 2] * A->x[n - 2]) / A->p[n - 1];
    }

    // Atualiza o vetor X com a ultima iteracao e calcula a norma
    // Primeira linha
    A->x[0] = (A->b[0] - A->s[0] * A->x[1]) / A->p[0];
    X[0] += A->x[0];
    *delta_norm = ABS(A->x[0]);

    for (i = 1; i <= limite - 4; i += 4)
    {
        real_t sup_1 = A->s[i] * A->x[i + 1];
        real_t inf_1 = A->i[i - 1] * A->x[i - 1];
        A->x[i] = (A->b[i] - sup_1 - inf_1) / A->p[i];
        X[i] += A->x[i];
        *delta_norm = (ABS(A->x[i]) > *delta_norm) ? ABS(A->x[i]) : *delta_norm;

        real_t sup_2 = A->s[i + 1] * A->x[i + 2];
        real_t inf_2 = A->i[i] * A->x[i];
        A->x[i + 1] = (A->b[i + 1] - sup_2 - inf_2) / A->p[i + 1];
        X[i + 1] += A->x[i + 1];
        *delta_norm = (ABS(A->x[i + 1]) > *delta_norm) ? ABS(A->x[i + 1]) : *delta_norm;

        real_t sup_3 = A->s[i + 2] * A->x[i + 3];
        real_t inf_3 = A->i[i + 1] * A->x[i + 1];
        A->x[i + 2] = (A->b[i + 2] - sup_3 - inf_3) / A->p[i + 2];
        X[i + 2] += A->x[i + 2];
        *delta_norm = (ABS(A->x[i + 2]) > *delta_norm) ? ABS(A->x[i + 2]) : *delta_norm;

        real_t sup_4 = A->s[i + 3] * A->x[i + 4];
        real_t inf_4 = A->i[i + 2] * A->x[i + 2];
        A->x[i + 3] = (A->b[i + 3] - sup_4 - inf_4) / A->p[i + 3];
        X[i + 3] += A->x[i + 3];
        *delta_norm = (ABS(A->x[i + 3]) > *delta_norm) ? ABS(A->x[i + 3]) : *delta_norm;
    }

    for(; i < limite; i ++)
    {
        real_t sup = A->s[i] * A->x[i + 1];
        real_t inf = A->i[i - 1] * A->x[i - 1];
        A->x[i] = (A->b[i] - sup - inf) / A->p[i];
        X[i] += A->x[i];
        *delta_norm = (ABS(A->x[i]) > *delta_norm) ? ABS(A->x[i]) : *delta_norm;
    }

    A->x[n - 1] = (A->b[n - 1] - A->i[n - 2] * A->x[n - 2]) / A->p[n - 1];
    X[n - 1] += A->x[n - 1];
    *delta_norm = (ABS(A->x[n - 1]) > *delta_norm) ? ABS(A->x[n - 1]) : *delta_norm;
}

matrizSOA *alocaMatrizSOA(lint_t n)
{
    matrizSOA *restrict T = (matrizSOA *)malloc(sizeof(matrizSOA) + 5 * sizeof(real_t) * n);
    
    if (T == NULL)
    {
        return NULL; // Prevencao caso falte memória
    }

    T->s = (real_t *)(T + 1); // Superdiagonal
    T->p = T->s + n; // Diagonal principal
    T->i = T->p + n; // Subdiagonal
    T->x = T->i + n; // Vetor de incógnitas
    T->b = T->x + n; // Vetor de termos independentes

    return T;
}

void liberaMatrizSOA(matrizSOA *restrict T)
{
    if (T != NULL)
    {
        free(T);
    }
}