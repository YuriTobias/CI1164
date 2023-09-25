### Atividade 03

Desenvolvida por:

- Izalorran Bonaldi (GRR20210582);
- Yuri Junqueira Tobias (GRR20211767);

Data de entrega: 25/09/2023
Docente: Guilherme Alex Derenievicz

Especificades:

- O programa foi executado e testado utilizando o script;
- O programa foi executado em uma máquina que não pertence aos laboratórios do departamento. Isso dito, a variável LIKWID_HOME possui como rota /usr/local;
- O script fica aguardando uma entrada no formato especificado no enunciado que pode ser digitada ou direcionada (receber um arquivo);
- A saída do tempo em milisegundos segue o padrão indicado pela biblioteca utils.h;
- No caso de erro por conta de xe não ser pertencente ao intervalo, o programa exibe uma mensagem de erro pré definida ("xe is out of range"), além de mensagens de erro relacionadas ao likwid que são esperadas, em função da não utilização da Marker API devido ao encerramento precoce da aplicação;
