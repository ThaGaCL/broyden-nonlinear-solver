#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "non_linear.h"

int main(int argc, char *argv[])
{
    LIKWID_MARKER_INIT;

    lint_t n, max_it;
    real_t x0, epsilon;
    FILE *out_file = NULL;

    // Processa a linha de comando
    if (argc == 1) {
        out_file = NULL;
    } else if (argc == 3 && strcmp(argv[1], "-o") == 0) {
        out_file = fopen(argv[2], "w"); // -o <arquivo_saida>

    } else {
        fprintf(stderr, "Uso: %s [-o <arquivo_saida>]\n", argv[0]);
        return 1;
    }

    printf("=> Digite: N x0 epsilon max_it:\n");
    scanf("%lld %lf %lf %lld", &n, &x0, &epsilon, &max_it);

    printf("=> Iniciando o programa com os valores:\n");
    printf("> Dimensao do SNLB: %lld\n", n);
    printf("> Valor inicial das variaveis (x0): %.10g\n", x0);
    printf("> Epsilon: %.10g\n", epsilon);
    printf("> Maximo de iteracoes: %lld\n", max_it);

    // Resolve o sistema pelo metodo de Newton
    real_t *X = alocaVetorInicializado(n, x0);
    newton(X, epsilon, max_it, n, out_file);
    
    // Libera a memoria alocada e fecha o arquivo de saida, se necessario
    liberaVetor(X);
    if (out_file != NULL) {
        fclose(out_file);
    }

    LIKWID_MARKER_CLOSE;

    return 0;
}