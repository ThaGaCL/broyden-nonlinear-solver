#include "doctest.h"

extern "C" {
    #include "utils.h"
    #include "non_linear.h"
}

TEST_CASE("Calculo da funcao de broyden para um vetor")
{
    SUBCASE("Vetor nulo")
    {
        // Edge case: vetor vazio, nenhuma computacao esperada
        // Apenas verifica se o programa nao da segfault

        broyden(nullptr, nullptr, 0);

        // Se chegou aqui sem segfault, o teste passou
    }

    SUBCASE("Vetor com um elemento")
    {
        // x = {2.0}
        // fx[0] = -2(2)² + 3(2) + 1 = -8 + 6 + 1 = -1.0

        lint_t n = 1;
        real_t x[] = {2.0};
        real_t fx[1];

        broyden(fx, x, n);

        CHECK(fx[0] == doctest::Approx(-1.0));
    }

    SUBCASE("Vetor de tamanho 2 com valores positivos")
    {
        // fx[0] = -2(1)² + 3(1) - 2(2) + 1 = -2 + 3 - 4 + 1 = -2.0
        // fx[1] = -2(2)² + 3(2) - (1) = -8 + 6 - 1 = -3.0
        
        lint_t n = 2;
        real_t x[] = {1.0, 2.0};
        real_t fx[2]; 
        
        broyden(fx, x, n);
        
        CHECK(fx[0] == doctest::Approx(-2.0));
        CHECK(fx[1] == doctest::Approx(-3.0));
    }

    SUBCASE("Vetor de tamanho 3 com zeros")
    {
        // fx[0] = -2(0)² + 3(0) - 2(0) + 1 = 1
        // fx[1] = -2(0)² + 3(0) - 0 - 2(0) + 1 = 1
        // fx[2] = -2(0)² + 3(0) - 0 = 0
        
        lint_t n = 3;
        real_t x[]  = {0.0, 0.0, 0.0};
        real_t fx[3]; 
        
        broyden(fx, x, n);
        
        CHECK(fx[0] == doctest::Approx(1.0));
        CHECK(fx[1] == doctest::Approx(1.0));
        CHECK(fx[2] == doctest::Approx(0.0));
    }

    SUBCASE("Vetor de tamanho 4 com valores negativos") 
    {
        // x = {-1.0, -2.0, -1.5, -0.5}
        // fx[0] = -2(-1)² + 3(-1) - 2(-2) + 1 = -2 - 3 + 4 + 1 = 0.0
        // fx[1] = -2(-2)² + 3(-2) - (-1) - 2(-1.5) + 1 = -8 - 6 + 1 + 3 + 1 = -9.0
        // fx[2] = -2(-1.5)² + 3(-1.5) - (-2) - 2(-0.5) + 1 = -4.5 - 4.5 + 2 + 1 + 1 = -5.0
        // fx[3] = -2(-0.5)² + 3(-0.5) - (-1.5) = -0.5 - 1.5 + 1.5 = -0.5
        
        lint_t n = 4;
        real_t x[] = {-1.0, -2.0, -1.5, -0.5};
        real_t fx[4];
        
        broyden(fx, x, n);
        
        CHECK(fx[0] == doctest::Approx(0.0));
        CHECK(fx[1] == doctest::Approx(-9.0));
        CHECK(fx[2] == doctest::Approx(-5.0));
        CHECK(fx[3] == doctest::Approx(-0.5));
    }

    SUBCASE("Vetor de tamanho 5 com valores mistos")
    {
        // x = {1.0, 2.0, -1.0, 0.5, 1.5}
        // fx[0] = -2(1)² + 3(1) - 2(2) + 1 = -2 + 3 - 4 + 1 = -2.0
        // fx[1] = -2(2)² + 3(2) - (1) - 2(-1) + 1 = -8 + 6 - 1 + 2 + 1 = 0.0
        // fx[2] = -2(-1)² + 3(-1) - (2) - 2(0.5) + 1 = -2 - 3 - 2 - 1 + 1 = -7.0
        // fx[3] = -2(0.5)² + 3(0.5) - (-1) - 2(1.5) + 1 = -0.5 + 1.5 + 1 - 3 + 1 = 0.0
        // fx[4] = -2(1.5)² + 3(1.5) - (0.5) = -4.5 + 4.5 - 0.5 = -0.5

        lint_t n = 5;
        real_t x[]  = {1.0, 2.0, -1.0, 0.5, 1.5};
        real_t fx[5];

        broyden(fx, x, n);
        
        CHECK(fx[0] == doctest::Approx(-2.0));
        CHECK(fx[1] == doctest::Approx(0.0));
        CHECK(fx[2] == doctest::Approx(-7.0));
        CHECK(fx[3] == doctest::Approx(0.0));  // Corrigido
        CHECK(fx[4] == doctest::Approx(-0.5)); // Corrigido
    }

    SUBCASE("Vetor com valores altos")
    {
        // x = {100.0, 50.0, 75.0}
        // fx[0] = -2(100)² + 3(100) - 2(50) + 1 = -20000 + 300 - 100 + 1 = -19799.0
        // fx[1] = -2(50)² + 3(50) - 100 - 2(75) + 1 = -5000 + 150 - 100 - 150 + 1 = -5099.0
        // fx[2] = -2(75)² + 3(75) - 50 = -11250 + 225 - 50 = -11075.0
        
        lint_t n = 3;
        real_t x[] = {100.0, 50.0, 75.0};
        real_t fx[3];
        
        broyden(fx, x, n);
        
        CHECK(fx[0] == doctest::Approx(-19799.0));
        CHECK(fx[1] == doctest::Approx(-5099.0));
        CHECK(fx[2] == doctest::Approx(-11075.0));
    }

    SUBCASE("Vetor com valores pequenos")
    {
        // x = {1e-6, 2e-6, 3e-6}
        // fx[0] = -2(1e-6)² + 3(1e-6) - 2(2e-6) + 1 ≈ -2e-12 + 3e-6 - 4e-6 + 1 = 0.999999
        // fx[1] = -2(2e-6)² + 3(2e-6) - 1e-6 - 2(3e-6) + 1 ≈ -8e-12 + 6e-6 - 1e-6 - 6e-6 + 1 = 0.999999
        // fx[2] = -2(3e-6)² + 3(3e-6) - 2e-6 ≈ -18e-12 + 9e-6 - 2e-6 = 0.000007

        lint_t n = 3;
        real_t x[] = {1e-6, 2e-6, 3e-6};
        real_t fx[3];

        broyden(fx, x, n);

        CHECK(fx[0] == doctest::Approx(0.999999));
        CHECK(fx[1] == doctest::Approx(0.999999));
        CHECK(fx[2] == doctest::Approx(0.000007));
    }

    SUBCASE("Vetor com valores muito pequenos e muito grandes")
    {
        // x = {1000.0, 1e-6, 0.5}
        // fx[0] = -2(1000)² + 3(1000) - 2(1e-6) + 1 = -2000000 + 3000 - 0.000002 + 1 ≈ -1996999.0
        // fx[1] = -2(1e-6)² + 3(1e-6) - 1000 - 2(0.5) + 1 = -2e-12 + 3e-6 - 1000 - 1 + 1 ≈ -1000.0
        // fx[2] = -2(0.5)² + 3(0.5) - 1e-6 = -0.5 + 1.5 - 0.000001 ≈ 0.999999

        lint_t n = 3;
        real_t x[] = {1000.0, 1e-6, 0.5};
        real_t fx[3];

        broyden(fx, x, n);

        CHECK(fx[0] == doctest::Approx(-1996999.0));
        CHECK(fx[1] == doctest::Approx(-1000.0));
        CHECK(fx[2] == doctest::Approx(0.999999));
    }

    SUBCASE("Nao escreve para alem dos limites do vetor fx")
    {        
        lint_t n = 3;
        real_t x[] = {1.0, 2.0, 3.0};
        
        // Adiciona "canarios" nas extremidades para detectar vazamentos
        real_t fx_guarded[] = {1234.0, 0.0, 0.0, 0.0, 4321.0};
        
        // Passa o ponteiro deslocado, de forma que fx[0] e na verdade fx_guarded[1]
        real_t* fx = &fx_guarded[1];

        broyden(fx, x, n);

        // Verifica que as bordas da memoria nao foram afetadas (underflow / overflow)
        CHECK(fx_guarded[0] == doctest::Approx(1234.0));
        CHECK(fx_guarded[4] == doctest::Approx(4321.0));

        // A corretude dos valores calculados e coberto pelos outros testes
    }
}