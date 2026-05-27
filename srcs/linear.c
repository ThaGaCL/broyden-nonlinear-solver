#include "utils.h"
#include "linear.h"

void solveLinearSystem(tridiagonal *A, lint_t n)
{
    gaussSeidelSOA(A, n);
}

/*
Implementacao utilizando a estrategia "struct-of-arrays"

real_t dp[n],
real_t ds[n],
real_t di[n],
real_t x[n+2], x[0] = x[n+1] = 0
real_t b[n+1]

                x0 b0 -> padding para facilitar os calculos
dp0 ds0 0   0   x1 b1
di0 dp1 ds1 0   x2 b2
0   di1 dp2 ds2 x3 b3
0   0   di2 dp2 x4 b4
                x5    -> padding para facilitar os calculos
*/
void gaussSeidelSOA(tridiagonal *A, lint_t n)
{
    for (lint_t j = 0; j < MAX_IT_GAUSS_SEIDEL; ++j)
    {
        for (lint_t i = 1; i < n + 1; ++i)
        {
            real_t sup = A->s[i - 1] * A->x[i + 1];
            real_t inf = A->i[i - 1] * A->x[i - 1];
            A->x[i] = (A->b[i] - sup - inf) / A->p[i - 1];
        }
    }
}

tridiagonal *alocaTridiagonalSOA(lint_t n)
{
    tridiagonal *T = (tridiagonal *)malloc(sizeof(tridiagonal));
    if (T == NULL)
    {
        return NULL; // Prevenção caso falte memória
    }

    T->s = (real_t *)calloc(n, sizeof(real_t));
    T->p = (real_t *)calloc(n, sizeof(real_t));
    T->i = (real_t *)calloc(n, sizeof(real_t));
    T->x = (real_t *)calloc(n + 2, sizeof(real_t));
    T->b = (real_t *)calloc(n + 1, sizeof(real_t));

    return T;
}

void liberaTridiagonalSOA(tridiagonal *T)
{
    if (T != NULL)
    {
        free(T->s);
        free(T->p);
        free(T->i);
        free(T->x);
        free(T->b);

        free(T);
    }
}