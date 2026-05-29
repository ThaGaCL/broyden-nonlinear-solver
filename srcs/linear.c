#include "utils.h"
#include "linear.h"

void solveLinearSystem(matrizSOA *A, lint_t n)
{
    gaussSeidelSOA(A, n);
}

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
*/
void gaussSeidelSOA(matrizSOA *A, lint_t n)
{
    for (lint_t j = 0; j < MAX_IT_GAUSS_SEIDEL; ++j)
    {
        // Primeira linha
        A->x[0] = (A->b[0] - A->s[0] * A->x[1]) / A->p[0];

        lint_t limite = n-1;
        lint_t i;

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

        for(; i < limite; i ++){

            real_t sup = A->s[i] * A->x[i + 1];
            real_t inf = A->i[i - 1] * A->x[i - 1];
        
            A->x[i] = (A->b[i] - sup - inf) / A->p[i];
        }

        A->x[n - 1] = (A->b[n - 1] - A->i[n - 2] * A->x[n - 2]) / A->p[n - 1];
    }
}

// void gaussSeidelSOA(matrizSOA *A, lint_t n)
// {
//     for (lint_t j = 0; j < MAX_IT_GAUSS_SEIDEL; ++j)
//     {
//         // Primeira linha
//         A->x[0] = (A->b[0] - A->s[0] * A->x[1]) / A->p[0];

//         lint_t limite = n-1;
//         lint_t i;

//         for (i = 1; i <= limite - 8; i += 8)
//         {
            
//             real_t sup_1 = A->s[i] * A->x[i + 1];
//             real_t inf_1 = A->i[i - 1] * A->x[i - 1];

//             A->x[i] = (A->b[i] - sup_1 - inf_1) / A->p[i];

//             real_t sup_2 = A->s[i + 1] * A->x[i + 2];
//             real_t inf_2 = A->i[i] * A->x[i];

//             A->x[i + 1] = (A->b[i + 1] - sup_2 - inf_2) / A->p[i + 1];

//             real_t sup_3 = A->s[i + 2] * A->x[i + 3];
//             real_t inf_3 = A->i[i + 1] * A->x[i + 1];

//             A->x[i + 2] = (A->b[i + 2] - sup_3 - inf_3) / A->p[i + 2];

//             real_t sup_4 = A->s[i + 3] * A->x[i + 4];
//             real_t inf_4 = A->i[i + 2] * A->x[i + 2];

//             A->x[i + 3] = (A->b[i + 3] - sup_4 - inf_4) / A->p[i + 3];

//             real_t sup_5 = A->s[i + 4] * A->x[i + 5];
//             real_t inf_5 = A->i[i + 3] * A->x[i + 3];
//             A->x[i + 4] = (A->b[i + 4] - sup_5 - inf_5) / A->p[i + 4];

//             real_t sup_6 = A->s[i + 5] * A->x[i + 6];
//             real_t inf_6 = A->i[i + 4] * A->x[i + 4];
//             A->x[i + 5] = (A->b[i + 5] - sup_6 - inf_6) / A->p[i + 5];

//             real_t sup_7 = A->s[i + 6] * A->x[i + 7];
//             real_t inf_7 = A->i[i + 5] * A->x[i + 5];
//             A->x[i + 6] = (A->b[i + 6] - sup_7 - inf_7) / A->p[i + 6];
            
//             real_t sup_8 = A->s[i + 7] * A->x[i + 8];
//             real_t inf_8 = A->i[i + 6] * A->x[i + 6];
//             A->x[i + 7] = (A->b[i + 7] - sup_8 - inf_8) / A->p[i + 7];

//         }

//         for(; i < limite; i ++){

//             real_t sup = A->s[i] * A->x[i + 1];
//             real_t inf = A->i[i - 1] * A->x[i - 1];
        
//             A->x[i] = (A->b[i] - sup - inf) / A->p[i];
//         }

//         A->x[n - 1] = (A->b[n - 1] - A->i[n - 2] * A->x[n - 2]) / A->p[n - 1];
//     }
// }

matrizSOA *alocaMatrizSOA(lint_t n)
{
    matrizSOA *T = (matrizSOA *)malloc(sizeof(matrizSOA));
    if (T == NULL)
    {
        return NULL; // Prevencao caso falte memória
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