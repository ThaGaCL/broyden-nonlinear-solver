#include "broyden.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    // Lê as entradas
    int n, max_it;
    double x0, episilon;

    scanf("%d %lf %lf %d", &n,&x0,&episilon,&max_it); 
    printf("Iniciando o programa com os valores: \n");
    printf("Dimensão do SNLB: %d \n Valor inicial das variáveis: %lf \n Episilon: %.10lf \n Máximo de iterações: %d\n",n,x0,episilon,max_it);
    
    return 0;

}