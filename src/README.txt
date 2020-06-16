POSIXLAB
SISTEMAS HARDWARE-SOFTWARE
INSPER - ENGENHARIA DE COMPUTAÇÃO

ERIC FERNANDO OTOFUJI ABRANTES
2020



1. INTRODUÇÃO
Implementação de conjunto de testes baseadas no exemplo original, aprimorando os headers para expandir o conjunto de funcionalidades das rotinas de testes unitários em C, de acordo com os requisitos emitidos pelo enunciado.

No item 2. IMPLEMENTAÇÃO DE REQUISITOS, será detalhada a implementação da expansão de requisitos dos arquivos headers modificados, disponíveis em src/mintest/.

No item 3. IMPLEMENTAÇÃO DE NOVOS TESTES, será detalhada a implementação de testes escritos pelo próprio autor no arquivo src/testes.c. O executável testes deve estar disponível no mesmo diretório. Caso não, pode ser compilado por meio do comando gcc -Wall -pedantic -std=gnu99 -Og -o testes testes.c.

Os requisitos implementados foram:
    1. Execução de testes de maneira isolada
    2. Execução dos testes em paralelo
    3. Execução de um teste específico
    4. Tempo limite de execução dos testes
    5. Relatório de erros dos testes que falharam
    6. PENDENTE
    7. PENDENTE
    8. PENDENTE

Além disso, funcionalidades extras foram implementadas:
    1. Status dos testes são impressos em cores
    2. PENDENTE



2. IMPLEMENTAÇÃO DE REQUISITOS 
Lorem ipsum dolor sit amet



3. IMPLEMENTAÇÃO DE NOVOS TESTES
Alguns testes foram desenhados para propositalmente dar erro e outros testes foram desenhados para dar certo. Nos prints no terminal, os testes cujo nome iniciam em PASS TEST devem passar com status [PASS] e serem exibidos em verde. Os testes cujo nome iniciam em FAULT TEST devem falhar com status [FAIL] ou [TIME], conforme aplicável, e serem exibidos em vermelho. Foram implementados sete testes, sendo que alguns possuem mais de um assert dentro deles, e o esperado é que 3/7 testes sejam bem sucedidos.

    3.1. CAUSE ERRO E TERMINE COM FALHA DE SEGMENTAÇÃO
    FAULT TEST SEGMENTATION: A função int test1() diponível no arquivo src/testes.c causa erro e termina com falha de segmentação. A falha ocorre por tentar acessar um endereço de memória inválido para a realização de um teste. O status deve ser [ERRO].

    3.2. CAUSE ERRO E TERMINE COM DIVISÃO POR ZERO
    FAULT TEST DIVISION BY ZERO: A função int test2() disponível no arquivo src/testes.c causa erro ao executar divisão por zero para ver se a condição 1/0 == 5 é verdadeira. O status deve ser [ERRO].

    3.3. FIQUE EM LOOP INFINITO
    FAULT TEST INFINITE LOOP: A função int test3() disponível no arquivo src/testes.c entra em loop infinito por meio de while(1) {t += 1;}. O nome da função não chega a aparecer no terminal.  Espera-se que a implementação descrita na sessão 2 termine esta execução forçadamente com o status [TIME].

    3.4. FAÇA MUITO TRABALHO, MAS EVENTUALMENTE ACABE
    PASS TEST LONG TASK: A função int test4() disponível no arquivo src/testes.c executa dois loops for sem propósito algum, somando em duas variáveis 1 a cada passo. Ao final, é verificada a condição se a == b. Espera-se que o status de saída seja [PASS].

    3.5. TENHA ASSERTS QUE FALHAM E PASSAM NO MESMO TESTE
    A função int test5() disponível no arquivo src/testes.c executa dois testes.

    PASS TEST ASSERT: Verifica se 1 == 1. Espera-se que o status de saída seja [PASS]

    FAULT TEST ASSERT: Verifica se 0 == 1. Espera-se que o status de saída seja [FAIL]

    Espera-se que a função retorne 1 e o resultado positivo de PASS TEST ASSERT não seja contabilizado no resultado final de testes bem sucedidos pass_count += 0.

    3.6. TENHA TESTES QUE FAÇAM MUITO PRINTS 
    PASS TEST DOZEN PRINT: A função int test6() disponível no arquivo src/testes.c realiza uma dúzia de prints com o nome do teste. Espera-se que o resultado de saída seja [PASS].

    3.7. TENHA TESTES QUE SEJAM RÁPIDOS E TESTES QUE SEJAM LENTOS
    A função int test7() disponível no arquivo src/testes.c tem dois testes.

    PASS TEST FAST ASSERT: Verifica se 0 == 0. Espera-se que o status de saída seja [PASS].

    PASS TEST SLOW ASSERTÇ Verifica se 1 != 0 após aguardar um segundo. Espera-se que o status de saída seja [PASS] e a função consiga retornar zero. 