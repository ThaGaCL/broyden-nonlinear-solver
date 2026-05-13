#include "doctest.h"

extern "C" {
    #include "non_linear.h"
}

TEST_CASE("Calculo do metodo de Newton para o sistema nao linear de broyden")
{
    // Redireciona a saida do metodo de Newton para um arquivo nulo para evitar poluicao dos testes.
    FILE *devnull = fopen("/dev/null", "w");

    SUBCASE("Chute inicial ja esta na solucao")
    {
        // Com X(0) = {1.0, 1.0}:
        // F(X) = {0.0, 0.0}
        // J(X) = [ -1.0  -2.0 ]
        //        [ -1.0  -1.0 ]
        // Resolvendo J(X) * delta = -F(X), obtemos:
        // delta = {0.0, 0.0}
        // X(1) = X(0) + delta = {1.0, 1.0}.

        real_t X[] = {1.0, 1.0};

        newton(X, 1e-10, 10, 2, devnull);

        CHECK(X[0] == doctest::Approx(1.0));
        CHECK(X[1] == doctest::Approx(1.0));
    }

    SUBCASE("Converge a partir de um chute proximo")
    {
        // Com X(0) = {0.8, 1.2}:
        // F(X) = {-0.28, -0.08}
        // J(X) = [ -0.2  -2.0 ]
        //        [ -1.0  -1.8 ]
        // Resolvendo J(X) * delta = -F(X), obtemos:
        // delta = {0.2098, -0.1610}
        // X(1) = X(0) + delta = {1.0098, 1.0390}.

        real_t X[] = {0.8, 1.2};

        newton(X, 1e-8, 20, 2, devnull);

        CHECK(X[0] == doctest::Approx(1.0));
        CHECK(X[1] == doctest::Approx(1.0));
    }

    SUBCASE("Nao altera o vetor quando max_it e zero")
    {
        // Com X(0) = {0.5, -0.25}:
        // F(X) = {1.5, -1.375}
        // J(X) = [  1.0  -2.0 ]
        //        [ -1.0   4.0 ]
        // Como max_it = 0, a iteracao nao ocorre.
        // delta = {0.0, 0.0}
        // X(1) = X(0) + delta = {0.5, -0.25}.

        real_t X[] = {0.5, -0.25};

        newton(X, 1e-8, 0, 2, devnull);

        CHECK(X[0] == doctest::Approx(0.5));
        CHECK(X[1] == doctest::Approx(-0.25));
    }

    SUBCASE("Converge a partir de chute qualquer")
    {
        // Com X(0) = {1.5, 0.5}:
        // F(X) = {0.0, -0.5}
        // J(X) = [ -3.0  -2.0 ]
        //        [ -1.0   1.0 ]
        // Resolvendo J(X) * delta = -F(X), obtemos:
        // delta = {-0.2, 0.3}
        // X(1) = X(0) + delta = {1.3, 0.8}.

        real_t X[] = {1.5, 0.5};

        newton(X, 1e-10, 20, 2, devnull);

        CHECK(X[0] == doctest::Approx(1.0));
        CHECK(X[1] == doctest::Approx(1.0));
    }

    SUBCASE("Para imediatamente quando a norma inicial ja esta abaixo de epsilon")
    {
        // Com X(0) = {0.0, 0.0}:
        // F(X) = {1.0, 0.0}
        // J(X) = [  3.0  -2.0 ]
        //        [ -1.0   3.0 ]
        // Norma de F(X) = 1.0. Como 1.0 < epsilon (2.0), a iteracao para imediatamente.
        // delta = {0.0, 0.0}
        // X(1) = X(0) + delta = {0.0, 0.0}.

        real_t X[] = {0.0, 0.0};

        newton(X, 2.0, 10, 2, devnull);

        CHECK(X[0] == doctest::Approx(0.0));
        CHECK(X[1] == doctest::Approx(0.0));
    }

    SUBCASE("Executa uma iteracao em dimensao 3")
    {
        // Com X(0) = {0.0, 0.0, 0.0}:
        // F(X) = {1.0, 1.0, 0.0}
        // J(X) = [  3.0  -2.0   0.0 ]
        //        [ -1.0   3.0  -2.0 ]
        //        [  0.0  -1.0   3.0 ]
        // Resolvendo J(X) * delta = -F(X), obtemos:
        // delta = {-13/15, -4/5, -4/15}
        // X(1) = X(0) + delta = {-13/15, -4/5, -4/15}.

        real_t X[] = {0.0, 0.0, 0.0};

        newton(X, 1e-12, 1, 3, devnull);

        CHECK(X[0] == doctest::Approx(-13.0 / 15.0));
        CHECK(X[1] == doctest::Approx(-4.0 / 5.0));
        CHECK(X[2] == doctest::Approx(-4.0 / 15.0));
    }

    SUBCASE("Converge a partir de chute grande com muitas iteracoes")
    {
        // Com X(0) = {5.0, -3.0}:
        // F(X) = {-28.0, -32.0}
        // J(X) = [ -17.0  -2.0 ]
        //        [  -1.0  15.0 ]
        // Resolvendo J(X) * delta = -F(X), obtemos:
        // delta = {-1.8833, 2.0078}
        // X(1) = X(0) + delta = {3.1167, -0.9922}.

        real_t X[] = {5.0, -3.0};

        newton(X, 1e-10, 200, 2, devnull);

        CHECK(X[0] == doctest::Approx(1.0));
        CHECK(X[1] == doctest::Approx(1.0));
    }

    SUBCASE("Nao converge quando max_it e insuficiente")
    {
        // Com X(0) = {0.8, 1.2}:
        // F(X) = {-0.28, -0.08}
        // J(X) = [ -0.2  -2.0 ]
        //        [ -1.0  -1.8 ]
        // Resolvendo J(X) * delta = -F(X), obtemos:
        // delta = {0.2098, -0.1610}
        // X(1) = X(0) + delta = {1.0098, 1.0390}.
        // O laco encerra apos essa 1ª iteracao, antes de chegar a {1.0, 1.0}.

        real_t X[] = {0.8, 1.2};

        newton(X, 1e-12, 1, 2, devnull);

        CHECK(X[0] != doctest::Approx(1.0));
        CHECK(X[1] != doctest::Approx(1.0));
    }

    SUBCASE("Converge em 4 dimensoes a partir de chute proximo")
    {
        // Com X(0) = {0.9, 1.1, 1.05, 0.95}:
        // F(X) = {-0.12, -1.12, -1.055, -0.005}
        // J(X) = [ -0.6  -2.0   0.0   0.0 ]
        //        [ -1.0  -1.4  -2.0   0.0 ]
        //        [  0.0  -1.0  -1.2  -2.0 ]
        //        [  0.0   0.0  -1.0  -0.8 ]
        // Resolvendo J(X) * delta = -F(X), obtemos:
        // delta = {-21.506, 6.392, 5.719, -7.155}
        // X(1) = X(0) + delta = {-20.606, 7.492, 6.769, -6.205}.

        real_t X[] = {0.9, 1.1, 1.05, 0.95};

        newton(X, 1e-8, 200, 4, devnull);

        CHECK(X[0] == doctest::Approx(1.75995));
        CHECK(X[1] == doctest::Approx(0.042508));
        CHECK(X[2] == doctest::Approx(-0.318018));
        CHECK(X[3] == doctest::Approx(-0.0994169));
    }

    SUBCASE("Converge a partir de chute negativo")
    {
        // Com X(0) = {-1.0, -1.5}:
        // F(X) = {-1.0, -8.0}
        // J(X) = [  7.0  -2.0 ]
        //        [ -1.0   9.0 ]
        // Resolvendo J(X) * delta = -F(X), obtemos:
        // delta = {0.4098, 0.9344}
        // X(1) = X(0) + delta = {-0.5902, -0.5656}.

        real_t X[] = {-1.0, -1.5};

        newton(X, 1e-8, 100, 2, devnull);

        CHECK(X[0] == doctest::Approx(-0.329484));
        CHECK(X[1] == doctest::Approx(-0.102785));
    }

    SUBCASE("Converge em dimensao 5")
    {
        // Com X(0) = {1.0, 1.0, 1.0, 1.0, 1.0}:
        // O metodo executara multiplas iteracoes ate atingir a convergencia (||F(X)|| < epsilon).
        // Para provar a convergencia, aplicamos o vetor X final de volta na funcao geradora do sistema broyden().
        // O resultado esperado no vetor Fx gerado deve ser {0, 0, 0, 0, 0}.

        lint_t n = 5;
        real_t X[] = {1.0, 1.0, 1.0, 1.0, 1.0};

        // Permite ate 200 iteracoes para garantir que o metodo alcance a convergencia real
        newton(X, 1e-8, 200, n, devnull);

        // Aloca vetor para receber os resultados de F(X) e chama a funcao original
        real_t Fx[5];
        broyden(Fx, X, n);

        // Confirma que a avaliacao da funcao chegou na raiz (y = 0) dentro de uma margem de seguranca
        CHECK(Fx[0] == doctest::Approx(0.0));
        CHECK(Fx[1] == doctest::Approx(0.0));
        CHECK(Fx[2] == doctest::Approx(0.0));
        CHECK(Fx[3] == doctest::Approx(0.0));
        CHECK(Fx[4] == doctest::Approx(0.0));
    }

    // Fecha o arquivo nulo usado para suprimir a saida do metodo
    fclose(devnull);
}