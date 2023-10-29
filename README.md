### Atividade 04

Desenvolvida por:

-   Izalorran Bonaldi (GRR20210582);
-   Yuri Junqueira Tobias (GRR20211767);

Data de entrega: 28/10/2023
Docente: Guilherme Alex Derenievicz

Especificades:

-   Existem dois scripts: script.sh e extensiveScript.sh, o primeiro é utilizado para realizar testes unitários sem gerar gráficos, enquanto que o último executa o programa para todos os tamanhos de entrada fornecidos no enunciado do trabalho e gera os gráficos;
-   O programa foi executado em uma máquina que não pertence aos laboratórios do departamento. Isso dito, a variável LIKWID_HOME possui como rota /usr/local;
-   A saída do tempo em milisegundos segue o padrão indicado pela biblioteca utils.h;

Otimizações:

-   As otimizações feitas consistem basicamente em fazer unroll & jam + blocking tanto na função dada "MultMatVet", dando origem a função "MultMatVetUJB", e na função dada "MultMatMat", dando origem a função "MultMatMatUJB". Vale comentar também que em cada função existem dois laços extras para realizar as operações nas linhas e colunas que eventualmente possam ter ficado de fora por conta do fator de unroll.
-   Foi adicionado "restrict" antes do tipo dos parâmetros nas funções para evitar que o compilador tenha dúvidas a respeito da existência ou não de dependências.
