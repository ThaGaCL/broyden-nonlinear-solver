#include "linear.h"

void eliminacao_gauss(double **A, double *b, int n){
    for (int i = 0; i < n; ++i) {
        int max_i = i;
        
        for (int k = i + 1; k < n; ++k){
            
            if (fabs(A[k][i]) > fabs(A[max_i][i]))
                max_i = k;

            if (max_i != i){
                double *tmp, aux;
                tmp = A[i] = A[max_i];
                A[max_i] = tmp;

                aux = b[i];
                b[i] = b[max_i];
                b[max_i] = aux;
            }

            for (int k = i+1; k < n; ++k){
                double m = A[k][i] / A [i][i];
                A[k][i] = 0.0;
                
                for (int j = i+1; j<n; ++j)
                    A[k][j] -= A[i][j] * m;
                
                b[k] -= b[i] * m;
            }
        }
    }
}

void retrosubstituicao(double **A, double *b, double *x, int n){
    for (int i = n-1; i>=0; --i){
        x[i] = b[i];
        
        for (int j; j < n; ++j)
            x[i] -= A[i][j] * x[j];
        
        x[i] /= A[i][i];
    }
}

void solve_linear_system(double **A, double *b, double *x, int n){
    eliminacao_gauss(A, b, n);
    retrosubstituicao(A, b, x, n);
}