#include "broyden.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    // Lê as entradas
    long long int n, max_it;
    double x0, episilon;

    scanf("%lld %lf %lf %lld", &n,&x0,&episilon,&max_it); 
    printf("Iniciando o programa com os valores: \n");
    printf("Dimensão do SNLB: %lld \n Valor inicial das variáveis: %lf \n Episilon: %.10lf \n Máximo de iterações: %lld\n",n,x0,episilon,max_it);
    
    double *X = calloc(n, sizeof(double));
    for (long long int i = 0; i < n; i++) {
        X[i] = x0; // Inicializa o vetor X com o valor inicial
    }

    for(int i = 0; i<n; i++)
        printf(" %.10lf ", X[i]);
    printf("\n");
    // gera Jacobiana inicial

    double **J = calloc(n, sizeof(double));
    
    for (long long int i = 0; i < n; i++){
        J[i] = calloc(n, sizeof(double));
    }

    calc_jacobiana(J, X , n);

    // resolve o sistema
    newton(broyden, calc_jacobiana, X, episilon, episilon, max_it, n);
    
    for(int i = 0; i<n; i++)
        printf(" %.10lf ", X[i]);
    
    printf("\n");

    return 0;
}