O programa abaixo recebe um tableau em um arquivo texto
e retorna sua solucao.

O programa "simplex. c" retorna como saida dois arquivos: 

- conclusao.txt, de onde e impresso o valor objetivo, vetor
solucao (se tiver) e certificados
- log.txt, que registra o passo-a-passo do algoritmo

O programa "simplexGrafico. c" retorna como saida apenas
o arquivo conclusao.txt. Sua funcionalidade e identica a
do "simplex.c", porem ele imprime a matriz no terminal do
linux ao inves de imprimi-la em um arquivo. Os tableaus 
sao impressos alinhados e em cores, indicando: 
    - coluna pivotal, de verde
    - linha pivotal, de verde
    - pivot, de azul
    - zeros, de vermelho
    - colunas da base, marcadas com uma setinha azul
      na parte de baixo da matriz
As matrizes sao impressas com precisao de 0 casas decimais,
para nao estourar o limite horizontal da janela (se ainda 
assim isso ocorrer, diminua o zoom do terminal).

IMPORTANTE: O arquivo "simplexGrafico.c" nao deve ser
tratado como o trabalho principal. O arquivo principal 
eh o "simplex.c". "simplexGrafico.c" tem finalidade
exclusivamente educativa e depuratoria, e nao deve ser 
tratado como outra coisa. 

Boa Sorte!

PS: Todos esses programas podem ser livemente utilizados e modificados.
