POSIXLAB
SISTEMAS HARDWARE-SOFTWARE
INSPER - ENGENHARIA DE COMPUTAÇÃO

ERIC FERNANDO OTOFUJI ABRANTES
2020



1. INTRODUÇÃO
Implementação de conjunto de testes baseados no exemplo original, aprimorando os headers para expandir o conjunto de funcionalidades das rotinas de testes unitários em C, de acordo com os requisitos emitidos pelo enunciado.

No item 2. IMPLEMENTAÇÃO DE REQUISITOS, será detalhada a implementação da expansão de requisitos dos arquivos headers modificados, disponíveis em src/mintest/.

No item 3. IMPLEMENTAÇÃO DE NOVOS TESTES, será detalhada a implementação de testes escritos pelo próprio autor no arquivo src/testes.c. O executável testes deve estar disponível no mesmo diretório. Caso não, pode ser compilado por meio do comando: 
    gcc -Wall -pedantic -std=gnu99 -Og -o testes testes.c

Os requisitos implementados foram:
    1. Execução de testes de maneira isolada
    2. Execução dos testes em paralelo
    3. Execução de um teste específico
    4. Tempo limite de execução dos testes
    5. Relatório de erros dos testes que falharam
    6. [NÃO IMPLEMENTADO]
    7. [NÃO IMPLEMENTADO]
    8. [NÃO IMPLEMENTADO]
    9. [NÃO IMPLEMENTADO]

Os itens 1-3 são requisitos do C, 4-6 do B e 7-9 do A. 

Além disso, funcionalidades extras foram implementadas:
    1. Status dos testes são impressos em cores
    2. [NÃO IMPLEMENTADO]

Os itens 1-2 são bônus. 


2. IMPLEMENTAÇÃO DE REQUISITOS 
Os requisitos foram implementados em três arquivos header: timer.h, macros.h e runner.h, disponíveis em src/mintest e foram desenhados à parte testes indepententes em src/testes.c. Os testes serão descritos na sessão 3. Esta sessão se limita à discussão da implementação dos headers em src/mintest.

Cabe ressaltar que esta sessão explica somente as modificações feitas nos arquivos originais e uma visão geral do novo arquivo timer.h não previsto anteriormente. O que já estava implementado antes no conjunto exemplo não será comentado.

    2.1. runner.h
    Adição de funções de suporte para alterar as cores das saídas de terminal: void white(), void green(), void red(), void ambar() e void blue(). 

    Separação da verificação se o teste foi bem sucedido por meio da nova função int run_all_test(int i), apenas para fins de organização. Retorna 1 se a função de teste retorna 0 (se todos os testes passarem), retorna 0 se a função de teste retorna 1 (se pelo menos um teste passou) e retorna zero em qualquer outro caso (se todos os testes falharem). O print em caso de sucesso não é mais feito aqui e será detalhado na sessão 2.2.

    Adição de argumentos para permitir executar testes isoladamente int main(int argc, char *argv[])

    Verificação do tamanho dos argumentos passados. Se argc == 1, executa todos os testes. Senão, somente o teste desejado.

    Roda testes em paralelo por meio de processos filhos e processos netos. A justificativa para a existẽncia de processos netos será dada na sessão 2.3. A justificativa para a saída da chamada da função que cria o processo neto ser negada também será dada na sessão 2.3 (e veremos que o processo filho também nega a saída do processo neto por conta das convenções de C de EXIT_FAILURE e EXIT_SUCCESS, e devido ao comportamento que esperamos no PosixLab).

    Processo pai verifica se processo neto foi bem sucedido na verificação de run_all_test. Caso tenha sido, retorna child_pass_count e soma 1 ao contador de quantas funções foram bem sucedidas para todos os testes.

    2.2. macros.h
    Adição de funções de suporte para alterar as cores das saídas de terminal: void fire(), void end_of_error() e void lime(). O motivo sincero de terem sido criadas funções de nome diferentes com a mesma função das funções de cores mancionadas na sessão 2.1 é gambiarra para resolver bug. 
    
    A mesma gambiarra foi feita na sessão 2.3 com as funções void flame() e void extinguiser() pelo mesmo motivo. Isto não será mencionado novamente em 2.3 porque já foi dito aqui.
    
    Modificação na macro test_assert para usar o código de cores e imprimir tanto status [FAIL] quanto o status [PASS], bem como o motivo. O print de [PASS] foi movido de runner.h para cá. 


    2.3. timer.h
    Implementação baseada em https://lowlevelbits.org/handling-timeouts-in-child-processes/

    Basicamente, o conteúdo de timer.h é a implementação sugerida pelo autor, mas com algumas modificações que foram feitas na proposta original para as necessidades do PosixLab. A explicação da implementação pode ser vista no link indicado acima. Aqui, serão detalhadas as modificações feitas em cima desse header.

    A função que cria os processos de teste e de contagem de tempo teve a estrutura alterada para conversar melhor com runner.h, sendo alterada para: 
        int watchdog_worker_timer(work_t work, long long timeout, int i)
    que cria processos filhos. 
    
    Todavia, é lembrado que a chamada para watchdog_worker_timer é feita num processo filho em runner.h. Logo, aqui na verdade são criados dois processos netos. 

    Tentei fazer antes da forma mais elegante que seria que tudo isso ocorresse nos próprios processos filhos, sem criar processos netos. Porém, encontrei problemas com a paralelização dos processos, de forma que o comportamento dos filhos estava estranho e não eram executados os testes como deveriam. Por fim, achei que esta implementação, embora menos elegante, é mais robusta quando existem vários testes rodando em paralelo, evitando que um teste interfira no outro, que haja duplicação, que algum teste nunca termine ou que algum teste nunca seja executado. 
    
    Nessa estrutura, cria-se uma árvore de chamadas de três andares, o que durante a implementação mostrou-se como a forma mais robusta de atingir os requisitos de implementação. O processo pai cria um processo filho que é responsável por executar os testes. Devido ao requisito de timeout, o processo filho se divide em dois processos netos. Um dos processos netos executa o teste e outro executa um timer. Dentre esses dois processos netos, aquele que acabar primeiro, mata o outro. É exatamente por conta desse comportamento de um matar o outro que as inconsistências descritas no parágrafo anterior ocorriam, porque quem acaba primeiro matava o outro independentemente de ser timer ou teste, ou o outro teste que não era o que o timer deveria verificar. Já estruturando em processos pai, filhos e netos, esse problema foi erradicado e as chamadas ficaram robustas e com comportamento previsível. 

    Se o primeiro processo a acabar for o processo que de fato executa o teste, o processo neto mata o outro processo neto (timer) e se reporta ao processo filho, enviando o status de run_all_test. O processo filho então reporta para o processo pai o resultado do status que foi recebido. 

    Se o primeiro processo a acabar for o processo timer, este mata o processo do teste e se reporta ao processo filho com EXIT_FAILURE. Como o processo pai nega o status do filho que por sua vez nega o status do neto, o dado chega corretamente no processo pai.


    



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

    PASS TEST SLOW ASSERT: Verifica se 1 != 0 após aguardar um segundo. Espera-se que o status de saída seja [PASS] e a função consiga retornar zero. 