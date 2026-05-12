# Objetivo

O objetivo deste trabalho é implementar um programa de forma otimizada para resolver Sistemas Não-Lineares de Broyden (SNLB) usando o Método de Newton e avaliar o seu desempenho.

Deverão ser entregues:

    - os códigos-fonte do programa implementado e
    - um relatório no formato de artigo científico com a análise de desempenho e justificativas das otimizações empregadas.

# Especificação do Programa

O programa broyden a ser desenvolvido deverá ler da entrada padrão os seguintes dados:

    - **N**: dimensão do SNLB
    - **x0**: valor inicial das variáveis (um único, igual para todas as variáveis)
    - **epsilon**: tolerância a ser considerada nos critérios de parada do Método de Newton
    - **MAX**: número máximo de iterações do Método de Newton

O programa deverá gerar como saída todas as iterações obtidas pelo Método de Newton, separadas pelo caractere '#'. Para cada iteração, imprimir cada incógnita em uma linha.

Além disso, ao final deve constar:

    - O tempo de execução total do Método de Newton;
    - A soma dos tempos de execução de todas as computações de matrizes Jacobianas efetuadas.
    - A soma dos tempos de execução de todas as resoluções de sistemas lineares efetuados pelo método.

Portanto,  a saída deve ter o formato abaixo:
```
x1 = <x1 na iteração 0>
...
xn = <xn na iteração 0>
#
...
#
x1 = <x1 na última iteração>
...
xn = <xn na última iteração>
###########
# Tempo Total: <tempo total do Método de Newton>
# Tempo Jacobiana: <tempo gasto na construção das Jacobianas>
# Tempo SL: <tempo gasto na resolução dos sistemas lineares>
###########
```

Os tempos devem ser calculados em milisegundos, utilizando-se a função **timestamp()** especificada nos módulos **utils.\***

Você deve deixar seu código modular, com funções e arquivos específicos para implementar cada parte do programa. O programa principal deve estar em um módulo separado dos demais, contendo apenas a função main().

As funções/código-fonte devem conter comentários adequados descrevendo cada função.

O arquivo Makefile deve possuir as regras necessárias para compilar os módulos individualmente e gerar o programa executável. As seguintes regras devem existir OBRIGATORIAMENTE:

    - **all**: compila e produz um executável chamado broyden
    - **clean**: remove todos os arquivos temporários e os arquivos gerados pela compilação do programa (*.o, executável, etc.)

O executável broyden deve ser invocado da seguinte forma:

```
./broyden -o <arquivo_saida>
```

Onde:

    - **-o <arquivo_saida>**: parâmetro opcional no qual arquivo_saida é o caminho completo para o arquivo que vai conter a solução. Caso este parâmetro não esteja especificado, os resultados devem ser enviados para a Saída Padrão (stdout).

# Otimização de Código

Você deve aplicar técnicas de melhoria de desempenho estudadas em aula ao seu programa e explicar no relatório quais otimizações foram implementadas e em quais funções ou trechos de códigos elas foram aplicadas, justificando por que estas modificações melhoram seu desempenho.

É aconselhável que conste no relatório análises comparativas de versões com e sem otimização na forma de gráficos de linhas (ver mais na seção Análise de Desempenho abaixo).

Alguns pontos que devem ser observados na implementação:

    - Qual método aplicar na resolução dos sistemas lineares internos?
    - Qual estrutura de dados utilizar para representar as matrizes e/ou vetores?
    - Como calcular a matriz Jacobiana a cada iteração do método?

# Análise de Desempenho

O relatório no formato de artigo científico deve apresentar uma análise de desempenho usando gráficos de linhas gerados a partir de métricas do LIKWID. Os seguintes trechos devem ser analisados:

    - Aplicação do Método de Newton (execução total);
    - Cálculo das matrizes Jacobianas;
    - Resolução dos Sistemas Lineares internos;

Para analisar o desempenho, você deve efetuar uma série de testes.

    - Cada teste deve ser reportado sob a forma de 3 (três) gráficos de linhas, um gráfico para cada trecho acima. As linhas do gráfico devem ter cores distintas para diferentes versões do programa (por exemplo, versão com e sem otimização)
    - No eixo das abcissas os gráficos representam o tamanho do SNLB (N). Cada teste deve ser executado para os seguintes tamanhos de matriz:  N={32, 64, 128, 256, 512, 1000, 2000, 4000, 8000, 9000, 10000, 20000};
    - Os gráficos devem ser apresentados com o eixo das abscissas (N) em escala logarítmica.
    - O número máximo de iterações em todos os testes, para todos tamanhos de matrizes, deve ser 25 (vinte e cinco), enquanto o epsilon deve ser 0 (zero). Isso vai permitir comparar diferentes versões do programa.
    Cada gráfico deve ser explicado e você deve demonstrar que consegue entender o que está reportado nele;

Os seguintes testes devem ser executados:

    - **Tempo de execução**: mostra o tempo médio do cálculo dos trechos, ambos em ms (utilize a função "timestamp()" da biblioteca utils para medir o tempo). Neste gráfico, o eixo das ordenadas (tempo) deve também estar em escala logarítmica;
    - **Banda de Memória**: utilizar o grupo MEM do LIKWID, e apresentar o resultado de "Memory bandwidth [MBytes/s]"; Caso não tenha o grupo MEM, utilize o grupo L3.
    - **Cache miss L2**: utilizar o grupo L2CACHE do LIKWID, e apresentar o resultado de "data cache miss ratio". Caso não tenha o cache miss da L2, utilize o cache miss da L1 (grupo L1CACHE ou CACHE do LIKWID).
    - **Operações aritméticas**: utilizar o grupo FLOPS_DP e FLOPS_AVX do LIKWID, e apresentar o resultado de "MFLOP/s".

Portanto, no total 12 (doze) gráficos deverão constar no relatório (4 gráficos para cada trecho analisado, sendo cada um deles referente a um teste do LIKWID acima).

É imprescindível que sejam respeitadas as seguintes condições:

    - Os códigos devem ser compilados com GCC e as opções:  `-O3   -march=native  -mavx   -fopt-info-vec;`
    - Os códigos devem ser compilados na mesma máquina utilizada para os testes.
    - Os testes devem utilizar os mesmos parâmetros e em igualdade de condições;
    - Os códigos devem ser instrumentados com a biblioteca do LIKWID para que se possa separar o cálculo de cada trecho, ou seja, você deve criar um marcador LIKWID para cada trecho;
    - Você pode escolher um computador de sua preferência, desde que possua os contadores LIKWID especificados. Não utilize as servidoras de processamento do DInf que tenham uso compartilhado. Elas podem ser máquinas virtuais e o compartilhamento impede medidas de desempenho. Em caso de dúvida, consulte o professor.
    - Você deve apresentar a arquitetura do processador utilizado nos testes no seu texto. Estas informações podem ser obtidas com o comando "LIKWID-topology -g -c".

 
# Entrega

O trabalho deve ser desenvolvido por um grupo composto de no máximo DOIS alunos regularmente matriculados na disciplina, com exceção dos alunos do PPGInf, que devem desenvolver o trabalho individualmente.

Cada grupo deve entregar via MOODLE um pacote de software completo contendo os fontes em linguagem C e o relatório . O pacote deve ser arquivado e compactado com zip ou tar, em um arquivo chamado login1.<ext>[1](se grupo com 1 membro) ou login1-login2.<ext> (se grupo com 2 membros), onde login1 e login2 são os logins (nos sistemas do DINF) dos alunos que compõem o grupo.

O pacote deve ter a seguinte estrutura de diretório e arquivos:

    - ./login1-login2/: diretório principal.
    - ./login1-login2/Makefile: o comando make deve ser capaz de gerar o executável.
    - ./login1-login2/*.{c,h,sh,py,pl}: códigos-fonte e scripts de processamento de dados do LIKWID e geração de gráficos (podem estar em sub-diretórios).
    - ./login1-login2/relatorio-login1-login2.pdf: relatório em formato de artigo científico.

O prazo final para a entrega deste trabalho é dia  14 de junho de 2026, 23:59:00h, IMPRETERIVELMENTE.
Avaliação

APENAS OS TRABALHOS QUE FUNCIONAREM SERÃO CORRIGIDOS. Se o trabalho não compilar ou acusar falha de segmentação (Segmentation fault) prematura durante os testes realizados pelo professor (sem que qualquer operação se efetue a contento) ou não produzir uma solução correta para o SNLB, será avaliado com NOTA 0 (ZERO).

Também receberão NOTA 0 (ZERO) trabalhos plagiados de qualquer fonte, e/ou com códigos idênticos ou similares. Além disso, apenas trabalhos entregues no prazo marcado receberão nota.

DEFESA: A defesa do trabalho será oral, e definirá a NOTA INDIVIDUAL de cada membro da equipe, de acordo com seu conhecimento a respeito do trabalho.
Uso de IA

O uso de IA (como assistentes de código e texto) pode comprometer o principal objetivo da atividade: desenvolver sua autonomia como programador e sua capacidade de análise científica.

A IA não deve ser utilizada para gerar soluções completas, código final, análises ou redações integrais do relatório. Mesmo seu uso para apoio pontual (por exemplo, para esclarecer conceitos gerais ou revisar texto) deve ser devidamente compreendido pelo aluno e explicitamente declarado no relatório.

Ressalta-se ainda que o uso indevido pode configurar violação de integridade acadêmica, além de possíveis questões legais relacionadas a direitos autorais. Os alunos são integralmente responsáveis pelo conteúdo entregue.