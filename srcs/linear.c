#include "utils.h"
#include "linear.h"

void solveLinearSystem(matrizSOA *A, real_t *b, real_t *x, lint_t n)
{
    for (lint_t i = 0; i < n; ++i)
    {
        A->b[i] = b[i];
    }

    gaussSeidelSOA(A, n);
    
    for (lint_t i = 0; i < n; ++i)
    {
        x[i] = A->x[i];
    }
}

/*
Implementacao utilizando a estrategia "struct-of-arrays"

real_t dp[n],
real_t ds[n],
real_t di[n],
real_t x[n]
real_t b[n]

dp0 ds0 0   0   x0 b0
di0 dp1 ds1 0   x1 b1
0   di1 dp2 ds2 x2 b2
0   0   di2 dp2 x3 b3
*/
void gaussSeidelSOA(matrizSOA *A, lint_t n)
{
    for (lint_t j = 0; j < MAX_IT_GAUSS_SEIDEL; ++j)
    {
        A->x[0] = (A->b[0] - A->s[0] * A->x[1]) / A->p[0];
        for (lint_t i = 1; i < n - 1; ++i)
        {
            real_t sup = A->s[i] * A->x[i + 1];
            real_t inf = A->i[i - 1] * A->x[i - 1];
            A->x[i] = (A->b[i] - sup - inf) / A->p[i];
        }
        A->x[n - 1] = (A->b[n - 1] - A->i[n - 2] * A->x[n - 2]) / A->p[n - 1];
    }
}

matrizSOA *alocaMatrizSOA(lint_t n)
{
    matrizSOA *T = (matrizSOA *)malloc(sizeof(matrizSOA));
    if (T == NULL)
    {
        return NULL; // Prevencaoo caso falte memoria
    }

    T->s = (real_t *)calloc(n, sizeof(real_t));
    T->p = (real_t *)calloc(n, sizeof(real_t));
    T->i = (real_t *)calloc(n, sizeof(real_t));
    T->x = (real_t *)calloc(n, sizeof(real_t));
    T->b = (real_t *)calloc(n, sizeof(real_t));

    return T;
}

void liberaMatrizSOA(matrizSOA *T)
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