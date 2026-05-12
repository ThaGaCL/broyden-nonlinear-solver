#ifndef __UTILS_H__
#define __UTILS_H__
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

// Valor absoluto de um número real. Alternativa ao uso da função 'fabs()'
#define ABS(num) ((num) < 0.0 ? -(num) : (num))

// real_t: tipo usado para representar valores em ponto flutuante
typedef double real_t;

// string_t: tipo usado para representar ponteiros para char/strings
typedef char * string_t;

// rtime_t: tipo usado para representar valores de tempo em ponto flutuante
typedef double rtime_t;

// lint_t: tipo usado para representar valores long int
typedef long long int lint_t;

// SIMD alignment macros (bytes)
#define ALIGN_64 __attribute__((aligned(64)))
#define ALIGN_32 __attribute__((aligned(32)))
#define ALIGN_16 __attribute__((aligned(16)))
#define ALIGN_8 __attribute__((aligned(8)))

// Número máximo de dígitos em um número inteiro
#define numDigits(n)  6  // ( n ? (int) log10(ABS(n)) + 1 : 1 )

// Macro para verificar de valor 'n' é potência de 2 ou não.
// 'n' DEVE ser positivo e não-nulo
#define isPot2(n) (!(n & (n - 1)))     // #define isPot2(n) (n && !(n & (n - 1)))

// Funções
rtime_t timestamp(void);
string_t markerName(string_t base_name, int n);
real_t* alocaVetor(lint_t n);
real_t* alocaVetorInicializado(lint_t n, real_t valor_inicial);
real_t **alocaMatriz(lint_t linhas, lint_t colunas);
real_t **alocaMatrizInicializada(lint_t linhas, lint_t colunas, real_t valor_inicial);
void liberaVetor(real_t* vetor);
void liberaMatriz(real_t** matriz, lint_t linhas);
void imprimeIteracao(real_t* X, lint_t n, FILE* out);

#endif // __UTILS_H__