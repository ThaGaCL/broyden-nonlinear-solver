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
                max_i = k; // Valor do elemento atual eh maior que o maximo encontrado até agora
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
            real_t m = A[k][i] / A [i][i]; // Fator de multiplicação para eliminar o elemento A[k][i]
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
            x[i] -= A[i][j] * x[j]; // Subtrai os termos já calculados da linha atual
        }
        
        x[i] /= A[i][i]; // Divide pelo elemento diagonal para obter o valor final de x[i]
    }
}

void solveLinearSystem(real_t **A, real_t *b, real_t *x, lint_t n)
{
    eliminacaoGauss(A, b, n);
    retrosubstituicao(A, b, x, n);
}