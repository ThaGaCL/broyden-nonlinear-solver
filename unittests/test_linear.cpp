#include <cmath>
#include "doctest.h"

extern "C" {
    #include "linear.h"
    #include "utils.h"
}

TEST_CASE("Calculo da solucao de um sistema linear")
{
    SUBCASE("Sistema nulo")
    {
        // Edge case: sistema vazio, nenhuma computacao esperada
        // Apenas verifica se o programa nao da segfault

        solveLinearSystem(nullptr, nullptr, nullptr, 0);

        // Se chegou aqui sem segfault, o teste passou
    }

    SUBCASE("Sistema 1x1")
    {
        // 4x = 8 -> x = 2

        lint_t n = 1;
        real_t **A = alocaMatrizInicializada(n, n, 0.0);
        real_t b[] = {8.0};
        real_t x[] = {0.0};

        A[0][0] = 4.0;

        solveLinearSystem(A, b, x, n);

        CHECK(x[0] == doctest::Approx(2.0));

        liberaMatriz(A, n);
    }

    SUBCASE("Sistema 2x2 com troca de linhas")
    {
        // Sistema:
        // 0x + 2y = 4  -> y = 2
        // x + 3y = 5   -> x + 6 = 5 -> x = -1

        lint_t n = 2;
        real_t **A = alocaMatrizInicializada(n, n, 0.0);
        real_t b[] = {4.0, 5.0};
        real_t x[] = {0.0, 0.0};

        A[0][0] = 0.0;
        A[0][1] = 2.0;
        A[1][0] = 1.0;
        A[1][1] = 3.0;

        solveLinearSystem(A, b, x, n);

        CHECK(x[0] == doctest::Approx(-1.0));
        CHECK(x[1] == doctest::Approx(2.0));

        liberaMatriz(A, n);
    }

    SUBCASE("Sistema 3x3 com solucao conhecida")
    {
        // Sistema:
        //  2x +  y - z =   8
        // -3x -  y + 2z = -11
        // -2x +  y + 2z =  -3
        // Solucao: x = 2, y = 3, z = -1

        lint_t n = 3;
        real_t **A = alocaMatrizInicializada(n, n, 0.0);
        real_t b[] = {8.0, -11.0, -3.0};
        real_t x[] = {0.0, 0.0, 0.0};

        A[0][0] = 2.0;
        A[0][1] = 1.0;
        A[0][2] = -1.0;
        A[1][0] = -3.0;
        A[1][1] = -1.0;
        A[1][2] = 2.0;
        A[2][0] = -2.0;
        A[2][1] = 1.0;
        A[2][2] = 2.0;

        solveLinearSystem(A, b, x, n);

        CHECK(x[0] == doctest::Approx(2.0));
        CHECK(x[1] == doctest::Approx(3.0));
        CHECK(x[2] == doctest::Approx(-1.0));

        liberaMatriz(A, n);
    }

    SUBCASE("Sistema diagonal com solucao direta")
    {
        // Sistema:
        // 2x = 8   -> x = 4
        // -3y = 6  -> y = -2
        // 5z = 5   -> z = 1

        lint_t n = 3;
        real_t **A = alocaMatrizInicializada(n, n, 0.0);
        real_t b[] = {8.0, 6.0, 5.0};
        real_t x[] = {0.0, 0.0, 0.0};

        A[0][0] = 2.0;
        A[1][1] = -3.0;
        A[2][2] = 5.0;

        solveLinearSystem(A, b, x, n);

        CHECK(x[0] == doctest::Approx(4.0));
        CHECK(x[1] == doctest::Approx(-2.0));
        CHECK(x[2] == doctest::Approx(1.0));

        liberaMatriz(A, n);
    }

    SUBCASE("Sistema 3x3 com pivotamento e coeficientes mistos")
    {
        // Sistema:
        // 0x + 2y + z = 1
        // x - y + 2z = 9
        // 3x + y + z = 8
        // Solucao: x = 2, y = -1, z = 3

        lint_t n = 3;
        real_t **A = alocaMatrizInicializada(n, n, 0.0);
        real_t b[] = {1.0, 9.0, 8.0};
        real_t x[] = {0.0, 0.0, 0.0};

        A[0][0] = 0.0;
        A[0][1] = 2.0;
        A[0][2] = 1.0;
        A[1][0] = 1.0;
        A[1][1] = -1.0;
        A[1][2] = 2.0;
        A[2][0] = 3.0;
        A[2][1] = 1.0;
        A[2][2] = 1.0;

        solveLinearSystem(A, b, x, n);

        CHECK(x[0] == doctest::Approx(2.0));
        CHECK(x[1] == doctest::Approx(-1.0));
        CHECK(x[2] == doctest::Approx(3.0));

        liberaMatriz(A, n);
    }

    SUBCASE("Sistema 3x3 com valores decimais")
    {
        // Sistema:
        // 1.5x - 0.5y + 2z = 4.5
        // 4y - z = -4.5
        // 2x + y + 3z = 4.5
        // Solucao: x = 2, y = -1, z = 0.5

        lint_t n = 3;
        real_t **A = alocaMatrizInicializada(n, n, 0.0);
        real_t b[] = {4.5, -4.5, 4.5};
        real_t x[] = {0.0, 0.0, 0.0};

        A[0][0] = 1.5;
        A[0][1] = -0.5;
        A[0][2] = 2.0;
        A[1][1] = 4.0;
        A[1][2] = -1.0;
        A[2][0] = 2.0;
        A[2][1] = 1.0;
        A[2][2] = 3.0;

        solveLinearSystem(A, b, x, n);

        CHECK(x[0] == doctest::Approx(2.0));
        CHECK(x[1] == doctest::Approx(-1.0));
        CHECK(x[2] == doctest::Approx(0.5));

        liberaMatriz(A, n);
    }

    SUBCASE("Sistema 5x5 com solucao conhecida")
    {
        // Sistema tridiagonal:
        // 4x +  y           =  6
        //  x + 4y +  z      = 12
        //      y + 4z +  w  = 18
        //           z + 4w + t = 24
        //                w + 4t = 24
        // Solucao: x = 1, y = 2, z = 3, w = 4, t = 5

        lint_t n = 5;
        real_t **A = alocaMatrizInicializada(n, n, 0.0);
        real_t b[] = {6.0, 12.0, 18.0, 24.0, 24.0};
        real_t x[] = {0.0, 0.0, 0.0, 0.0, 0.0};

        A[0][0] = 4.0;
        A[0][1] = 1.0;
        A[1][0] = 1.0;
        A[1][1] = 4.0;
        A[1][2] = 1.0;
        A[2][1] = 1.0;
        A[2][2] = 4.0;
        A[2][3] = 1.0;
        A[3][2] = 1.0;
        A[3][3] = 4.0;
        A[3][4] = 1.0;
        A[4][3] = 1.0;
        A[4][4] = 4.0;

        solveLinearSystem(A, b, x, n);

        CHECK(x[0] == doctest::Approx(1.0));
        CHECK(x[1] == doctest::Approx(2.0));
        CHECK(x[2] == doctest::Approx(3.0));
        CHECK(x[3] == doctest::Approx(4.0));
        CHECK(x[4] == doctest::Approx(5.0));

        liberaMatriz(A, n);
    }

    SUBCASE("Sistema sem solucao")
    {
        // Sistema inconsistente:
        // x + y = 2
        // 2x + 2y = 5
        // A eliminacao de Gauss produz uma linha do tipo 0x + 0y = valor != 0.
        // Como o solver atual nao detecta singularidade, a retrosubstituicao
        // acaba dividindo por zero e gera valores nao finitos.

        lint_t n = 2;
        real_t **A = alocaMatrizInicializada(n, n, 0.0);
        real_t b[] = {2.0, 5.0};
        real_t x[] = {0.0, 0.0};

        A[0][0] = 1.0;
        A[0][1] = 1.0;
        A[1][0] = 2.0;
        A[1][1] = 2.0;

        solveLinearSystem(A, b, x, n);

        CHECK((std::isnan(x[0]) || std::isinf(x[0])));
        CHECK((std::isnan(x[1]) || std::isinf(x[1])));

        liberaMatriz(A, n);
    }

    SUBCASE("Sistema com infinitas solucoes")
    {
        // Sistema dependente:
        // x + y = 2
        // 2x + 2y = 4
        // A eliminacao de Gauss produz uma linha do tipo 0x + 0y = 0.
        // Como o solver atual nao trata pivo nulo, a retrosubstituicao
        // faz 0/0 e o resultado esperado e NaN.

        lint_t n = 2;
        real_t **A = alocaMatrizInicializada(n, n, 0.0);
        real_t b[] = {2.0, 4.0};
        real_t x[] = {0.0, 0.0};

        A[0][0] = 1.0;
        A[0][1] = 1.0;
        A[1][0] = 2.0;
        A[1][1] = 2.0;

        solveLinearSystem(A, b, x, n);

        CHECK(std::isnan(x[0]));
        CHECK(std::isnan(x[1]));

        liberaMatriz(A, n);
    }

    SUBCASE("Sistema com valores muito grandes")
    {
        // Sistema:
        // 1000000x + y = 2000003
        // x + 1000000y + z = 3000006
        // y + 1000000z = 4000003
        // Solucao: x = 2, y = 3, z = 4

        lint_t n = 3;
        real_t **A = alocaMatrizInicializada(n, n, 0.0);
        real_t b[] = {2000003.0, 3000006.0, 4000003.0};
        real_t x[] = {0.0, 0.0, 0.0};

        A[0][0] = 1000000.0;
        A[0][1] = 1.0;
        A[1][0] = 1.0;
        A[1][1] = 1000000.0;
        A[1][2] = 1.0;
        A[2][1] = 1.0;
        A[2][2] = 1000000.0;

        solveLinearSystem(A, b, x, n);

        CHECK(x[0] == doctest::Approx(2.0));
        CHECK(x[1] == doctest::Approx(3.0));
        CHECK(x[2] == doctest::Approx(4.0));

        liberaMatriz(A, n);
    }

    SUBCASE("Sistema com valores muito pequenos")
    {
        // Sistema:
        // 1e-9x + y = -1.999999999
        // x + 1e-9y + z = 2.999999998
        // y + 1e-9z = -1.999999998
        // Solucao: x = 1, y = -2, z = 2

        lint_t n = 3;
        real_t **A = alocaMatrizInicializada(n, n, 0.0);
        real_t b[] = {-1.999999999, 2.999999998, -1.999999998};
        real_t x[] = {0.0, 0.0, 0.0};

        A[0][0] = 1e-9;
        A[0][1] = 1.0;
        A[1][0] = 1.0;
        A[1][1] = 1e-9;
        A[1][2] = 1.0;
        A[2][1] = 1.0;
        A[2][2] = 1e-9;

        solveLinearSystem(A, b, x, n);

        CHECK(x[0] == doctest::Approx(1.0));
        CHECK(x[1] == doctest::Approx(-2.0));
        CHECK(x[2] == doctest::Approx(2.0));

        liberaMatriz(A, n);
    }

    SUBCASE("Sistema com valores muito pequenos e muito grandes")
    {
        // Sistema com escalas bem diferentes:
        // 1e9x + y = 2000000003
        // x + 1e-9y + z = 6.000000003
        // y + 1e-9z = 3.000000004
        // Solucao: x = 2, y = 3, z = 4

        lint_t n = 3;
        real_t **A = alocaMatrizInicializada(n, n, 0.0);
        real_t b[] = {2000000003.0, 6.000000003, 3.000000004};
        real_t x[] = {0.0, 0.0, 0.0};

        A[0][0] = 1000000000.0;
        A[0][1] = 1.0;
        A[1][0] = 1.0;
        A[1][1] = 1e-9;
        A[1][2] = 1.0;
        A[2][1] = 1.0;
        A[2][2] = 1e-9;

        solveLinearSystem(A, b, x, n);

        CHECK(x[0] == doctest::Approx(2.0));
        CHECK(x[1] == doctest::Approx(3.0));
        CHECK(x[2] == doctest::Approx(4.0));

        liberaMatriz(A, n);
    }

    SUBCASE("Sistema com valores negativos")
    {
        // Sistema:
        // -x + y - z = -6
        // 2x - 3y + z = 11
        // -x - y - 2z = -6
        // Solucao: x = 3, y = -1, z = 2

        lint_t n = 3;
        real_t **A = alocaMatrizInicializada(n, n, 0.0);
        real_t b[] = {-6.0, 11.0, -6.0};
        real_t x[] = {0.0, 0.0, 0.0};

        A[0][0] = -1.0;
        A[0][1] = 1.0;
        A[0][2] = -1.0;
        A[1][0] = 2.0;
        A[1][1] = -3.0;
        A[1][2] = 1.0;
        A[2][0] = -1.0;
        A[2][1] = -1.0;
        A[2][2] = -2.0;

        solveLinearSystem(A, b, x, n);

        CHECK(x[0] == doctest::Approx(3.0));
        CHECK(x[1] == doctest::Approx(-1.0));
        CHECK(x[2] == doctest::Approx(2.0));

        liberaMatriz(A, n);
    }

    SUBCASE("Nao escreve para alem dos limites do vetor x")
    {
        // Verifica se a funcao nao escreve para alem dos limites do vetor x

        lint_t n = 3;
        real_t **A = alocaMatrizInicializada(n, n, 0.0);
        real_t b[] = {8.0, -11.0, -3.0};
        real_t x_guarded[] = {1234.0, 0.0, 0.0, 0.0, 4321.0};
        real_t *x = &x_guarded[1];

        A[0][0] = 2.0;
        A[0][1] = 1.0;
        A[0][2] = -1.0;
        A[1][0] = -3.0;
        A[1][1] = -1.0;
        A[1][2] = 2.0;
        A[2][0] = -2.0;
        A[2][1] = 1.0;
        A[2][2] = 2.0;

        solveLinearSystem(A, b, x, n);

        CHECK(x_guarded[0] == doctest::Approx(1234.0));
        CHECK(x_guarded[4] == doctest::Approx(4321.0));

        liberaMatriz(A, n);
    }
}