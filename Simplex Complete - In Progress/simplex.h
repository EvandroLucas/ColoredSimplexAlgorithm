/*

TRABALHO PRATICO DE PESQUISA OPERACIONAL
ASSUNTO: O ALGORITMO SIMPLEX
Professor: Gabriel Coutinho
Aluno: Evandro Lucas Figueiredo Teixeira
14/05/2018

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
sao impressos alinhas e em cores, indicando: 
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

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "color.h"

#ifndef SIMPL_H
#define SIMPL_H

FILE *registra;


typedef struct fracao{
    double valor;
    double num;
    double den;
}fracao;

int engatilhaSimplex(int,int,int,double**,bool*);

void imprime_tableau_terminal(int m, int n, double** tableau, int lp, int cp, bool* base){
    int i = 0;
    int j = 0;
    int k = 0;
    double aux = 0;
    double aux2 = 0;
    int maxCasas = 0;
    int numEspacos = 0;
    printf("\n");

    if((cp == m+1+n) || (lp == 0)){
        cp = lp = -1;
    }

    maxCasas = 0;
    aux = -999999;
    aux2 = 999999;
    //acha o maior elemento em módulo
    for(i=0;i<=m;i++){
        for(j=0; j <= m+n; j++){
            if (tableau[i][j] > aux){
                aux = tableau[i][j];
            }
            if (tableau[i][j] < aux2){
                aux2 = tableau[i][j];
            }
        }
    }
    if(aux2 < 0){
        aux2 = -1*aux2;
        if(aux2 > aux){
            aux = aux2;
        }
    }
    while(aux >= 10){
        aux = aux/10;
        maxCasas++;
    }
    //printf("\nM = %i, N=%i, B = %i\n",m,n,m+n);

    for(i=0; i <= m; i++){
        printf("\n");
        if(i==1){
            for(j=0; j <= m+n; j++){
                //printf("--");
                numEspacos = maxCasas+2;
                if((j==n) || (j==m+n)){
                    printf("-");
                }
                for(k=0;k<numEspacos;k++){
                    printf("--");
                    //numEspacos--;
                }
                    
            }
            printf("--\n");
        }
        for(j=0; j <= m+n; j++){
            if((j == n) || (j == m+n)){
                printf("|");
            }
            numEspacos = 0;
            aux = 0;
            aux2 = tableau[i][j];
            if(aux2< 0){
                aux2 = -1*aux2; 
            }
            else{
                if(((cp == j) || (lp == i))){
                    printf_super_green(" ");
                }
                else
                    printf(" ");
            }
            
            while(aux2 >= 9.5){
                aux2 = aux2/10;
                //if(aux2 == 1) aux++;
                aux++; //numero de descontos no espaco
            }

            numEspacos = maxCasas-aux;

            if((cp == j) && (lp == i)){
                while(numEspacos > 0){
                    printf_super_blue(" ");
                    numEspacos--;
                }
                printf_super_blue("%.2lf",tableau[i][j]);
                if(j == m+n){
                    printf("|");
                }
            } else if(((cp == j) || (lp == i))){ 
                while(numEspacos > 0){
                    printf_super_green(" ");
                    numEspacos--;
                }
                if(j != m+n)
                    printf_super_green("%.2lf",tableau[i][j]);
                if(j == m+n)
                    printf_super_green("%lf",tableau[i][j]);
                if(j == m+n){
                    printf("|");
                }
            }
            else{
                while(numEspacos > 0){
                    printf(" ");
                    numEspacos--;
                }
                if(j == m+n){
                    if(tableau[i][j] != 0)
                        printf("%lf",tableau[i][j]);
                    else{
                        tableau[i][j] = 0;
                        printf("%lf",tableau[i][j]);
                    }
                }
                else if(tableau[i][j] != 0)
                    printf("%.2lf",tableau[i][j]);
                else{
                    tableau[i][j] = 0;
                    printf_red("%.2lf",tableau[i][j]);
                }
                if(j == m+n){
                    printf("|");
                }
            }
        }
    }
    printf("\n");
    for(j=0; j <= m+n; j++){
        //printf("--");
        numEspacos = maxCasas+2;
        if((j==n) || (j==m+n)){
            printf_red(" ");
        }
        for(k=0;k<numEspacos-1;k++){
                printf("-");
        }
        if( base[j] )
            printf_cyan("^---");
        else
            printf_green("----");
                    
    }   

    printf("--\n");
    return;

}
void imprime_tableau_ilimitada_terminal(int n, int m, double** tableau, int colunaProblematica, bool* base){
    int i = 0;
    int j = 0;
    int k = 0;
    double aux = 0;
    double aux2 = 0;
    int maxCasas = 0;
    int numEspacos = 0;
    printf("\n");

    maxCasas = 0;
    aux = -999999;
    aux2 = 999999;
    for(i=0;i<n+1;i++){
        for(j=0; j < m+1+n; j++){
            if (tableau[i][j] > aux){
                aux = tableau[i][j];
            }
            if (tableau[i][j] < aux2){
                aux2 = tableau[i][j];
            }
        }
    }
    if(aux2 < 0){
        aux2 = -1*aux2;
        if(aux2 > aux){
            aux = aux2;
        }
    }
    while(aux >= 10){
        aux = aux/10;
        maxCasas++;
    }

    for(i=0; i < n+1; i++){
        printf("\n");
        if(i==1){
            for(j=0; j < m+n+1; j++){
                //printf("--");
                numEspacos = maxCasas+2;
                if((j==m) || (j==m+n)){
                    printf("-");
                }
                for(k=0;k<numEspacos;k++){
                    printf("-");
                    //numEspacos--;
                }
                    
            }
            printf("--\n");
        }
        for(j=0; j < m+1+n; j++){
            if((j == m) || (j == m+n)){
                printf("|");
            }
            numEspacos = 0;
            aux = 0;
            aux2 = tableau[i][j];
            if(aux2< 0){
                aux2 = -1*aux2; 
            }
            else{
                if(j == colunaProblematica)
                    printf_super_red(" ");
                else
                    printf(" ");
            }
            
            while(aux2 >= 9.5){
                aux2 = aux2/10;
                //if(aux2 == 1) aux++;
                aux++; //numero de descontos no espaco
            }

            numEspacos = maxCasas-aux;

            while(numEspacos > 0){
                if(j == colunaProblematica)
                    printf_super_red(" ");
                else
                    printf(" ");
                numEspacos--;
            }
            if(j == colunaProblematica){
                printf_super_red("%.0lf",tableau[i][j]);
            }
            else{
                if(tableau[i][j] != 0)
                    printf("%.0lf",tableau[i][j]);
                else
                    printf_red("%.0lf",tableau[i][j]);
            }
            if(j == m+n){
                printf("|");
            }
        }
    }
    printf("\n");
    for(j=0; j < m+n+1; j++){
        //printf("--");
        numEspacos = maxCasas+2;
        if((j==m) || (j==m+n)){
            printf("-");
        }
        for(k=0;k<numEspacos-1;k++){
                printf("-");
        }
            if( base[j] )
                printf_cyan("^");
            else
                printf("-");
                    
    }           

    printf("--\n");
    return;

}

void imprime_tableau_arquivo(int m, int n, double** tableau, int lp, int cp, bool* base){
    int i = 0;
    int j = 0;
    int k = 0;
    double aux = 0;
    double aux2 = 0;
    int maxCasas = 0;
    int numEspacos = 0;
    fprintf(registra,"\n");

    if((cp == m+1+n) || (lp == 0)){
        cp = lp = -1;
    }

    maxCasas = 0;
    aux = -999999;
    aux2 = 999999;
    //acha o maior elemento em módulo
    for(i=0;i<=m;i++){
        for(j=0; j <= m+n; j++){
            if (tableau[i][j] > aux){
                aux = tableau[i][j];
            }
            if (tableau[i][j] < aux2){
                aux2 = tableau[i][j];
            }
        }
    }
    if(aux2 < 0){
        aux2 = -1*aux2;
        if(aux2 > aux){
            aux = aux2;
        }
    }
    while(aux >= 10){
        aux = aux/10;
        maxCasas++;
    }
    //fprintf(registra,"\nM = %i, N=%i, B = %i\n",m,n,m+n);

    for(i=0; i <= m; i++){
        fprintf(registra,"\n");
        if(i==1){
            for(j=0; j <= m+n; j++){
                //fprintf(registra,"--");
                numEspacos = maxCasas+4;
                if((j==n) || (j==m+n)){
                    fprintf(registra,"-");
                }
                for(k=0;k<numEspacos;k++){
                    fprintf(registra,"-");
                    //numEspacos--;
                }
                    
            }
            fprintf(registra,"\n");
        }
        for(j=0; j <= m+n; j++){
            if((j == n) || (j == m+n)){
                fprintf(registra,"|");
            }
            numEspacos = 0;
            aux = 0;
            aux2 = tableau[i][j];
            if(aux2< 0){
                aux2 = -1*aux2; 
            }
            else{
                if(((cp == j) || (lp == i))){
                    fprintf(registra," ");
                }
                else
                    fprintf(registra," ");
            }
            
            while(aux2 >= 10){
                aux2 = aux2/10;
                //if(aux2 == 1) aux++;
                aux++; //numero de descontos no espaco
            }

            numEspacos = maxCasas-aux;

            if((cp == j) && (lp == i)){
                while(numEspacos > 0){
                    fprintf(registra," ");
                    numEspacos--;
                }
                fprintf(registra,"%.1lf",tableau[i][j]);
                if(j == m+n){
                    fprintf(registra,"|");
                }
            } else if(((cp == j) || (lp == i))){ 
                while(numEspacos > 0){
                    fprintf(registra," ");
                    numEspacos--;
                }
                if(j != m+n)
                    fprintf(registra,"%.1lf",tableau[i][j]);
                if(j == m+n)
                    fprintf(registra,"%.4lf",tableau[i][j]);
                if(j == m+n){
                    fprintf(registra,"|");
                }
            }
            else{
                while(numEspacos > 0){
                    fprintf(registra," ");
                    numEspacos--;
                }
                if(j == m+n){
                    if(tableau[i][j] != 0)
                        fprintf(registra,"%.4lf",tableau[i][j]);
                    else{
                        tableau[i][j] = 0;
                        fprintf(registra,"%.4lf",tableau[i][j]);
                    }
                }
                else{ 
                    if(tableau[i][j] != 0)
                        fprintf(registra,"%.1lf",tableau[i][j]);
                    else{
                        tableau[i][j] = 0;
                        fprintf(registra,"%.1lf",tableau[i][j]);
                    }
                }
                if(j == m+n){
                    fprintf(registra,"|");
                }
            }
        }
    }
    fprintf(registra,"\n");
    for(j=0; j <= m+n; j++){
        //fprintf(registra,"--");
        numEspacos = maxCasas+4;
        if((j==m+n)){
            fprintf(registra,"-");
        }
        for(k=0;k<numEspacos-1;k++){
                fprintf(registra,"-");
        }
            if( base[j] )
                fprintf(registra,"^");
            else
                fprintf(registra,"-");
                    
    }         

    fprintf(registra,"--\n");
    return;

}

//le o input
double pegaNum(FILE *entrada){

    char c = '#';
    double v = 0;
    bool temMais = true; 
    bool temMaisDec = false;
    int lastOrder = 1;
    double aux = 0;
    int sinal = 1;
    int limit = 6;
    int i = 0;

    while(!isdigit(c)){ //pega o primeiro
        c = fgetc(entrada);
        if(c == '-') sinal = -1; 
        if(feof(entrada)) return v*sinal;
    }
    
    v = c - '0';
    c = fgetc(entrada); 
    if(feof(entrada) || (c == ',') ) return v*sinal;
    
    if(isdigit(c)){
        temMais = true;
        while(temMais){
            v = v*10;
            v += c - '0';
            c = fgetc(entrada); 
            if(feof(entrada) || (c == ',') ) return v*sinal;

            if(!isdigit(c)){
                temMais = false;
                break;
            }
        }
    }
    if(c == '.'){
        ////printf("\nAchou um '%c', v = %lf", c, v);
        temMaisDec = true;
        while(temMaisDec){
            c = fgetc(entrada); if(feof(entrada) || (c == ',') ) return v*sinal;
            if(!isdigit(c)){
                return v*sinal;
            }
            ////printf("\nAte que entrou");
            aux = c - '0';
            ////printf("\naux = %lf, c = %c",aux,c);
            for(i=0;i<lastOrder;i++){
                aux = aux/10;
            }
                v += aux;
            lastOrder++;
            ////printf(", v = %lf, aux = %lf",v,aux);
            limit --;
            if(limit==0){
                return v*sinal;
            }
        }

    }
    else return v*sinal;

    return v*sinal;
}
bool podeDual(int m,int b,double **tableau){
    int i = 0;
    int j = 0;

    //se tiver negativo em c, nao fazemos o dual
    for(j=0; j<= b; j++){
        if(tableau[0][j] < 0){
            fprintf(registra,"\n   - Nao pode dual, tem negativo em c");
            return false;
        }
    }
    //se chegou aqui, nao tem negativo em c
    //agora checamos para ver se b tem negativo
    //tem que ter, caso contrario fazemos auxiliar
    for(i=0; i<= m; i++){
        if(tableau[i][b] < 0){
            fprintf(registra,"\n   - Pode fazer dual, nao tem negativo em c, mas tem negativo em b");
            return true;
        }
    }
    //se chegou aqui, nao tem negativo nem em c nem em b
    //nesse caso, temos que fazer o auxiliar
    fprintf(registra,"\n   - Nao pode dual, nao tem negativo em nenhum dos dois");
    return false;
}
bool podePrimal(int m,int b,double **tableau){

    int i = 0;
    int j = 0;
    bool tem = false;

    //se tiver negativo em c, podemos fazer o primal
    for(j=0; j<= b; j++){
        if(tableau[0][j] < 0){
            tem = true;
            break;
        }
    }
    if(!tem) {
        fprintf(registra,"\n   - Nao pode primal, nao tem negativo em c");
        return false;
    }
    //se chegou aqui,tem negativo em c
    //agora temos que chegar se tem algum negativo em b
    //se tiver, temos que fazer auxiliar

    for(i=1; i<= m; i++){
        if(tableau[i][b] < 0){
            fprintf(registra,"\n   - Nao pode primal, tem negativo em c, mas tambem tem em b");
            return false;
        }
    }
    //se chegou aqui, tem negativo em c mas nao tem em b
    //faremos primal
    fprintf(registra,"\n   - Pode fazer primal, tem negativo em c, mas nao tem em b");
    return true;
}
bool podeAux(int m,int b,double **tableau){
    //sempre podemos fazer se recebermos uma PL na forma especificada
    return true;
}
int achaColunaPivotalPrimal(int n, int b, double **tableau){
    
    int cp = 0;
    int j = 0;
    cp = 0;

    for(j=0;j<b;j++){ //metodo alternativo, pode ser mais rapido
        if(tableau[0][j] < tableau[0][cp] ){
            cp = j;
            //break;
        }
    }
    if(tableau[0][cp] >= 0)
        cp = -1;

    return cp;
}
int achaLinhaPivotalPrimal(int m, int b, int cp,double **tableau){

    int lp = 0;
    int i = 0;
    bool primeiroFoi = false;
    bool achouPositivo = false;
    fracao razao;
    fracao razaoMin;
  
    razaoMin.valor = 0;
    razaoMin.num = 0;
    razaoMin.den = 1;
    razao.valor = 0;
    razao.num = 0;
    razao.den = 1;


    for(i=1;i<=m;i++){
        razao.num = tableau[i][b];
        razao.den = tableau[i][cp];
        //fprintf(registra,"\nLendo: num = %.1lf, den=%.1lf",razao.num,razao.den);
        

        if((razao.num == 0) && (razao.den > 0)){ //regra de bland
            fprintf(registra,"\n   - Achou por regra de bland");
            lp = i;
        }
        if( (razao.den > 0) && (razao.num > 0) ){ //se o denominador e o numerador forem positivos, testamos
            razao.valor = razao.num/razao.den;
            //fprintf(registra,", valor = %.1lf",razao.valor);

            //como convencao, a primeira razao valida sera a minima
            //para o resto, a menor razao positiva eh o que queremos
            if((razao.valor < razaoMin.valor) || (!primeiroFoi)){
                fprintf(registra,"\n   - Achou minima razao positiva: num = %.1lf, den=%.1lf, valor = %.1lf",razao.num,razao.den,razao.valor);
                lp = i;
                razaoMin.valor = razao.valor;
                razaoMin.num = razao.num;
                razaoMin.den = razao.den;
                if(!primeiroFoi) primeiroFoi = true;
            }
            achouPositivo = true;
        }
    }
    if(achouPositivo == true){
        return lp;
    }
    else{
        return -1; //ilimitada
    }

}
int achaColunaPivotalDual(int b, int lp, double **tableau){
    
    int cp = 0;
    int i = 0;
    bool primeiroFoi = false;
    bool achouNegativo = false;
    fracao razao;
    fracao razaoMin;
    
    razaoMin.valor = 0;
    razaoMin.num = 0;
    razaoMin.den = 1;
    razao.valor = 0;
    razao.num = 0;
    razao.den = 1;

    for(i=0;i<b;i++){
        razao.num = tableau[0][i];
        razao.den = tableau[lp][i];
        //fprintf(registra,"\nLendo: num = %.1lf, den=%.1lf, valor = %.1lf",razao.num,razao.den,razao.valor);
        

        if((razao.num == 0) && (razao.den < 0) ){ //regra de bland
            fprintf(registra,"\n   - Vai retornar %i pela regra de bland",i);
            return i;
        }
        if( (razao.den < 0) && (razao.num > 0) ){ //garantindo razao negativa
            razao.valor = razao.num/razao.den;

            //como convencao, a primeira razao valida sera a minima
            //para o resto, a maior razao negativa eh o que queremos
            if((razao.valor > razaoMin.valor) || (!primeiroFoi)){
                cp = i;
                fprintf(registra,"\n   - Achou maior razao negativa: col: %i num = %.1lf, den=%.1lf, valor = %.1lf",cp, razao.num,razao.den,razao.valor);
                razaoMin.valor = razao.valor;
                razaoMin.num = razao.num;
                razaoMin.den = razao.den;
                if(!primeiroFoi) primeiroFoi = true;
            }
            achouNegativo = true;
        }   
    }
    if(achouNegativo == true){
        //fprintf(registra,"\nVai retornar %i",cp);
        return cp;
    }
    else{
        return -1; //ilimitada
    }

}
int achaLinhaPivotalDual(int m, int b,double **tableau){

    int lp = 0;
    int i = 0;
    lp = 1;

    for(i=1;i<=m;i++){
        if(tableau[i][b] < tableau[lp][b] )
            lp = i;
    }
    if(tableau[lp][b] >= 0)
        lp = -1;
    return lp;

}
int pivoteia(int m, int n, int lp, int cp, double** tableau, bool* base){

    int i = 0; //auxiliar
    int j = 0; //auxiliar
    int b = 0;
    double pivot = 0; //guarda o pivot
    double mult = 0; //multiplicador
    
    b = m+n;

    pivot = tableau[lp][cp];
    for(j=0;j<b;j++){
        if((tableau[lp][j] == 1) && (base[j])){
            base[j] = 0;
        }
    }
    base[cp] = 1;

    /*if(pivot <= 0){ //erro!
        fprintf(registra,"\nPIVOT NEGATIVO!");
        return -1;
    }*/

    for (j = 0; j <= b; j++){
       tableau[lp][j] /= pivot; //divide a linha inteira pelo pivot
       if((tableau[lp][j] < 0.00009)&&(tableau[lp][j] > -0.00009)){
           tableau[lp][j] = 0;
       }
    }

    if(tableau[lp][cp] != 1 ){ //erro!
        fprintf(registra,"\nERRO AO PIVOTEAR");
        return -2;
    }


    for (i = 0; i < m+1; i++){ // fazemos a conta com todos os elementos da matriz
        mult = tableau[i][cp];
            //fprintf(registra,"\nOI\n");
        if (i != lp){ //ignoramos a linha pivotal, ela ja foi
            for (j = 0; j <= b; j++){
                tableau[i][j] -= mult *tableau[lp][j];
                    if((tableau[i][j] < 0.00009)&&(tableau[i][j] > -0.00009)){
                        tableau[i][j] = 0;
                    }
            }
        }
    }

    return 1;

}

void criaTableauAux(int m, int n, int b, double **tableau, double **tableauAux,bool *base){

    int i = 0; 
    int j = 0; 
    int k = 0;
    int lp = 0;

    //preenche vetor c
    //zero em tudo, 1 acima da identidade
    for(i=0;i<=0;i++){ 
        //acima de A
        for(j=0;j<n;j++){
            tableauAux[i][j] = 0;
        }
        //acima da "identidade" original
        for(j=n;j<b;j++){
            tableauAux[i][j] = 0;
        }
        //acima da identidade auxiliar
        for(j=b;j<b+m;j++){
            tableauAux[i][j] = 1;
        }
        for(j=b+m;j<=b+m;j++){
            tableauAux[i][j] = 0;
        }
    }
    //preenche tableau auxiliar
    for(i=1;i<=m;i++){
        //multiplica por -1 uma linha onde b eh negativo
        //os dois loops estao ai para caso seja necessaria
        //alguma modificacao futura
        for(j=0;j<b;j++){
            if((tableau[i][b] >= 0) ){
                tableauAux[i][j] = tableau[i][j];
            }
            else{
                if(tableau[i][j] != 0){
                    tableauAux[i][j] = -1*tableau[i][j];
                    tableau[i][j] = -1*tableau[i][j];
                }
                else{
                    tableauAux[i][j] = 0;
                    tableau[i][j] = 0;
                }
            }
            //se a linha é negativada, perdemos uma base
            base[j] = false;
        }
        for(j=b;j<=b;j++){
            if((tableau[i][b] >= 0) ){
                tableauAux[i][j+m] = tableau[i][j];
            }
            else{
                if(tableau[i][j] != 0){
                    tableauAux[i][j+m] = -1*tableau[i][j];
                    tableau[i][j] = -1*tableau[i][j];
                }
                else{
                    tableauAux[i][j+m] = 0;
                    tableau[i][j] = 0;
                }
            }
        }

    }
    //preenche a diagonal das variaveis auxiliares
    for(i=1;i<=m;i++){
        for(j=b;j<b+m;j++){
            if(j-b+1==i){
                tableauAux[i][j] = 1; 
                base[j] = true;
            }
            else{
                tableauAux[i][j] = 0; 
            }
        }
    }
    fprintf(registra,"\nSegue o tableau original com sinais corrigidos");
    imprime_tableau_arquivo(m,n,tableau,-1,-1,base);
    fprintf(registra,"\nSegue o tableau auxiliar");
    imprime_tableau_arquivo(m,n+m,tableauAux,-1,-1,base);
    fprintf(registra,"\nAgora pivotearemos o tableau auxiliar");
    for(j=0;j<b+m;j++){
        if(tableauAux[0][j] == 1){
            for(i=1;i<=m;i++){
                if(tableauAux[i][j] == 1){
                    lp = i;
                }
            }
            fprintf(registra,"\nVai pivotear: linha %i, coluna %i",lp,j);
            imprime_tableau_arquivo(m,n+m,tableauAux,lp,j,base);       
            pivoteia(m,n+m,lp,j,tableauAux,base);
            fprintf(registra,"\nPivoteado:");     
            imprime_tableau_arquivo(m,n+m,tableauAux,lp,j,base);
        }
    }


    return;

}

void pivoteiaBaseOriginal(int m, int n, int b, double **tableau, double **tableauAux,bool *base){
    
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int salvaLinha = 0;
    bool achouPivotValido = true;

    fprintf(registra,"Eis a auxiliar:");
    imprime_tableau_arquivo(m,n+m,tableauAux,-1,-1,base);

    fprintf(registra,"\nPioteando a base original: \n");

    for(j=0;j<b;j++){
        achouPivotValido = false;
        if(base[j]){
            for(i=1;i<=m;i++){
                if(tableauAux[i][j] == 1){
                    salvaLinha = i;
                    if(tableau[i][j] != 0){
                        achouPivotValido = true;
                        fprintf(registra,"Vai pivotear na linha %i coluna %i",i,j);
                        imprime_tableau_arquivo(m,n,tableau,i,j,base);
                        pivoteia(m,n,i,j,tableau,base);
                        fprintf(registra,"\nPivoteado:");
                        imprime_tableau_arquivo(m,n,tableau,i,j,base);
                    }
                    break;
                }
            }
            if(!achouPivotValido){
                //se nao achou um pivot valido, 
                //somamos essa linha por outra
                i =salvaLinha;
                for(k=1;k<=m;k++){
                    if(tableau[k][j] != 0){
                        for(l=0;l<=b;l++){
                            tableau[salvaLinha][l] += tableau[k][l];
                        }
                        break;
                    }
                }
                fprintf(registra,"\nFoi somada uma linha para pivotear");
                imprime_tableau_arquivo(m,n,tableau,salvaLinha,j,base);
                pivoteia(m,n,salvaLinha,j,tableau,base);
                imprime_tableau_arquivo(m,n,tableau,salvaLinha,j,base);
                
            }
        }
    }

    return;
}


int simplexPrimal(int m, int n, int b, double** tableau, bool *base){

    int i = 0;
    int j = 0;
    int aux = 0;
    double v = 0;
    bool fim = 0;

    int colunaPivotal = b;
    int linhaPivotal = 0;

    fprintf(registra,"\nEXECUTANDO SIMPLEX PRIMAL");
    fprintf(registra,"\nMatriz original:");
    imprime_tableau_arquivo(m,n,tableau,linhaPivotal,colunaPivotal,base);

    while (!fim){
        fprintf(registra,"\nProcurando uma coluna pivotal...");
        colunaPivotal = achaColunaPivotalPrimal(n,b,tableau);
        if(colunaPivotal < 0){ 
            //um valor negativo retornado indica que nao foi encontrada 
            //nesse caso, o simplex  eh  viavel e limitado
            fprintf(registra,"\nO Simplex esta pronto!\n");
            fim = true;
            return 2;
        }
        fprintf(registra,"\nColuna pivotal = %i",colunaPivotal);
        fprintf(registra,"\nProcurando uma linha pivotal...");
        linhaPivotal = achaLinhaPivotalPrimal(m,b,colunaPivotal,tableau);

        if(linhaPivotal < 0){
            //um valor negativo retornado indica que nao foi encontrada 
            //nesse caso, o simplex  eh  ilimitado
            fprintf(registra,"\nIlimitado: nao foi encontrada nenhuma razao positiva");
            fprintf(registra,"\nO Simplex  eh  ilimitado!");
            fprintf(registra,"\nColuna oblemática: %i",colunaPivotal);
            fim = true;
            return 1;
        }
        fprintf(registra,"\nLinha pivotal = %i",linhaPivotal);
        fprintf(registra,"\nElemento = %.1lf",tableau[linhaPivotal][colunaPivotal]);
        fprintf(registra,"\nVai pivotear: linha %i, coluna %i",linhaPivotal,colunaPivotal);
        imprime_tableau_arquivo(m,n,tableau,linhaPivotal,colunaPivotal,base);
        aux =  pivoteia(m,n,linhaPivotal, colunaPivotal, tableau, base);
        //fprintf(registra,"\nMAX = %.1lf",tableau[0][b]);
        if(aux < 0){
            fprintf(registra,"\nO Simplex eh invalido, erro no pivoteamento");
            fim = true;
            return 0;
        }
        fprintf(registra,"\nPivoteado:");
        imprime_tableau_arquivo(m,n,tableau,linhaPivotal,colunaPivotal,base);

    }
   
    return 0;
}
int simplexDual(int m, int n, int b, double** tableau, bool *base){

    int i = 0;
    int j = 0;
    int aux = 0;
    double v = 0;
    bool fim = 0;

    int colunaPivotal = b;
    int linhaPivotal = 0;
    fprintf(registra,"\nEXECUTANDO SIMPLEX PRIMAL");

    imprime_tableau_arquivo(m,n,tableau,linhaPivotal,colunaPivotal,base);

    while (!fim){
        fprintf(registra,"\nProcurando uma linha pivotal...");
        linhaPivotal = achaLinhaPivotalDual(m,b,tableau);
        if(linhaPivotal < 0){
            //um valor negativo retornado indica que nao foi encontrada 
            //nesse caso, o simplex  eh  viavel e limitado
            fprintf(registra,"\nO Simplex esta pronto!");
            fim = true;
            return 0;
        }
        fprintf(registra,"\nLinha pivotal = %i",linhaPivotal);
        fprintf(registra,"\nProcurando uma coluna pivotal...");
        colunaPivotal = achaColunaPivotalDual(b,linhaPivotal,tableau);
        if(colunaPivotal < 0){
            fprintf(registra,"\nO Simplex eh invalido, erro na coluna pivotal");
            fim = true;
            return 2;
        }
        fprintf(registra,"\nColuna pivotal = %i",colunaPivotal);

        fprintf(registra,"\nElemento = %.1lf",tableau[linhaPivotal][colunaPivotal]);
        imprime_tableau_arquivo(m,n,tableau,linhaPivotal,colunaPivotal,base);
        aux =  pivoteia(m,n,linhaPivotal, colunaPivotal, tableau, base);
        //fprintf(registra,"\nMAX = %.1lf",tableau[0][b]);
        if(aux < 0){
            fprintf(registra,"\nO Simplex eh invalido, erro no pivoteamento");
            fim = true;
            return 0;
        }

        imprime_tableau_arquivo(m,n,tableau,linhaPivotal,colunaPivotal,base);

    }

    return 0;
}
int simplexAux(int m, int n, int b, double** tableau, bool *base){

    int i = 0;
    int j = 0;
    int aux = 0;
    int result = 0;
    double v = 0;
    bool fim = 0;

    int colunaPivotal = b;
    int linhaPivotal = 0;

    double** tableauAux;

    //aloca o tableua auxiliar
    tableauAux = (double**) malloc((m+2)*sizeof(double*));
    for(i=0; i< m+2; i++){
        tableauAux[i] = (double*) malloc((b+m+2)*sizeof(double));
    }
    for(i=0; i< m+2; i++){
        for(j=0; j< b+m+2; j++){
            tableauAux[i][j] = 0;
        }
    }

    fprintf(registra,"\nEXECUTANDO SIMPLEX AUXILIAR");

    fprintf(registra,"\nMatriz original, ainda intacta");
    imprime_tableau_arquivo(m,n,tableau,linhaPivotal,colunaPivotal,base);
    fprintf(registra,"\nCriaremos um tableau auxiliar");
    //cria um tableau auxiliar e o pivoteia
    criaTableauAux(m,n,b,tableau,tableauAux,base);
    fprintf(registra,"\nEis o tableau auxiliar, pivoteado");
    imprime_tableau_arquivo(m,n+m,tableauAux,linhaPivotal,colunaPivotal,base);

    fprintf(registra,"\nAgora, resolveremos o tableau auxiliar usando o simplex primal");
    //passa-se o tableau pivoteado para o simplex
    result = simplexPrimal(m,n+m,b+m,tableauAux,base);

    fprintf(registra,"\nEis a matriz auxiliar logo depois de ser resolvida pelo simplex primal");
    imprime_tableau_arquivo(m,n+m,tableauAux,-1,-1,base);

    //se o tableau auxiliar tiver um valor objetivo negativo,
    //a PL original eh inviavel
    if(tableauAux[0][b+m] < 0){
        fprintf(registra,"\nA PL eh inviavel!\n");
       
        for(i=0;i<m+2;i++)
            free(tableauAux[i]);
        free(tableauAux);
        return 0;
    }

    fprintf(registra,"\nAgora pivotearemos a base original olhando as bases do tableau auxiliar\n");
    //pivoteia no tableau original os elementos da base encontrados no auxiliar
     fprintf(registra,"\nAgora pivotearemos a base encotrada na matriz original");
    pivoteiaBaseOriginal(m,n,b,tableau,tableauAux,base);

    fprintf(registra,"\nMatriz original depois de ter sua base pivoteada");
    imprime_tableau_arquivo(m,n,tableau,-1,-1,base);

    fprintf(registra,"\nPassaremos essa matriz original modificada ao simplex primal");
    //agora podemos resolver o simplex primal:
    result = simplexPrimal(m,n,b,tableau,base);

    for(i=0;i<m+2;i++)
        free(tableauAux[i]);
    free(tableauAux);
    return result;
}

int engatilhaSimplex(int m,int n,int b,double** tableau,bool *base){
    
    int result = 0;
    registra = fopen("log.txt","w"); 
    ////imprime_tableau_terminal(m,n,tableau,-1,-1,base);
    if(podeDual(m,b,tableau)) //checa se pode fazer o dual
        result = simplexDual(m,n,b,tableau,base);
    else if(podePrimal(m,b,tableau)) //checa se pode fazer o primal
        result = simplexPrimal(m,n,b,tableau,base);
    else if(podeAux(m,b,tableau))//checa se pode fazer o auxiliar
        result = simplexAux(m,n,b,tableau,base);
    ////imprime_tableau_terminal(m,n,tableau,-1,-1,base);
    fclose(registra);
    return result;

}


#endif