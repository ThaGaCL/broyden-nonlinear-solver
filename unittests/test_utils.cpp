#include "doctest.h"

extern "C" {
    #include "utils.h"
}

#include <string>

TEST_CASE("Funcoes utilitarias: alocacao, markerName e timestamp")
{
    SUBCASE("alocaVetor e liberaVetor")
    {
        real_t *v = alocaVetor(5);
        CHECK(v != nullptr);
        if (v) {
            for (lint_t i = 0; i < 5; ++i) v[i] = (real_t)i + 0.5;
            liberaVetor(v);
        }

        CHECK(alocaVetor(0) == nullptr);
        CHECK(alocaVetor(-3) == nullptr);
    }

    SUBCASE("alocaVetorInicializado")
    {
        real_t *v = alocaVetorInicializado(4, 3.14159);
        CHECK(v != nullptr);
        if (v) {
            for (lint_t i = 0; i < 4; ++i) CHECK(v[i] == doctest::Approx(3.14159));
            liberaVetor(v);
        }
    }

    SUBCASE("alocaMatriz e liberaMatriz")
    {
        real_t **m = alocaMatriz(3, 2);
        CHECK(m != nullptr);
        if (m) {
            for (lint_t i = 0; i < 3; ++i) {
                CHECK(m[i] != nullptr);
                for (lint_t j = 0; j < 2; ++j) m[i][j] = (real_t)(i * 10 + j);
            }
            liberaMatriz(m, 3);
        }

        CHECK(alocaMatriz(0, 2) == nullptr);
        CHECK(alocaMatriz(2, 0) == nullptr);
    }

    SUBCASE("alocaMatrizInicializada")
    {
        real_t **m = alocaMatrizInicializada(2, 3, -1.0);
        CHECK(m != nullptr);
        if (m) {
            for (lint_t i = 0; i < 2; ++i)
                for (lint_t j = 0; j < 3; ++j)
                    CHECK(m[i][j] == doctest::Approx(-1.0));

            liberaMatriz(m, 2);
        }
    }

    SUBCASE("markerName")
    {
        string_t s = markerName((char*)"TEST", 42);
        CHECK(s != nullptr);
        if (s) {
            std::string ss(s);
            CHECK(ss == "TEST_42");
            free(s);
        }
    }

    SUBCASE("timestamp monotonia")
    {
        // Amostras simples: verifica que timestamp() e nao-negativo
        // e que uma segunda amostra nao e maior que a primeira.
        rtime_t t1 = timestamp();
        rtime_t t2 = timestamp();

        CHECK(t1 >= 0.0);
        CHECK(t2 >= t1);
    }
}
