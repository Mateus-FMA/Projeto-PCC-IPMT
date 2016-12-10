# ipmt

Programa desenvolvido em C++ para compressão de textos em índices. A ferramenta também dá suporte
à busca de padrões em índices previamente criados.
Segundo projeto da disciplina de Processamento de Cadeias de Caracteres (IF767), lecionada
pelo Prof. Dr. Paulo Gustavo Soares Fonseca no período de 2016.2.

Alunos/Autores:
  - Mateus de Freitas Leite (mfl3@cin.ufpe.br)
  - Valdemir de Andrade Borges Júnior (vabj@cin.ufpe.br)

## Criando o executável

+ Na pasta ipmt, abra o terminal e execute o comando `make`.
+ Após a execução do `make`, uma pasta `bin/` deverá aparecer. Nela estará o executável da
ferramenta.

## Instruções de uso

Utilização:
`$ ipmt <mode> [options] pattern indexfile [indexfile ...]`

+ `<mode>` determina qual funcionalidade da ferramenta se deseja utilizar. Os modos implementados
são `index` (indexação) e `search` (busca).
+ `pattern` é o padrão de entrada a ser encontrado no texto. Argumento obrigatório apenas no modo
de busca; no modo de indexação, ele é interpretado como sendo o nome do arquivo de texto a ser
indexado.
+ `indexfile` é o arquivo de texto a ser comprimido, no modo de indexação, ou o arquivo de índice
que representa o texto comprimido a ser processado. Mais de um arquivo pode ser especificado.
*Wildcards* também podem ser utilizados.

Opções do modo de indexação:

  -c --compression    Especifica quais o algoritmo de compressão a ser utilizado para a criação do
                      arquivo de índice. As opções implementadas são as seguintes: "huffman" 
                      (Algoritmo de Huffman) e "lz78" (Algoritmo de Lempel-Ziv, 1978).
  -i --indexfile      Determina qual a estrutura de indexação para utilização no modo de busca da 
                      ferramenta.

Opções do modo de busca:

  -c --count          Imprime apenas o número de ocorrências do padrão no texto.
  -p --pattern        Se esta opção for escolhida, o argumento "pattern" será interpretado como um
                      arquivo contendo todos os padrões a serem procurados no texto.

