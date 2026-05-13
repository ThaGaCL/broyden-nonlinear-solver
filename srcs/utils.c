#include <stdio.h>
#include <string.h>
#include <math.h>

#include "utils.h"

/*  Retorna tempo em milisegundos desde EPOCH
    Forma de uso:
    rtime_t tempo;
    tempo = timestamp();
    <trecho de programa do qual se deseja medir tempo>
    tempo = timestamp() - tempo;
*/
rtime_t timestamp()
{
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC, &tp);

  return ((rtime_t)tp.tv_sec * 1.0e3 + (rtime_t)tp.tv_nsec * 1.0e-6);
}

/* Gera string '<baseName>_n'
 * Por exemplo, se baseName = "ABC" e n = 10,
 *  Funcao retorna a string "ABC_10"
 * Util para gerar marcadores para LIKWID
 */
string_t markerName(string_t base_name, lint_t n)
{
  string_t mark = (string_t)malloc((strlen(base_name) + 1) + numDigits(n) + 1);
  sprintf(mark, "%s_%lld", base_name, n);

  return mark;
}

// Aloca um vetor de tamanho n, mas nao o inicializa
real_t* alocaVetor(lint_t n)
{
    if (n <= 0)
    {
        return NULL;
    }

    return (real_t*) malloc(n * sizeof(real_t));
}

// Aloca um vetor de tamanho n e o inicializa
real_t* alocaVetorInicializado(lint_t n, real_t valor_inicial)
{
    if (n <= 0)
    {
        return NULL;
    }

    real_t* vetor = (real_t*) malloc(n * sizeof(real_t));

    for (lint_t i = 0; i < n; i++)
    {
        vetor[i] = valor_inicial;
    }

    return vetor;
}

// Aloca uma matriz de tamanho linhas x colunas, mas nao a inicializa
real_t **alocaMatriz(lint_t linhas, lint_t colunas)
{
    if (linhas <= 0 || colunas <= 0)
    {
        return NULL;
    }

    real_t** matriz = (real_t**) malloc(linhas * sizeof(real_t*));

    for (lint_t i = 0; i < linhas; i++)
    {
        matriz[i] = (real_t*) malloc(colunas * sizeof(real_t));
    }

    return matriz;
}

// Aloca uma matriz de tamanho linhas x colunas e a inicializa
real_t **alocaMatrizInicializada(lint_t linhas, lint_t colunas, real_t valor_inicial)
{
    if (linhas <= 0 || colunas <= 0)
    {
        return NULL;
    }

    real_t** matriz = (real_t**) malloc(linhas * sizeof(real_t*));

    for (lint_t i = 0; i < linhas; i++)
    {
        matriz[i] = (real_t*) malloc(colunas * sizeof(real_t));

        for (lint_t j = 0; j < colunas; j++)
        {
            matriz[i][j] = valor_inicial;
        }
    }

    return matriz;
}

// Libera a memoria alocada para um vetor
void liberaVetor(real_t* vetor)
{
    if (vetor)
    {
        free(vetor);
    }
}

// Libera a memoria alocada para uma matriz
void liberaMatriz(real_t** matriz, lint_t linhas)
{
    if (matriz)
    {
        for (lint_t i = 0; i < linhas; i++)
        {
            free(matriz[i]);
        }
        free(matriz);
    }
}

/* Imprime uma iteracao do vetor X no formato:
	x1 = <valor>\n
	x2 = <valor>\n
	...
*/
void imprimeIteracao(real_t* X, lint_t n, FILE* out)
{
    if (!X || n <= 0) return;

    for (lint_t i = 0; i < n; ++i)
    {
        fprintf(out, "x%lld = %.10lf\n", (long long)(i + 1), X[i]);
    }
}

/* Imprime os tempos gastos em cada etapa do metodo de Newton no formato:
###########
# Tempo Total: <tempo total do Metodo de Newton>
# Tempo Jacobiana: <tempo gasto na construcao das Jacobianas>
# Tempo SL: <tempo gasto na resolucao dos sistemas lineares>
###########
*/
void imprimeTempos(rtime_t newton_time, rtime_t jac_time, rtime_t linear_time, FILE* out)
{
    fprintf(out, "###########\n");
    fprintf(out, "# Tempo Total: %.10lf ms\n", newton_time);
    fprintf(out, "# Tempo Jacobiana: %.10lf ms\n", jac_time);
    fprintf(out, "# Tempo SL: %.10lf ms\n", linear_time);
    fprintf(out, "###########\n");
}
