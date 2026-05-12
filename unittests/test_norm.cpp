#include "doctest.h"

extern "C" {
    #include "non_linear.h"
}

TEST_CASE("Calculo da norma pelo maximo de um vetor")
{
    SUBCASE("Vetor com valores positivos e negativos")
    {
        double values[] = {-3.0, 2.5, -4.5, 1.0};
        CHECK(norm(values, 4) == doctest::Approx(4.5));
    }

    SUBCASE("Vetor apenas com valores negativos")
    {
        double values[] = {-0.25, -10.0, -2.0};
        CHECK(norm(values, 3) == doctest::Approx(10.0));
    }

    SUBCASE("Vetor com um único elemento negativo")
    {
        double values[] = {-7.75};
        CHECK(norm(values, 1) == doctest::Approx(7.75));
    }

    SUBCASE("Vetor apenas com valores positivos")
    {
        double values[] = {1.2, 5.5, 3.14};
        CHECK(norm(values, 3) == doctest::Approx(5.5));
    }

    SUBCASE("Vetor contendo apenas zeros")
    {
        double values[] = {0.0, 0.0, 0.0, 0.0};
        CHECK(norm(values, 4) == doctest::Approx(0.0));
    }
    
    SUBCASE("Vetor com números decimais muito pequenos")
    {
        double values[] = {0.0001, -0.0005, 0.0002};
        CHECK(norm(values, 3) == doctest::Approx(0.0005));
    }
}