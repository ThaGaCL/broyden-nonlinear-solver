#include "doctest.h"

extern "C" {
    #include "non_linear.h"
}

TEST_CASE("Calculo do metodo de Newton para o sistema nao linear de broyden")
{
    // Redireciona a saída do metodo de Newton para um arquivo nulo para evitar poluição dos testes.
    FILE *devnull = fopen("/dev/null", "w");

    SUBCASE("Chute inicial ja esta na solucao")
    {
        // x = {1.0, 1.0}
        // f_0(x) = -2(1)^2 + 3(1) - 2(1) + 1 = 0
        // f_1(x) = -2(1)^2 + 3(1) - 1 = 0
        // Como ||F(X)|| = 0, o metodo deve retornar sem alterar X.

        real_t X[] = {1.0, 1.0};

        newton(X, 1e-10, 10, 2, devnull);

        CHECK(X[0] == doctest::Approx(1.0));
        CHECK(X[1] == doctest::Approx(1.0));
    }

    SUBCASE("Converge a partir de um chute proximo")
    {
        // Chute inicial: x = {0.8, 1.2}
        // O metodo de Newton deve convergir para a raiz {1.0, 1.0}.
        // Depois da convergencia, a norma de F(X) deve ser muito pequena.

        real_t X[] = {0.8, 1.2};

        newton(X, 1e-8, 20, 2, devnull);

        CHECK(X[0] == doctest::Approx(1.0));
        CHECK(X[1] == doctest::Approx(1.0));
    }

    SUBCASE("Nao altera o vetor quando max_it e zero")
    {
        // Com max_it = 0, o corpo do metodo nao executa.
        // O vetor deve permanecer com os valores iniciais.

        real_t X[] = {0.5, -0.25};

        newton(X, 1e-8, 0, 2, devnull);

        CHECK(X[0] == doctest::Approx(0.5));
        CHECK(X[1] == doctest::Approx(-0.25));
    }

    SUBCASE("Converge a partir de chute qualquer")
    {
        // Chute inicial: x = {1.5, 0.5}
        // O metodo de Newton deve convergir para a solucao {1.0, 1.0}.

        real_t X[] = {1.5, 0.5};

        newton(X, 1e-10, 20, 2, devnull);

        CHECK(X[0] == doctest::Approx(1.0));
        CHECK(X[1] == doctest::Approx(1.0));
    }

    SUBCASE("Para imediatamente quando a norma inicial ja esta abaixo de epsilon")
    {
        // Em X = {0.0, 0.0}, temos F(X) = {1.0, 1.0} e ||F(X)|| = 1.0.
        // Como epsilon = 2.0, o metodo deve parar antes de atualizar X.

        real_t X[] = {0.0, 0.0};

        newton(X, 2.0, 10, 2, devnull);

        CHECK(X[0] == doctest::Approx(0.0));
        CHECK(X[1] == doctest::Approx(0.0));
    }

    SUBCASE("Executa uma iteracao em dimensao 3")
    {
        // Com X(0) = {0, 0, 0}:
        // F(X) = {1, 1, 0}
        // J(X) = [ 3  -2   0 ]
        //        [ -1  3  -2 ]
        //        [ 0  -1   3 ]
        // Resolvendo J(X) * delta = -F(X), obtemos:
        // delta = {-13/15, -4/5, -4/15}
        // X(1) = X(0) + delta.

        real_t X[] = {0.0, 0.0, 0.0};

        newton(X, 1e-12, 1, 3, devnull);

        CHECK(X[0] == doctest::Approx(-13.0 / 15.0));
        CHECK(X[1] == doctest::Approx(-4.0 / 5.0));
        CHECK(X[2] == doctest::Approx(-4.0 / 15.0));
    }

    SUBCASE("Converge a partir de chute grande com muitas iteracoes")
    {
        // Chute inicial grande: x = {5.0, -3.0}
        // Requer mais iteracoes para aproximar a raiz {1.0, 1.0}.

        real_t X[] = {5.0, -3.0};

        newton(X, 1e-10, 200, 2, devnull);

        CHECK(X[0] == doctest::Approx(1.0));
        CHECK(X[1] == doctest::Approx(1.0));
    }

    SUBCASE("Nao converge quando max_it e insuficiente")
    {
        // Chute proximo, mas apenas 1 iteracao permitida.

        real_t X[] = {0.8, 1.2};

        newton(X, 1e-12, 1, 2, devnull);

        CHECK(X[0] != doctest::Approx(1.0));
        CHECK(X[1] != doctest::Approx(1.0));
    }

    SUBCASE("Converge em 4 dimensoes a partir de chute proximo")
    {
        // Chute inicial próximo da solução generalizada (valores próximos a 1).

        real_t X[] = {0.9, 1.1, 1.05, 0.95};

        newton(X, 1e-8, 200, 4, devnull);

        CHECK(X[0] == doctest::Approx(1.0));
        CHECK(X[1] == doctest::Approx(1.0));
        CHECK(X[2] == doctest::Approx(1.0));
        CHECK(X[3] == doctest::Approx(1.0));
    }

    SUBCASE("Converge a partir de chute negativo")
    {
        // Chute inicial com valores negativos; deve convergir para {1,1}.

        real_t X[] = {-1.0, -1.5};

        newton(X, 1e-8, 100, 2, devnull);

        CHECK(X[0] == doctest::Approx(1.0));
        CHECK(X[1] == doctest::Approx(1.0));
    }

    // Fecha o arquivo nulo usado para suprimir a saída do método
    fclose(devnull);
}