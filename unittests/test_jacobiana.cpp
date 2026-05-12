#include "doctest.h"

extern "C" {
    #include "utils.h"
    #include "non_linear.h"
}

TEST_CASE("Calculo da matriz jacobiana para a funcao de broyden")
{
    SUBCASE("Vetor nulo")
    {
        // Edge case: vetor vazio, nenhuma computação esperada
        // Apenas verifica se o programa nao da segfault
        jacobiana(nullptr, nullptr, 0);

        // Se chegou aqui sem segfault, o teste passou
    }

    SUBCASE("Vetor com um elemento")
    {
        // x = {2.0}
        // J[0][0] = -4(2) + 3 = -8 + 3 = -5.0

        lint_t n = 1;
        real_t x[] = {2.0};
        real_t **jac = alocaMatrizInicializada(1, 1, 0.0);

        jacobiana(jac, x, n);

        CHECK(jac[0][0] == doctest::Approx(-5.0));

        liberaMatriz(jac, n);
    }

    SUBCASE("Vetor de tamanho 2 com valores positivos")
    {
        // J[0][0] = -4(1) + 3 = -1.0
        // J[0][1] = -2.0
        // J[1][0] = -1.0
        // J[1][1] = -4(2) + 3 = -5.0
        
        lint_t n = 2;
        real_t x[] = {1.0, 2.0};
        real_t **jac = alocaMatrizInicializada(2, 2, 0.0);
        
        jacobiana(jac, x, n);
        
        // Linha 0
        CHECK(jac[0][0] == doctest::Approx(-1.0));
        CHECK(jac[0][1] == doctest::Approx(-2.0));

        // Linha 1
        CHECK(jac[1][0] == doctest::Approx(-1.0));
        CHECK(jac[1][1] == doctest::Approx(-5.0));

        liberaMatriz(jac, n);
    }

    SUBCASE("Vetor de tamanho 3 com zeros")
    {
        // x = {0.0, 0.0, 0.0}
        // As derivadas -4x_i + 3 resultarão todas em 3.0
        
        lint_t n = 3;
        real_t x[]  = {0.0, 0.0, 0.0};
        real_t **jac = alocaMatrizInicializada(3, 3, 0.0);
        
        jacobiana(jac, x, n);
        
        // Linha 0
        CHECK(jac[0][0] == doctest::Approx(3.0));
        CHECK(jac[0][1] == doctest::Approx(-2.0));
        CHECK(jac[0][2] == doctest::Approx(0.0)); // Deve permanecer 0
        
        // Linha 1
        CHECK(jac[1][0] == doctest::Approx(-1.0));
        CHECK(jac[1][1] == doctest::Approx(3.0));
        CHECK(jac[1][2] == doctest::Approx(-2.0));
        
        // Linha 2
        CHECK(jac[2][0] == doctest::Approx(0.0)); // Deve permanecer 0
        CHECK(jac[2][1] == doctest::Approx(-1.0));
        CHECK(jac[2][2] == doctest::Approx(3.0));

        liberaMatriz(jac, n);
    }

SUBCASE("Vetor de tamanho 4 com valores negativos") 
    {
        lint_t n = 4;
        real_t x[] = {-1.0, -2.0, -1.5, -0.5};
        real_t **jac = alocaMatrizInicializada(n, n, 0.0);
        
        jacobiana(jac, x, n);
        
        // Linha 0
        CHECK(jac[0][0] == doctest::Approx(7.0));
        CHECK(jac[0][1] == doctest::Approx(-2.0));
        CHECK(jac[0][2] == doctest::Approx(0.0));
        CHECK(jac[0][3] == doctest::Approx(0.0));
        
        // Linha 1
        CHECK(jac[1][0] == doctest::Approx(-1.0));
        CHECK(jac[1][1] == doctest::Approx(11.0));
        CHECK(jac[1][2] == doctest::Approx(-2.0));
        CHECK(jac[1][3] == doctest::Approx(0.0));
        
        // Linha 2
        CHECK(jac[2][0] == doctest::Approx(0.0));
        CHECK(jac[2][1] == doctest::Approx(-1.0));
        CHECK(jac[2][2] == doctest::Approx(9.0));
        CHECK(jac[2][3] == doctest::Approx(-2.0));
        
        // Linha 3
        CHECK(jac[3][0] == doctest::Approx(0.0));
        CHECK(jac[3][1] == doctest::Approx(0.0));
        CHECK(jac[3][2] == doctest::Approx(-1.0));
        CHECK(jac[3][3] == doctest::Approx(5.0));

        liberaMatriz(jac, n);
    }

    SUBCASE("Vetor de tamanho 5 com valores mistos")
    {
        lint_t n = 5;
        real_t x[]  = {1.0, 2.0, -1.0, 0.5, 1.5};
        real_t **jac = alocaMatrizInicializada(n, n, 0.0);

        jacobiana(jac, x, n);
        
        // Linha 0
        CHECK(jac[0][0] == doctest::Approx(-1.0));
        CHECK(jac[0][1] == doctest::Approx(-2.0));
        CHECK(jac[0][2] == doctest::Approx(0.0));
        CHECK(jac[0][3] == doctest::Approx(0.0));
        CHECK(jac[0][4] == doctest::Approx(0.0));

        // Linha 1
        CHECK(jac[1][0] == doctest::Approx(-1.0));
        CHECK(jac[1][1] == doctest::Approx(-5.0));
        CHECK(jac[1][2] == doctest::Approx(-2.0));
        CHECK(jac[1][3] == doctest::Approx(0.0));
        CHECK(jac[1][4] == doctest::Approx(0.0));

        // Linha 2
        CHECK(jac[2][0] == doctest::Approx(0.0));
        CHECK(jac[2][1] == doctest::Approx(-1.0));
        CHECK(jac[2][2] == doctest::Approx(7.0));
        CHECK(jac[2][3] == doctest::Approx(-2.0));
        CHECK(jac[2][4] == doctest::Approx(0.0));

        // Linha 3
        CHECK(jac[3][0] == doctest::Approx(0.0));
        CHECK(jac[3][1] == doctest::Approx(0.0));
        CHECK(jac[3][2] == doctest::Approx(-1.0));
        CHECK(jac[3][3] == doctest::Approx(1.0));
        CHECK(jac[3][4] == doctest::Approx(-2.0));

        // Linha 4
        CHECK(jac[4][0] == doctest::Approx(0.0));
        CHECK(jac[4][1] == doctest::Approx(0.0));
        CHECK(jac[4][2] == doctest::Approx(0.0));
        CHECK(jac[4][3] == doctest::Approx(-1.0));
        CHECK(jac[4][4] == doctest::Approx(-3.0));

        liberaMatriz(jac, n);
    }

        SUBCASE("Vetor com valores altos")
    {
        lint_t n = 3;
        real_t x[] = {10000.0, 5000.0, 7500.0};
        real_t **jac = alocaMatrizInicializada(3, 3, 0.0);

        jacobiana(jac, x, n);

        CHECK(jac[0][0] == doctest::Approx(-39997.0));
        CHECK(jac[0][1] == doctest::Approx(-2.0));
        CHECK(jac[0][2] == doctest::Approx(0.0));

        CHECK(jac[1][0] == doctest::Approx(-1.0));
        CHECK(jac[1][1] == doctest::Approx(-19997.0));
        CHECK(jac[1][2] == doctest::Approx(-2.0));

        CHECK(jac[2][0] == doctest::Approx(0.0));
        CHECK(jac[2][1] == doctest::Approx(-1.0));
        CHECK(jac[2][2] == doctest::Approx(-29997.0));

        liberaMatriz(jac, n);
    }

    SUBCASE("Vetor com valores pequenos")
    {
        lint_t n = 3;
        real_t x[] = {1e-6, 2e-6, 3e-6};
        real_t **jac = alocaMatrizInicializada(3, 3, 0.0);

        jacobiana(jac, x, n);

        CHECK(jac[0][0] == doctest::Approx(2.999996));
        CHECK(jac[0][1] == doctest::Approx(-2.0));

        CHECK(jac[1][0] == doctest::Approx(-1.0));
        CHECK(jac[1][1] == doctest::Approx(2.999992));
        CHECK(jac[1][2] == doctest::Approx(-2.0));

        CHECK(jac[2][1] == doctest::Approx(-1.0));
        CHECK(jac[2][2] == doctest::Approx(2.999988));

        liberaMatriz(jac, n);
    }

    SUBCASE("Vetor com valores muito pequenos e muito grandes")
    {
        lint_t n = 3;
        real_t x[] = {100000.0, 1e-6, 0.5};
        real_t **jac = alocaMatrizInicializada(3, 3, 0.0);

        jacobiana(jac, x, n);

        CHECK(jac[0][0] == doctest::Approx(-399997.0));
        CHECK(jac[0][1] == doctest::Approx(-2.0));

        CHECK(jac[1][0] == doctest::Approx(-1.0));
        CHECK(jac[1][1] == doctest::Approx(2.999996));
        CHECK(jac[1][2] == doctest::Approx(-2.0));

        CHECK(jac[2][1] == doctest::Approx(-1.0));
        CHECK(jac[2][2] == doctest::Approx(1.0));

        liberaMatriz(jac, n);
    }

    SUBCASE("Nao escreve para alem dos limites da matriz (Horizontal e Vertical)")
    {
        lint_t n = 3;
        real_t x[] = {1.0, 2.0, 3.0};
        
        // Aloca matriz 5x5, a matriz a ser preenchida é a submatriz 3x3 central
        real_t **jac_guarded = alocaMatrizInicializada(5, 5, 0.0);
        
        // Preenche a borda com canários
        for(lint_t k = 0; k < 5; k++)
        {
            // Linhas externas
            jac_guarded[0][k] = 1234.0; 
            jac_guarded[4][k] = 4321.0; 
            
            // Colunas externas
            if (k >= 1 && k <= 3)
            {
                jac_guarded[k][0] = 1234.0; 
                jac_guarded[k][4] = 4321.0; 
            }
        }

        // Passa o ponteiro deslocado, de forma que jac[0][0] é na verdade jac_guarded[1][1]
        real_t* jac[3] = {
            &jac_guarded[1][1], // jac[0][0] aponta para jac_guarded[1][1]
            &jac_guarded[2][1], // jac[1][0] aponta para jac_guarded[2][1]
            &jac_guarded[3][1] // jac[2][0] aponta para jac_guarded[3][1]
        };

        jacobiana(jac, x, n);

        // Verifica que as bordas da matriz não foram modificadas
        for(lint_t k = 0; k < 5; k++) {
            CHECK(jac_guarded[0][k] == doctest::Approx(1234.0)); // Linha superior, underflow horizontal
            CHECK(jac_guarded[4][k] == doctest::Approx(4321.0)); // Linha inferior, overflow horizontal

            if (k >= 1 && k <= 3)
            {
                CHECK(jac_guarded[k][0] == doctest::Approx(1234.0)); // Coluna esquerda, underflow vertical
                CHECK(jac_guarded[k][4] == doctest::Approx(4321.0)); // Coluna direita, overflow vertical
            }
        }

        liberaMatriz(jac_guarded, 5);
    }
}