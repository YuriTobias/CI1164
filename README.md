### Atividade 01

Desenvolvida por:

-   Izalorran Bonaldi (GRR20210582);
-   Yuri Junqueira Tobias (GRR20211767);

Data de entrega: 19/08/2023
Docente: Guilherme Alex Derenievicz

Especificades:

-   Apesar do enunciado propor a aplicação das funções m(x) e M(x) sobre o resultado das operações, nossa implementação aplica apenas a operação M(x) sobre o intervalo superior (utilizando a função nextafter()), deixando que o intervalo inferior seja definido automaticamente pela máquina conforme diretiva FE_DOWNWARD (definida através da função fesetround()), visando obter resultado correspondente ao obtido no exemplo fornecido pelo professor no enunciado.
-   A ULP é calculada através da diferença entre as representações inteiras dos intervalos superior e inferior, exceto quando os números possuem sinais diferentes, quando o cálculo é feito obtendo a soma dos números representáveis entre o valor negativo e zero e entre zero e o valor positivo, conforme sugerido pelo professor em encontro.
-   Nossa implementação encontra resultados de ULP inconsistentes para ULP entre valores especiais como inf e nan, levando ao estouro dos valores inteiros representáveis em int.
-   Nossa implementação encontra resultados imprecisos em operações que deveriam resultar em zero (casos como 2-2), obtendo resultados com ULP e erros relativos consideravelmente altos.
-   A implementação calcula a ULP através da diferença entre as representações inteiras dos intervalos superior e inferior menos um apenas em casos em que ambos possuam mesmo sinal. Caso contrário, o cálculo será a soma das ULPS entre o valor negativo e zero e entre zero e o valor positivo, conforme instruído pelo professor em encontro.
-   O Makefile gera um executável nomeado calc, vide não haver especificação de nomenclatura no enunciado.
-   Não foram adicionados casos de teste conforme não-necessidade apontada pelo professor em encontro.
