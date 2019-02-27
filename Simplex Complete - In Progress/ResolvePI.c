/*
TRABALHO PRATICO DE PESQUISA OPERACIONAL
ASSUNTO: O ALGORITMO SIMPLEX
Professor: Gabriel Coutinho
Aluno: Evandro Lucas Figueiredo Teixeira
14/05/2018

O programa abaixo recebe uma Programação Inteira em 
um arquivo texto e retorna sua solucao.


O programa "ResolvePI. c" retorna como saida dois arquivos: 

- conclusao.txt, de onde e impresso o resultado dos algortimos 
de Programação Inteira, os valores objetivos, vetor solucao (se tiver) 
e certificados (de otimalidade ou ilimitada)
- log.txt, que registra o passo-a-passo do algoritmo simplex

Para rodar, abra o terminal do linux na pasta e digite make, de um enter.
Boa Sorte!

PS: por default ou ausencia de parametros, o arquivo default.txt sera
utilizado como demonstracao

*/




#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "color.h"
#include "simplex.h"

#define LIMITE 200 //evita estourar a memória
bool erroNumerico; //avisa erros inerentes à arquitetura do computador
bool erroLimite; //garante que não vai passar do limite


typedef struct branchResult{
    bool valido; //avisa se o resultado serve ou não
    double Z; //guarda o valor objetivo
    double solucao[2*LIMITE]; //guarda o vetor solucao
    int solucaoTamanho;

    double ZLR; //guarda o valor objetivo
    double solucaoRL[2*LIMITE]; //guarda o vetor solucao da relaxacao linear
    int solucaoRLTamanho; //solucao da relaxacao linear

    int certTamanho; //tamanho do certificado
    double cert[2*LIMITE]; //certificado (otimalidade, ilimitada, o que precisar)
    
    
}branchResult;

//construtor
branchResult new_branchResult(){
    branchResult branchVar;
    branchVar.valido = 0;
    branchVar.Z = 0;
    branchVar.solucaoTamanho = 0;
    branchVar.ZLR = 0;
    branchVar.solucaoRLTamanho = 0;
    branchVar.certTamanho = 0;
    return branchVar;
}

//prepara a resposta para quanto temos uma PI Viavel e Limitada
void preencheViavelLimitada(int m, int n, int b, double** tableau, bool* base, branchResult* resBranch){

    int i = 0; 
    int j = 0;
    int k = 0;
    
    //preenche o vetor solucao
    for(k=0;k<b;k++){
        if(base[k]){
            for(i=0;i<=m;i++){
                if( tableau[i][k] == 1){
                    resBranch->solucaoRL[k] = tableau[i][b];
                    resBranch->solucaoRLTamanho++;
                }
            }
        }
    }
    resBranch->ZLR = tableau[0][b];
    //preenche o certificado
    resBranch->certTamanho = b-n;
    for(j=n;j<b;j++){
        resBranch->cert[j-n] = tableau[0][j];
    }

    return;

}
//imprime a resposta para quanto temos uma PI Viavel e Limitada
void imprimeResposta(branchResult resBranch){

    int i, j = 0;
    FILE* saida;

    saida = fopen("conclusao.txt","w");

    fprintf(saida,"2\n");

    /*printf_super_yellow("\n\n\n    ");
    printf_yellow("  RESPOSTA:  ");
    printf_super_yellow("    ");*/
    printf("\n");

    //printf("Validez = %i",resBranch.valido);
    //printf("\n      Z = %lf",resBranch.Z);
    //printf("\nSolucao = [");
    fprintf(saida,"[");
    for(i=0;i<resBranch.solucaoTamanho;i++){
        //printf("%.0lf",resBranch.solucao[i]);
        fprintf(saida,"%.0lf",resBranch.solucao[i]);
        if(i < resBranch.solucaoTamanho-1){
            //printf(",");
            fprintf(saida,",",resBranch.solucao[i]);
        }
    }
    //printf("]\n");
    fprintf(saida,"]\n");
    fprintf(saida,"%.0lf\n",resBranch.Z);

    fprintf(saida,"[");
    for(i=0;i<resBranch.solucaoRLTamanho;i++){
        fprintf(saida,"%.4lf",resBranch.solucaoRL[i]);
        if(i < resBranch.solucaoRLTamanho-1){
            fprintf(saida,",",resBranch.solucaoRL[i]);
        }
    }

    fprintf(saida,"]\n");
    fprintf(saida,"%.4lf\n",resBranch.ZLR);

    fprintf(saida,"[");
    for(i=0;i<resBranch.certTamanho;i++){
        fprintf(saida,"%.4lf",resBranch.cert[i]);
        if(i < resBranch.certTamanho-1){
            fprintf(saida,",",resBranch.cert[i]);
        }
    }
    fprintf(saida,"]\n");

    fprintf(saida,"\n");
    fclose(saida);
    return;
}

//prepara a resposta para quanto temos uma PI Viavel porém Ilimitada
void preencheViavelIlimitada(int m, int n, int b, double** tableau, bool* base, branchResult* resBranch){

    int i = 0; 
    int j = 0;
    int k = 0;
    bool achou = 0; 
    int cp = 0; 

    // acha uma coluna problematicaprintf("\n* ");
    for(k=0;k<b;k++){
        if(tableau[0][k] < 0){
            for(i=1;i<=m;i++){
                if(tableau[i][k] > 0){
                    achou = 1;
                }
            }
            if(!achou){
                cp = k;
                break;
            }
        }
    }
    for(j=0;j<b;j++){
        if(base[j]){
            for(i=0;i<=m;i++){
                if(tableau[i][j] == 1){
                    resBranch->cert[i] = -1*tableau[i][cp];
                    resBranch->certTamanho++;
                }
            }
        }
    }

    return;

}
//imprime a resposta para quanto temos uma PI Viavel porém Ilimitada
void imprimeRespostaIlimitada(branchResult resBranch){

    int i, j = 0;
    FILE* saida;

    saida = fopen("conclusao.txt","w");

    fprintf(saida,"1\n");

        fprintf(saida,"[");
    for(i=0;i<resBranch.certTamanho;i++){
        fprintf(saida,"%.4lf",resBranch.cert[i]);
        if(i < resBranch.certTamanho-1){
            fprintf(saida,",",resBranch.cert[i]);
        }
    }
    fprintf(saida,"]\n");

    fprintf(saida,"\n");
    fclose(saida);
    return;
}
//imprime a resposta para quanto temos uma PI Inviavel
void imprimeRespostaInviavel(){

    int i, j = 0; //o costume eh tao grande que ja coloquei os dois
    //eu sei que nao precido de variavel alguma, mas deixarei aqui 
    //por motivos emocionais
    FILE* saida;

    saida = fopen("conclusao.txt","w");

    fprintf(saida,"0\n");

    fprintf(saida,"\n");
    fclose(saida);
    return;
}
//
double processa_solucao(int m,int n,int b,double** tableau, bool* base, int* resultado, int variable, int res){
    
    int i,j = 0;
    double* solucao;
    double aux = 0;
    double aux2 = 0;

    //resultado[0] = 0 se for tudo inteiro, 1 se tiver algum racional
    //resultado[1] = posição n de xn
    //resultado[2] = floor
    //resultado[3] = ceil
    solucao = (double*) calloc(b+m+m,sizeof(double));

    //encontra e copia o vetor solucao
    for(j=0;j<n;j++){
        if(base[j]){
            for(i=0;i<=m;i++){
                if( tableau[i][j] == 1){
                    solucao[j] = tableau[i][b];
                    //printf_green("x%i= %.2lf  ",j,solucao[j]);
                    break;
                }
            }
        }
    }
   
    //seleciona a variavel de maior valor decimal
    i = 0;
    aux = 1;
    for(j=0;j<b;j++){
        if(ceil(solucao[j]) != solucao[j]){
            resultado[0] = 1;
            if(ceil(solucao[j]) - solucao[j] <= aux){
                i = j;
                aux = ceil(solucao[j]) - solucao[j];
            }
        }
    } 
    aux = solucao[variable]; //parametro de retorno
   
   /*printf_blue(" Z = %4.2lf",tableau[0][b]);
    if((resultado[0] == 0) && (res == 2))
        printf_super_green("  ");*/

    resultado[1] = i;
    resultado[2] = floor(solucao[i]);
    resultado[3] = ceil(solucao[i]);

    free(solucao);
    //printf(" Vai retornar: %lf",aux);
    return aux;
}

//parte recursiva do branch and bound
branchResult branch(int m,int n,int b,double** tableauRecebido,bool* base, double bValue, int variable, int qualBranch, int limite){
    int i, j = 0; //auxiliares
    //ATENCAO: Nao foi feita a analise de sensibilidade
    //a restricao eh adicionada no tableau bruto para ser resolvida desde o inicio
    //a funcao ficou grande e com partes repetidas
    //o tempo foi pessoalmente curto, nao tive como cortar a parte desnecessaria

    double** tableauBranch; //tableau que sera modificado pelo simplex
    bool* baseBranch; //base do tableau acima
    double** tableauSave; //tableau que sera passado recursivamente
    bool* baseSave;  //base do tableau acima
    int* resultado; //vetor que indica os primeiros resultados do simplex

    //auxiliares para o simplex
    int res = 2;
    double v = 0;
    int M = m+1;
    int N= n; 
    int B = M+N;

    //inicializa os branchResults para serem passados como parametro
    branchResult resPrimeiro = new_branchResult();
    branchResult resSegundo = new_branchResult();

    resPrimeiro.solucaoTamanho= resSegundo.solucaoTamanho = n; //so pra garantir
    
    //Aloca tudo
    tableauBranch = (double**) calloc((m+2+1),sizeof(double*));
    for(i=0; i< m+2; i++){
        tableauBranch[i] = (double*) calloc((b+2+1),sizeof(double));
    }
    baseBranch = (bool*) calloc((B+M+M),sizeof(bool));
    tableauSave = (double**) calloc((m+2+1),sizeof(double*));
    for(i=0; i< m+2; i++){
        tableauSave[i] = (double*) calloc((b+2+1),sizeof(double));
    }
    baseSave = (bool*) calloc((B+M+M),sizeof(bool));
    resultado = (int*) calloc((6),sizeof(int));

    //Copiam-se os tableaus
    //Note que eles também sao expandidos, ja previamente alocados com essa intenção
    for(i=0; i <= m; i++){
        for(j=0; j <= n; j++){
            tableauBranch[i][j] = tableauRecebido[i][j];  
              tableauSave[i][j] = tableauRecebido[i][j];    
        }
        for(j=n; j < b; j++){
            if(i+m-1 == j){
                tableauBranch[i][j] = 1;
                baseBranch[j] = 1;
                  tableauSave[i][j] = 1;
                  baseSave[j] = 1;
            }
        }
        tableauBranch[i][B] = tableauRecebido[i][b];
          tableauSave[i][B] = tableauRecebido[i][b];
    }

    //para a ultima linha: 
    //qualBranch = -1 se a coisa foi pra direita,  isso é, xn >= bValue
    //qualBranch =  1 se a coisa foi pra esquerda, isso é, xn <= bValue
        tableauBranch[M][variable] = 1*qualBranch;
          tableauSave[M][variable] = 1*qualBranch;
        tableauBranch[M][B] = bValue*qualBranch;
          tableauSave[M][B] = bValue*qualBranch;
        tableauBranch[M][B-1] = 1;
          tableauSave[M][B-1] = 1;
        baseBranch[B-1] = 1;
          baseSave[B-1] = 1;
    
    //printf_super_yellow("\nPreparando para o simplex:\n");
    //imprime_tableau_terminal(M,N,tableauBranch,-1,-1,baseBranch);

    //Roda o simplex no tableau
    res = engatilhaSimplex(M,N,B,tableauBranch,baseBranch);

    /*printf("\n ");
    for(i=0;i<limite;i++){
        printf("  ");
    }
    if(qualBranch == -1){
        printf("x%i >= %.2lf : ",variable, bValue);
    }
    if(qualBranch == 1){
        printf("x%i <= %.2lfi: ",variable, bValue);
    }*/

    //Processa os dados do simplex
    v = processa_solucao(M,N,B,tableauBranch,baseBranch,resultado,variable,res);
    
    //Caso ilimitado ou inviavel, ignora-se o branch
    if(res != 2){
        //printf_super_red("  ");
        //printf_super_red("\nSem solução viavel\n");
        for(i=0;i<m+3;i++) free(tableauBranch[i]);
        free(tableauBranch);
        free(baseBranch);
        for(i=0;i<m+3;i++) free(tableauSave[i]);
        free(tableauSave);
        free(baseSave);
        free(resultado);
        return resPrimeiro;
    }
    //arredondamento para evitar erro numerico
    //so ameniza o problema, nao resolve
    if((v - bValue < 0.00001) && (v - bValue > 0)){
        v = bValue;
    }
    if((v - bValue > -0.00001) && (v - bValue < 0)){
        v = bValue;
    }
    
    //trata erros numericos
    if(qualBranch == -1){
        if(v < bValue){
            printf_red("\nIMPEDIDO: x%i = %lf quando x%i >= %lf ",variable,v,variable,bValue);
            //printf_super_red("\nDepois do simplex:\n");
            //imprime_tableau_terminal(M,N,tableauBranch,-1,-1,baseBranch);
            erroNumerico = 1;
            for(i=0;i<m+3;i++) free(tableauBranch[i]);
            free(tableauBranch);
            free(baseBranch);
            for(i=0;i<m+3;i++) free(tableauSave[i]);
            free(tableauSave);
            free(baseSave);
            free(resultado);
            return resPrimeiro;
        }
    }
    //trata erros numericos
    if(qualBranch == 1){
        if(v > bValue){
            printf_red("\nIMPEDIDO: x%i = %lf quando x%i <= %lf ",variable,v,variable,bValue);
            //printf_super_red("\nDepois do simplex:\n");
            //imprime_tableau_terminal(M,N,tableauBranch,-1,-1,baseBranch);
            erroNumerico = 1;
            for(i=0;i<m+3;i++) free(tableauBranch[i]);
            free(tableauBranch);
            free(baseBranch);
            for(i=0;i<m+3;i++) free(tableauSave[i]);
            free(tableauSave);
            free(baseSave);
            free(resultado);
            return resPrimeiro;
        }
    }

    //printf("\nDepois do simplex:\n");
    //imprime_tableau_terminal(M,N,tableauBranch,-1,-1,baseBranch);

    //No caso abaixo, temos um candidato valido
    //devolvemos ele
    if(resultado[0] == 0){
        resPrimeiro.valido = 1;
        resPrimeiro.Z = tableauBranch[0][B];
        //printf("\nresPrimeiro.Z = %lf, resPrimeiro.valido = %i",resPrimeiro.Z,resPrimeiro.valido);
        //preeche o vetor solucao
        for(j=0;j<N;j++){
            if(baseBranch[j]){
                for(i=0;i<=M;i++){
                    if( tableauBranch[i][j] == 1){
                        resPrimeiro.solucao[j] = tableauBranch[i][B];
                        break;
                    }

                }
            }
        }
        for(i=0;i<m+3;i++) free(tableauBranch[i]);
        free(tableauBranch);
        free(baseBranch);
        for(i=0;i<m+3;i++) free(tableauSave[i]);
        free(tableauSave);
        free(baseSave);
        free(resultado);
        return resPrimeiro;
    }

    //Se chegar até aqui, mais branchs. 
    if( limite == LIMITE){ //testamos um limite seguro pra memoria
        printf_super_red("OPERACAO ENCERRADA: PASSOU DO LIMITE");
        erroLimite = 1;
        resPrimeiro.valido = 0;
        for(i=0;i<m+4;i++) free(tableauBranch[i]);
        free(tableauBranch);
        free(baseBranch);
        for(i=0;i<m+4;i++) free(tableauSave[i]);
        free(tableauSave);
        free(baseSave);
        free(resultado);
        return resPrimeiro;
    }
    else{ //estando tudo bem, faremos mais branches
        i = limite + 1;
        //Chama o primeiro branch, onde x <= algumacoisa
        resPrimeiro = branch(M,N,B,tableauSave,baseSave,resultado[2],resultado[1], 1,i);
        //Chama o segundo  branch, onde x >= algumacoisa
         resSegundo = branch(M,N,B,tableauSave,baseSave,resultado[3],resultado[1],-1,i);
        
        //retorna o primeiro ou o segundo, depende de qual for o melhor
        if((resPrimeiro.Z*resPrimeiro.valido) > (resSegundo.Z*resSegundo.valido)) {
            for(i=0;i<m+3;i++) free(tableauBranch[i]);
            free(tableauBranch);
            free(baseBranch);
            for(i=0;i<m+3;i++) free(tableauSave[i]);
            free(tableauSave);
            free(baseSave);
            free(resultado);
            return resPrimeiro;
        }
        else{
            for(i=0;i<m+3;i++) free(tableauBranch[i]);
            free(tableauBranch);
            free(baseBranch);
            for(i=0;i<m+3;i++) free(tableauSave[i]);
            free(tableauSave);
            free(baseSave);
            free(resultado);
            return resSegundo;
        }
    }

    for(i=0;i<m+3;i++) free(tableauBranch[i]);
    free(tableauBranch);
    free(baseBranch);
    for(i=0;i<m+3;i++) free(tableauSave[i]);
    free(tableauSave);
    free(baseSave);
    free(resultado);
    return resPrimeiro;

}

//parte inicial do branch and bound
int branch_and_bound(int m, int n, int b, double **tableau, bool* base){
    
    int i,j = 0;//auxiliares
    int res = 0;//resposta do simplex

    double** tableauSave; //tableau a ser passado como parametro
    bool* baseSave; //base do tableau acima
    int* resultado; //resultado geral do tableau

    //inicializa respostas a serem recebidas
    branchResult resPrimeiro= new_branchResult();
    branchResult resSegundo= new_branchResult();

    //variavel global
    erroNumerico = 0;//por enquanto, nao ocorreu nenhum erro

    //Aloca tudo
    tableauSave = (double**) calloc((m+1),sizeof(double*));
    for(i=0; i< m+1; i++){
        tableauSave[i] = (double*) calloc((b+1),sizeof(double));
    }
    baseSave = (bool*) calloc((b+m+m),sizeof(bool));

    resultado = (int*) calloc((6),sizeof(int));

    for(i=0;i<=m;i++){
        for(j=0;j<=b;j++){
            tableauSave[i][j] = tableau[i][j];
        }
    }
    

    //Roda o simplex e analiza o resultado
    //printf("Começou branch and bound");
    res = engatilhaSimplex(m,n,b,tableau,base);
    
    //Se 1, ilimitada, pode imprimir e fechar
    if(res == 1){
        //printf("\nO simplex eh ilimitado\n");
        preencheViavelIlimitada(m,n,b,tableau,base,&resPrimeiro);
        imprimeRespostaIlimitada(resPrimeiro);
        free(resultado);
        free(baseSave);
        for(i=0;i<m+2;i++) free(tableauSave[i]);
        free(tableauSave);
        return 0;
    }   
     //Se 0, inviavel, pode imprimir e fechar
    if(res == 0){
        //printf_red("\nO simplex eh inviavel\n");
        imprimeRespostaInviavel();
        free(resultado);
        free(baseSave);
        for(i=0;i<m+1;i++) free(tableauSave[i]);
        free(tableauSave);
        return 0;
    }

    //printf("Resolução da PL Original:");
    //imprime_tableau_terminal(m,n,tableau,-1,-1,base);
    printf("\n ");

    //Processa o tableau "simplexado"
    processa_solucao(m,n,b,tableau,base,resultado,0,2);

    //Chamam-se os branches, passando como parametro o tableau bruto

    //qualBranch = -1 se a coisa foi pra direita,  isso é, xn >= bValue
    //qualBranch =  1 se a coisa foi pra esquerda, isso é, xn <= bValue

    //printf("\nVai chamar a primeira:");
    resPrimeiro = branch(m,n,b,tableauSave,baseSave,resultado[2],resultado[1],1,1);
    //printf("\nVai chamar a segunda:");
    resSegundo = branch(m,n,b,tableauSave,baseSave,resultado[3],resultado[1],-1,1);
    //printf("\nresPrimeiro.Z = %lf, resPrimeiro.valido = %i",resPrimeiro.Z,resPrimeiro.valido);
    //printf("\nresSegundo.Z = %lf, resSegundo.valido = %i",resSegundo.Z,resSegundo.valido);

    //Compara e preenche a resposta
    if((resPrimeiro.Z*resPrimeiro.valido) >= (resSegundo.Z*resSegundo.valido)) {
        preencheViavelLimitada(m,n,b,tableau,base,&resPrimeiro);
        imprimeResposta(resPrimeiro);
    }
    else{
        preencheViavelLimitada(m,n,b,tableau,base,&resSegundo);
        imprimeResposta(resSegundo);
    }

    
    free(resultado);
    free(baseSave);
    for(i=0;i<m+1;i++) free(tableauSave[i]);
    free(tableauSave);
    return 0;


}
//avisa se um numero eh ou nao inteiro
bool ehInteiro(double num){

    if(num < ceil(num)) {
        //printf_red("\n%lf eh menor do que %lf",num,ceil(num));
        return 0; 
    }
    if(num > floor(num)) {
        //printf_red("\n%lf eh maior do que %lf",num,floor(num));
        return 0;
    }
    //printf_blue("\n%lf eh  inteiro",num);
    return 1;
}
//avisa se existem inteiros na solucao ou nao
int solucaoEhInteira(int m, int n, int b, double** tableau,bool* base){
    int i = 0;
    int j = 0;
    int k = 0;


    for(k=0;k<b;k++){
        if(base[k]){
            for(i=0;i<=m;i++){
                if( tableau[i][k] == 1){
                    //printf("\nVai testar para tableau[%i][%i] = %lf",i,b,tableau[i][b]);
                    if(!ehInteiro(tableau[i][b])) {
                        //printf("\nAchou nao-inteiro vai retornar linha %i",i);
                        return i;
                    }
                }
            }
        }
    }
    //printf("\nVai retornar porque so achou inteiro");
    return -1;
}
//adiciona uma linha de pisos dos valores racionais
void adicionaPiso(int M,int N,int B,double** tableau,bool* base,double** tableauSave,bool* baseSave,int sol){

    int i,j = 0;
    //corrigir posição de B e ja adianta a nova coluna
    for(i=0;i<=M;i++){
        tableauSave[i][B] = tableauSave[i][B-1];
        tableauSave[i][B-1] = 0;
        if(i == M){
            tableauSave[i][B-1] = 1;
            baseSave[B-1] = 1;
        }
    }

    //restaura a boa e velha base
    for(j=0;j<B;j++){
        base[j] = 0;
    } 
    for(j=M;j>B-1;j++){
        base[j] = 1;
    }
    //adiciona a nova linha
    for(j=0;j<B-1;j++){
        if(j!=B-1){
            tableauSave[M][j] = floor(tableau[sol][j]);
        }
    }
    tableauSave[M][B] = floor(tableau[sol][B-1]);
    
    return;

}


int planos_de_corte(int m, int n, int b, double **tableau, bool* base){    
    
    int i,j = 0;
    int res = 0;
    int limite = 0;

    int M = m;
    int N = n;
    int B = b;
    int sol = 0;

    int* resultado; //dados iniciais do tableau apos o simplex
    double** tableauSave; //salva o tableau bruto (nao "simplexado")
    bool* baseSave;//base do tableau acima
    branchResult resUnico = new_branchResult(); 

    //variavel global
    erroNumerico = 0; //trata erro numerico

    //Aloca tudo
    //AVISO: existe um limite estatico, mude ele se precisar de memoria!
    resultado = (int*) calloc((6),sizeof(int));
    tableauSave = (double**) calloc((2*LIMITE),sizeof(double*));
    for(i=0; i< 2*LIMITE; i++){
        tableauSave[i] = (double*) calloc((2*LIMITE),sizeof(double));
    }
    baseSave = (bool*) calloc((4*LIMITE),sizeof(bool));
    for(i=0;i<=M;i++){
        for(j=0;j<=B;j++){
            tableauSave[i][j] = tableau[i][j];
        }
    }

    //Fazemos o primeiro simplex
    //printf("Começou planos de corte");
    res = engatilhaSimplex(m,n,b,tableau,base);
    
    //Caso ilimitado, imprime e encerra
    if(res == 1){
        //printf("\nO simplex eh ilimitado\n");
        preencheViavelIlimitada(m,n,b,tableau,base,&resUnico);
        imprimeRespostaIlimitada(resUnico);

        return 0;
    }
    //Caso inviavel, imprime e encerra
    if(res == 0){
       // printf_red("\nO simplex eh inviavel\n");
        imprimeRespostaInviavel();

        return 0;
    }
    preencheViavelLimitada(M,N,B,tableau,base,&resUnico);

    //printf("\nResolução da PL Original:\n");
    //imprime_tableau_terminal(M,N,tableau,-1,-1,base);

    //SOL guarda a linha a ser copiada e transformada em inteira
    sol = solucaoEhInteira(M,N,B,tableau,base);
    while(sol >= 0){
        limite++; 
        if(limite == LIMITE){ //deu ruim
            printf("LIMITE ATINGIDO: OPERACAO CANCELADA");
            break;
        }
        sol = solucaoEhInteira(M,N,B,tableau,base);
        //printf("\nRetornou sol = linha %i",sol);
            if(sol < 0) break; //se nao tiver racionais, paramos
        //printf("\nVai continuar porque na linha %i temos %lf, que nao eh inteiro",sol,tableau[sol][B]);

        //printf("\nTableau antes de qualquer coisa: \n");
        //imprime_tableau_terminal(M,N,tableauSave,-1,-1,baseSave);
        M = M+1;
        B = B+1;
        //so pra garantir
        //adiciona a linha do piso dos valores racionais da linha escolhida (SOL)
        adicionaPiso(M,N,B,tableau,base,tableauSave,baseSave,sol);
        //printf("\nTableau antes do simplex: \n");
        //imprime_tableau_terminal(M,N,tableauSave,-1,-1,baseSave);
        //Restaura o simplex de sua versao bruta, porem com uma linha e uma coluna a mais
        for(i=0;i<=M;i++){
            for(j=0;j<=B;j++){
                tableau[i][j] = tableauSave[i][j];
            }
        }
        //rodamos o novo simplex
        res = engatilhaSimplex(M,N,B,tableau,base);
        if(res != 2){
            if(res == 1){
                //printf("\nO simplex eh ilimitado\n");
                preencheViavelIlimitada(m,n,b,tableau,base,&resUnico);
                imprimeRespostaIlimitada(resUnico);
                return 0;
            }
            if(res == 0){
                //printf_red("\nO simplex eh inviavel\n");
                imprimeRespostaInviavel();
                return 0;
            }
        }
        //se chegou aqui,o simplex foi resolvido com sucesso.
        //se dermos sorte, saimos do loop
        //ou reinteramos
            //printf("\nTableau depois do simplex: \n");
            //imprime_tableau_terminal(M,N,tableau,-1,-1,base);
  
    }
    //se chegou aqui, temos uma solucao para a PI
        resUnico.Z = tableau[0][B];
        for(j=0;j<B;j++){
            if(base[j]){
                for(i=0;i<=M;i++){
                    if( tableau[i][j] == 1){
                        resUnico.solucao[j] = tableau[i][B];
                        resUnico.solucaoTamanho++;
                    }
                }
            }
        }
        imprimeResposta(resUnico);
   
    return 0;
}


//recebe uma matriz, coloca em FPI e resolve o simplex
int main (int argc, char*argv[]){

    double** tableau;
    bool *base;
    int n = 1; //quantidade de colunas em A, tamanho de cada linha
    int m = 1; //quantidade de linhas em A, tamanho de cada coluna
    int b = 1; //quantidade de linha no tableau
    bool op = 0; 
    int result = 0; //resultado do simplex
    FILE *entrada; //de onde sera lido o tableau

    int i = 0; //auxiliar
    int j = 0; //auxiliar
    char c = 'a'; //auxiliar
    double v = 0; //auxiliar

    //o trecho abaixo le o input
    {
        if(argc != 1){ //recebe o arquivo de entrada do usuario
            if(argc < 2){
                printf("\nEntrada invalida: especifique 1 e apenas 1 arquivo de entrada\n");
                return 0;
            }

            entrada = fopen(argv[1],"r");

            if(entrada == NULL){
                printf("\nEntrada invalida: o arquivo especificado nao existe\n");
                return 0;
            }
        }
        else{ //se nao for especificado, le do arquivo default
            //printf("\nAbrindo default.txt\n");
            entrada = fopen("default.txt","r");
            //printf("\nAbriu default.txt\n");

        }
        fscanf(entrada,"%i",&op);
        fscanf(entrada,"%i",&m);
        fscanf(entrada,"%i",&n);

        b = m+n;

        if(b <= 2){
            printf("\nEntrada invalida: matriz pequena demais\n");
            return 0;
        }
        //começa o programa

        //aloca o tableau
        tableau = (double**) malloc((2*LIMITE)*sizeof(double*));
        for(i=0; i< 2*LIMITE; i++){
            tableau[i] = (double*) malloc((2*LIMITE)*sizeof(double));
        }
        base = (bool*) malloc((4*LIMITE)*sizeof(bool));
        
        //inicializa o tableau como zero em todas as posições
        for(i=0; i < m+2; i++){
            for(j=0; j < b+2; j++){
                tableau[i][j] = 0;
            }
        }
        for(j=0; j < 2*b+m; j++){
            base[j] = false;
        }
        //le os numeros da entrada e prenche a identidade
        //a função peganum recebe um arquivo de texto e retira dali um elemento da matriz
        for(i=0; i < m+1; i++){
            for(j=0; j < b+1; j++){
                //fscanf(entrada,"%c",c);
                    if((j == n) || ( (j > n) && (j < b))) {
                        v = 0;
                        if(i+n-1 == j){
                            v = 1;
                            base[j] = 1;
                        }
                    }
                    else{
                        v = pegaNum(entrada);
                    }
                    if((i==0) && (v!=0))
                        v = v*(-1);
                    tableau[i][j] = v;       
            }
        }

        fclose(entrada);

    }
    //descobre o simplex a ser usado e usa
    
    if(op == 1){
        result = branch_and_bound(m,n,b,tableau,base);
    }
    else{
        result = planos_de_corte(m,n,b,tableau,base);
    }

    if(erroNumerico){
        printf_super_red("OCORREU UM ERRO NUMERICO, o resultado nao sera exato");
    }
    if(erroLimite){
        printf_super_red("OCORREU UM ERRO DE LIMITE, mude a constante LIMITE para liberar mais espaço na arvore de execução");
    }

    free(base);
    for(i=0;i<2*LIMITE;i++)
        free(tableau[i]);
    free(tableau);

    return 0;

}