#include "utils.h"
#include "linear.h"

void eliminacaoGauss(real_t **A, real_t *b, lint_t n)
{
    for (lint_t i = 0; i < n; ++i)
    {
        // Troca a linha i pela linha com o maior valor absoluto na coluna i (pivotamento parcial)
        lint_t max_i = i;
        for (lint_t k = i + 1; k < n; ++k)
        {
            if (fabs(A[k][i]) > fabs(A[max_i][i]))
            {
                max_i = k; // Valor do elemento atual eh maior que o maximo encontrado ate agora
            }
        }
        
        // Se o indice maximo for diferente do indice atual, troca as linhas
        if (max_i != i)
        {
            real_t *tmp, aux;
            tmp = A[i];
            A[i] = A[max_i];
            A[max_i] = tmp;

            aux = b[i];
            b[i] = b[max_i];
            b[max_i] = aux;
        }

        // Elimina as linhas abaixo da linha i
        for (lint_t k = i + 1; k < n; ++k)
        {
            real_t m = A[k][i] / A [i][i]; // Fator de multiplicacao para eliminar o elemento A[k][i]
            A[k][i] = 0.0; // Zera o elemento abaixo do pivo
                
            // Atualiza os elementos restantes da linha k
            for (lint_t j = i+1; j<n; ++j)
            {
                A[k][j] -= A[i][j] * m;
            }
                
            // Atualiza o elemento correspondente do vetor b
            b[k] -= b[i] * m;
        }
    }
}

void retrosubstituicao(real_t **A, real_t *b, real_t *x, lint_t n)
{
    for (lint_t i = n-1; i>=0; --i)
    {
        // Inicializa o elemento atual com o valor correspondente do vetor b
        x[i] = b[i];
        
        for (lint_t j = i+1; j < n; ++j)
        {
            x[i] -= A[i][j] * x[j]; // Subtrai os termos ja calculados da linha atual
        }
        
        x[i] /= A[i][i]; // Divide pelo elemento diagonal para obter o valor final de x[i]
    }
}

void solveLinearSystem(real_t **A, real_t *b, real_t *x, lint_t n)
{
    eliminacaoGauss(A, b, n);
    retrosubstituicao(A, b, x, n);
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
void gaussSeidelSOA(real_t *ds, real_t *dp, real_t *di, real_t *b, real_t *x, lint_t n, lint_t max_it)
{
    for (lint_t j = 0; j < max_it; ++j)
    {
        for (lint_t i = 1; i < n + 1; ++i)
        {
            real_t sup = ds[i] * x[i+1];
            real_t inf = di[i-2] * x[i-1];
            x[i] = (b[i] - sup - inf) / dp[i-1];
        }
    }
}

/*
Implementacao utilizando a estrategia "arrays-of-struct"

tridiagonal A[n+2], A[0].x = A[n+1].x = 0


                       A0.x  A0.b -> padding para facilitar os calculos
A1.p  A1.s  0    0     A1.x  A1.b
A2.i  A2.p  A2.s 0     A2.x  A2.b
0     A3.i  A3.p A3.s  A3.x  A3.b
0     0     A4.i A4.p  A4.x  A4.b
                       A5.x       -> padding para facilitar os calculos
*/
void gaussSeidelAOS(tridiagonal *A, lint_t n, lint_t max_it)
{
    for (lint_t j = 0; j < max_it; ++j)
    {
        for (lint_t i = 1; i < n + 1; ++i)
        {
            real_t sup = A[i].s * A[i+1].x;
            real_t inf = A[i].i * A[i-1].x;
            A[i].x = (A[i].b - sup - inf) / A[i].p;
        }
    }
}
