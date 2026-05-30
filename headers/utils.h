#ifndef __UTILS_H__
#define __UTILS_H__
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

#include <likwid.h>

// Valor absoluto de um número real. Alternativa ao uso da funcao 'fabs()'
#define ABS(num) ((num) < 0.0 ? -(num) : (num))

// real_t: tipo usado para representar valores em ponto flutuante
typedef double real_t;

// string_t: tipo usado para representar ponteiros para char/strings
typedef char * string_t;

// rtime_t: tipo usado para representar valores de tempo em ponto flutuante
typedef double rtime_t;

// lint_t: tipo usado para representar valores long int
typedef long long int lint_t;

// Numero maximo de digitos em um numero inteiro
#define numDigits(n)  6  // ( n ? (int) log10(ABS(n)) + 1 : 1 )

// Macro para medir o tempo gasto em um trecho de codigo usando LIKWID
#define MEDE_TRECHO(marker_name, trecho) \
({ \
    rtime_t start_time = timestamp(); \
    LIKWID_MARKER_START(marker_name); \
    do { trecho; } while (0); \
    LIKWID_MARKER_STOP(marker_name); \
    timestamp() - start_time; \
})

// Funcoes
rtime_t timestamp();
string_t markerName(string_t base_name, lint_t n);
real_t* alocaVetor(lint_t n);
real_t* alocaVetorInicializado(lint_t n, real_t valor_inicial);
void liberaVetor(real_t* restrict vetor);
void imprimeIteracao(real_t* restrict X, lint_t n, FILE* restrict out);
void imprimeTempos(rtime_t newton_time, rtime_t jac_time, rtime_t linear_time, FILE* restrict out);

#endif // __UTILS_H__
