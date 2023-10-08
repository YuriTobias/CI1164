### Trabalho 01

Desenvolvida por:

- Izalorran Bonaldi (GRR20210582);
- Yuri Junqueira Tobias (GRR20211767);

Data de entrega: 08/10/2023
Docente: Guilherme Alex Derenievicz

Especificidades:

- O programa foi executado e testado utilizando o script;
- O programa foi executado em uma máquina que não pertence aos laboratórios do departamento. Em função disso, a variável LIKWID_HOME possui como rota /usr/local;
- O script fica aguardando uma entrada no formato especificado no enunciado que pode ser digitada ou direcionada (receber um arquivo);
- O programa lê os pontos como uma string e espera uma entrada no formato da fornecida em teste.in no enunciado do trabalho. Em função disso, NÃO funciona caso x e y sejam passados em linhas diferentes;
- A saída do tempo em milisegundos segue o padrão indicado pela biblioteca utils.h;
- Toda a documentação de parâmetros e retornos das funções está contida nos arquivos system_ops.h e interval_ops.h;
- O programa não funcionará para casos em que o grau do polinômio seja igual ou menor ao número de pontos, pois a triangularização resultará em sistema possível indeterminado, incorrendo em divisão por zero na retrossubstituição. A opção por não retornar erro neste caso é deliberada vide não haver restrição no enunciado;
- Os arredondamentos foram alterados em relação ao EP01 para utilizarem a função fesetround em detrimento de nextafter, portanto, números representáveis poderão estar em intervalos contendo apenas eles;
- A inserção de números inválidos incorrerá em sua conversão para 0, sujeita às particularidades da função atof;

Módulos:
- interval_ops: contém as funções e estruturas relacionadas à alocações, inicializações e cálculos intervalares;
- system_ops: conteḿ as funções relacionadas à construção e solução do sistema linear, incluindo o método dos mínimos quadrados, a eliminação gaussiana com pivoteamento parcial e a retrossubstituição, tal como outras funções periférias relacionadas. Inclui também funções relacionadas à impressão de resultados e estruturas dos sistemas construídos;
- utils: fornecido pelos docentes;