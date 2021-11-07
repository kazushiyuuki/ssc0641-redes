#include"jogadores.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/socket.h>

//Essa função realiza a distribuição de peças para um determinado jogador
void iniciarPiecesJog(pontDeque monte, pontJogadores jog)
{
    pontCarta temp;
    for(int i = 0; i < 6; i++)
    {
        //Aqui a carta do início do monte é colocada na posição i do vetor de peças da estrutura jogador
        jog->piecesJogador[i].cor = monte->inicio->info.cor;
        jog->piecesJogador[i].formato = monte->inicio->info.formato;
        //Aqui é feito o remanejamento dos ponteiros
        temp = monte->inicio;
        monte->inicio = monte->inicio->prox;
        monte->qtdCartas--;
        free(temp);
    }
    jog->qtdPieces = 6;
}

//Essa função realiza a troca da peça na posição pos do vetor de peças do jogador
void trocarPiecesJog(pontDeque monte, pontJogadores jog, int pos)
{
    //Aloca-se um espaço para uma nova carta, armazena as informações da peça a ser trocada nela e coloca ela no fim do monte
    pontCarta temp = (pontCarta)malloc(sizeof(carta));
    temp->prox = NULL;
    temp->info.cor = jog->piecesJogador[pos].cor;
    temp->info.formato = jog->piecesJogador[pos].formato;
    monte->fim->prox = temp;

    //Coloca as informações da carta iniciaal na posição do vetor de peças do jogador
    jog->piecesJogador[pos].cor = monte->inicio->info.cor;
    jog->piecesJogador[pos].formato = monte->inicio->info.formato;

    //Libera a memória da carta no início do deque e remaneja o ponteiro para o novo início do deque
    temp = monte->inicio;
    monte->inicio = monte->inicio->prox;
    free(temp);
}

//Função para imprimir na tela algumas as informações de um jogador: nome e todas as peças disponíveis
void printarJog(pontJogadores jog)
{
    char msg[256];
    sprintf(msg, "Sua vez de jogar\n");
    send(jog->numSocket, msg, strlen(msg), 0);
    sprintf(msg, "Pecas disponiveis: ");
    send(jog->numSocket, msg, strlen(msg), 0);
    for(int i = 0; i < jog->qtdPieces; i++)
    {
        if(jog->piecesJogador[i].formato != ' ' &&  jog->piecesJogador[i].cor != ' ')
        {
            sprintf(msg, "[%c%c] ", jog->piecesJogador[i].formato, jog->piecesJogador[i].cor);
            send(jog->numSocket, msg, strlen(msg), 0);
        }
    }
    sprintf(msg, "\n");
    send(jog->numSocket, msg, strlen(msg), 0);
}

void joga(pont board, pontDeque monte, pontJogadores jog, char peca, char numero, int linha, int coluna, int *n_jogada, int *todasJogadas)
{  
    char msg[256];
    int i, aux = 0, pos;
    if(linha >= board->rows || coluna >= board->columns || linha < 0 || coluna < 0 || board->pieces[linha][coluna].cor != ' ' || board->pieces[linha][coluna].formato != ' '){
        aux = 1;
    }
    else{
        for(i = 0; i < jog->qtdPieces; i++)
        {   
            if(peca == jog->piecesJogador[i].formato && numero == jog->piecesJogador[i].cor)
            {
                aux = 0;
                pos = i;
                break;
            }
            else{
                aux = 1;
            }
        }
    }    
    int verifica = verificarJogada(board, peca, numero, linha, coluna, n_jogada, todasJogadas);
    if(aux == 0 && verifica == 1){
        board->pieces[linha][coluna].formato = peca; 
        board->pieces[linha][coluna].cor = numero;
        reallocBoard(board, linha, coluna, (*n_jogada*2), todasJogadas);
        //Retirando a peça jogada das peças do jogador
        int k = 0;
        for(i = 0; i < jog->qtdPieces; i++){
            if(i != pos){
                jog->piecesJogador[k].formato = jog->piecesJogador[i].formato;
                jog->piecesJogador[k].cor = jog->piecesJogador[i].cor;
                k++;
            }
        }
        jog->piecesJogador[k].formato = ' ';
        jog->piecesJogador[k].cor = ' ';
        jog->qtdPieces--;
        (*n_jogada)++;      //Atualizando o número da jogada
    }
    else{
        sprintf(msg, "***Jogada invalida***\n");
        send(jog->numSocket, msg, strlen(msg), 0);
    }
}

//Função que verifica se a posição em que a peça vai ser jogada é válida ou não
//Retorna 1 caso seja válida e 0 caso contrário
//Verifica se as casas adjacentes estão ocupadas, se a peça é da mesma cor ou formato que a sua vizinha, se uma linha/coluna já é de uma cor ou formato
// e limita a jogada à mesma linha ou coluna a partir da segunda jogada 
int verificarJogada(pont board, char peca, char numero, int linha, int coluna, int *n_jogada, int *todasJogadas){
    int verifica = 1;   //Variável a ser retornada
    int posAdj = -1;    //Variável que indica qual a posição das peças adjacentes em relação à peça jogada (0 = acima, 1 = esquerda, 2 = direita, 3 = abaixo)
    int pecaIgual = -1;     //Variável que indica se o formato ou a cor são iguais à peça adjacente (1 = formato, 2 = cor)
    
    if(board->columns != 1 && board->rows != 1){
        //Limitando as comparações nas bordas do tabuleiro para que o programa não acesse uma posição inexistente
        if(linha == 0){
            //Caso em que linha e coluna são iguais a 0
            if(coluna == 0){
                if(board->pieces[linha][coluna + 1].formato == ' ' && board->pieces[linha + 1][coluna].formato == ' '){
                    verifica = 0;
                }
                else{
                    //Verificando se o formato e a cor da peça é igual à sua vizinha
                    //Caso seja, a jogada é inválida
                    if(board->pieces[linha][coluna + 1].formato != ' '){
                        if(peca == board->pieces[linha][coluna + 1].formato){
                            if(numero == board->pieces[linha][coluna + 1].cor){
                                verifica = 0;
                            }
                            else{
                                posAdj = 2;
                                pecaIgual = 1;
                            }
                        }
                        else if(numero == board->pieces[linha][coluna + 1].cor){
                            posAdj = 2;
                            pecaIgual = 2;
                        }
                        else{
                            verifica = 0;
                        }
                    }
                    if(board->pieces[linha + 1][coluna].formato != ' '){
                        if(peca == board->pieces[linha + 1][coluna].formato){
                            if(numero == board->pieces[linha + 1][coluna].cor){
                                verifica = 0;
                            }
                            else{
                                posAdj = 3;
                                pecaIgual = 1;
                            }
                        }
                        else if(numero == board->pieces[linha + 1][coluna].cor){
                            posAdj = 3;
                            pecaIgual = 2;
                        }
                        else{
                            verifica = 0;
                        }
                    }
                }
            }
            //Caso em que linha igual a 0 e coluna no tamanho máximo
            else if(coluna == board->columns - 1){
                if(board->pieces[linha][coluna - 1].formato == ' ' && board->pieces[linha + 1][coluna].formato == ' '){
                    verifica = 0;
                }
                else{
                    //Verificando se o formato e a cor da peça é igual à sua vizinha
                    //Caso seja, a jogada é inválida
                    if(board->pieces[linha][coluna - 1].formato != ' '){
                        if(peca == board->pieces[linha][coluna - 1].formato){
                            if(numero == board->pieces[linha][coluna - 1].cor){
                                verifica = 0;
                            }
                            else{
                                posAdj = 1;
                                pecaIgual = 1;
                            }
                        }
                        else if(numero == board->pieces[linha][coluna - 1].cor){
                            posAdj = 1;
                            pecaIgual = 2;
                        }
                        else{
                            verifica = 0;
                        }
                    }
                    if(board->pieces[linha + 1][coluna].formato != ' '){
                        if(peca == board->pieces[linha + 1][coluna].formato){
                            if(numero == board->pieces[linha + 1][coluna].cor){
                                verifica = 0;
                            }
                            else{
                                posAdj = 3;
                                pecaIgual = 1;
                            }
                        }
                        else if(numero == board->pieces[linha + 1][coluna].cor){
                            posAdj = 3;
                            pecaIgual = 2;
                        }
                        else{
                            verifica = 0;
                        }
                    }
                }
            }
            //Caso em que apenas linha igual a 0
            else{
                if(board->pieces[linha][coluna + 1].formato == ' ' && board->pieces[linha][coluna - 1].formato == ' ' && board->pieces[linha + 1][coluna].formato == ' '){
                    verifica = 0;
                }
                else{
                    //Verificando se o formato e a cor da peça é igual à sua vizinha
                    //Caso seja, a jogada é inválida
                    if(board->pieces[linha][coluna + 1].formato != ' '){
                        if(peca == board->pieces[linha][coluna + 1].formato){
                            if(numero == board->pieces[linha][coluna + 1].cor){
                                verifica = 0;
                            }
                            else{
                                posAdj = 2;
                                pecaIgual = 1;
                            }
                        }
                        else if(numero == board->pieces[linha][coluna + 1].cor){ 
                            posAdj = 2;
                            pecaIgual = 2;
                        }     
                        else{
                            verifica = 0;
                        }
                    }
                    if(board->pieces[linha][coluna - 1].formato != ' '){
                        if(peca == board->pieces[linha][coluna - 1].formato){
                            if(numero == board->pieces[linha][coluna - 1].cor){
                                verifica = 0;
                            }
                            else{
                                posAdj = 1;
                                pecaIgual = 1;
                            }
                        }
                        else if(numero == board->pieces[linha][coluna - 1].cor){
                            posAdj = 1;
                            pecaIgual = 2;
                        }
                        else{
                            verifica = 0;
                        }
                    }
                    if(board->pieces[linha + 1][coluna].formato != ' '){
                        if(peca == board->pieces[linha + 1][coluna].formato){
                            if(numero == board->pieces[linha + 1][coluna].cor){
                                verifica = 0;
                            }
                            else{
                                posAdj = 3;
                                pecaIgual = 1;
                            }
                        }
                        else if(numero == board->pieces[linha + 1][coluna].cor){
                            posAdj = 3;
                            pecaIgual = 2;
                        }
                        else{
                            verifica = 0;
                        }
                    }
                }
            }
        }
        else if(linha == board->rows - 1){
            //Caso em que linha igual ao tamanho máximo e coluna igual a 0
            if(coluna == 0){
                if(board->pieces[linha][coluna + 1].formato == ' ' && board->pieces[linha - 1][coluna].formato == ' '){
                    verifica = 0;
                }
                else{
                    //Verificando se o formato e a cor da peça é igual às suas vizinhas
                    //Caso seja, a jogada é inválida
                    if(board->pieces[linha][coluna + 1].formato != ' '){
                        if(peca == board->pieces[linha][coluna + 1].formato){
                            if(numero == board->pieces[linha][coluna + 1].cor){
                                verifica = 0;
                            }
                            else{
                                posAdj = 2;
                                pecaIgual = 1;
                            }
                        }
                        else if(numero == board->pieces[linha][coluna + 1].cor){ 
                            posAdj = 2;
                            pecaIgual = 2;
                        } 
                        else{
                            verifica = 0; 
                        }
                    }
                    if(board->pieces[linha - 1][coluna].formato != ' '){
                        if(peca == board->pieces[linha - 1][coluna].formato){
                            if(numero == board->pieces[linha - 1][coluna].cor){
                                verifica = 0;
                            }
                            else{
                                posAdj = 0;
                                pecaIgual = 1;
                            }
                        }
                        else if(numero == board->pieces[linha - 1][coluna].cor){
                            posAdj = 0;
                            pecaIgual = 2;
                        }
                        else{
                            verifica = 0;
                        }
                    }
                }
            }
            //Caso em que linha e coluna são iguais ao tamanho máximo
            else if(coluna == board->columns - 1){
                if(board->pieces[linha][coluna - 1].formato == ' ' && board->pieces[linha - 1][coluna].formato == ' '){
                    verifica = 0;
                }
                else{
                    //Verificando se o formato e a cor da peça é igual à sua vizinha
                    //Caso seja, a jogada é inválida
                    if(board->pieces[linha][coluna - 1].formato != ' '){
                        if(peca == board->pieces[linha][coluna - 1].formato){
                            if(numero == board->pieces[linha][coluna - 1].cor){
                                verifica = 0;
                            }
                            else{
                                posAdj = 1;
                                pecaIgual = 1;
                            }
                        }
                        else if(numero == board->pieces[linha][coluna - 1].cor){
                            posAdj = 1;
                            pecaIgual = 2;
                        }
                        else{
                            verifica = 0;
                        }
                    }
                    if(board->pieces[linha - 1][coluna].formato != ' '){
                        if(peca == board->pieces[linha - 1][coluna].formato){
                            if(numero == board->pieces[linha - 1][coluna].cor){
                                verifica = 0;
                            }
                            else{
                                posAdj = 0;
                                pecaIgual = 1;
                            }
                        }
                        else if(numero == board->pieces[linha - 1][coluna].cor){
                            posAdj = 0;
                            pecaIgual = 2;
                        }
                        else{
                            verifica = 0;
                        }
                    }
                }
            }
            //Caso em que apenas a linha é do tamanho máximo
            else{
                if(board->pieces[linha][coluna + 1].formato == ' ' && board->pieces[linha][coluna - 1].formato == ' ' && board->pieces[linha - 1][coluna].formato == ' '){
                    verifica = 0;
                }
                else{
                    //Verificando se o formato e a cor da peça é igual à sua vizinha
                    //Caso seja, a jogada é inválida
                    if(board->pieces[linha][coluna + 1].formato != ' '){
                        if(peca == board->pieces[linha][coluna + 1].formato){
                            if(numero == board->pieces[linha][coluna + 1].cor){
                                verifica = 0;
                            }
                            else{
                                posAdj = 2;
                                pecaIgual = 1;
                            }
                        }
                        else if(numero == board->pieces[linha][coluna + 1].cor){ 
                            posAdj = 2;
                            pecaIgual = 2;
                        } 
                        else{
                            verifica = 0;
                        }
                    }
                    if(board->pieces[linha][coluna - 1].formato != ' '){
                        if(peca == board->pieces[linha][coluna - 1].formato){
                            if(numero == board->pieces[linha][coluna - 1].cor){
                                verifica = 0;
                            }
                            else{
                                posAdj = 1;
                                pecaIgual = 1;
                            }
                        }
                        else if(numero == board->pieces[linha][coluna - 1].cor){
                            posAdj = 1;
                            pecaIgual = 2;
                        }
                        else{
                            verifica = 0;
                        }
                    }
                    if(board->pieces[linha - 1][coluna].formato != ' '){
                        if(peca == board->pieces[linha - 1][coluna].formato){
                            if(numero == board->pieces[linha - 1][coluna].cor){
                                verifica = 0;
                            }
                            else{
                                posAdj = 0;
                                pecaIgual = 1;
                            }
                        }
                        else if(numero == board->pieces[linha - 1][coluna].cor){
                            posAdj = 0;
                            pecaIgual = 2;
                        }
                        else{
                            verifica = 0;
                        }
                    }
                }
            }
        }
        //Caso em que apenas a coluna é igual a 0
        else if(coluna == 0){
            if(board->pieces[linha][coluna + 1].formato == ' ' && board->pieces[linha + 1][coluna].formato == ' ' && board->pieces[linha - 1][coluna].formato == ' '){
                verifica = 0;
            }
            else{
                //Verificando se o formato e a cor da peça é igual à sua vizinha
                //Caso seja, a jogada é inválida
                if(board->pieces[linha][coluna + 1].formato != ' '){
                    if(peca == board->pieces[linha][coluna + 1].formato){
                        if(numero == board->pieces[linha][coluna + 1].cor){
                            verifica = 0;
                        }
                        else{
                            posAdj = 2;
                            pecaIgual = 1;
                        }
                    }
                    else if(numero == board->pieces[linha][coluna + 1].cor){
                        posAdj = 2;
                        pecaIgual = 2;
                    } 
                    else{
                        verifica = 0;
                    }
                }
                if(board->pieces[linha + 1][coluna].formato != ' '){
                    if(peca == board->pieces[linha + 1][coluna].formato){
                        if(numero == board->pieces[linha + 1][coluna].cor){
                            verifica = 0;
                        }
                        else{
                            posAdj = 3;
                            pecaIgual = 1;
                        }
                    }
                    else if(numero == board->pieces[linha + 1][coluna].cor){
                        posAdj = 3;
                        pecaIgual = 2;
                    }
                    else{
                        verifica = 0;
                    }
                }
                if(board->pieces[linha - 1][coluna].formato != ' '){
                    if(peca == board->pieces[linha - 1][coluna].formato){
                        if(numero == board->pieces[linha - 1][coluna].cor){
                            verifica = 0;
                        }
                        else{
                            posAdj = 0;
                            pecaIgual = 1;
                        }
                    }
                    else if(numero == board->pieces[linha - 1][coluna].cor){
                        posAdj = 0;
                        pecaIgual = 2;
                    }
                    else{
                        verifica = 0;
                    }
                }
            }
        }
        //Caso em que apenas a coluna é igual ao seu tamanho máximo
        else if(coluna == board->columns - 1){
            if(board->pieces[linha][coluna - 1].formato == ' ' && board->pieces[linha + 1][coluna].formato == ' ' && board->pieces[linha - 1][coluna].formato == ' '){
                verifica = 0;
            }
            else{
                //Verificando se o formato e a cor da peça é igual à sua vizinha
                //Caso seja, a jogada é inválida
                if(board->pieces[linha][coluna - 1].formato != ' '){
                    if(peca == board->pieces[linha][coluna - 1].formato){
                        if(numero == board->pieces[linha][coluna - 1].cor){
                            verifica = 0;
                        }
                        else{
                            posAdj = 1;
                            pecaIgual = 1;
                        }
                    }
                    else if(numero == board->pieces[linha][coluna - 1].cor){
                        posAdj = 1;
                        pecaIgual = 2;
                    }
                    else{
                        verifica = 0;
                    }
                }
                if(board->pieces[linha + 1][coluna].formato != ' '){
                    if(peca == board->pieces[linha + 1][coluna].formato){
                        if(numero == board->pieces[linha + 1][coluna].cor){
                            verifica = 0;
                        }
                        else{
                            posAdj = 3;
                            pecaIgual = 1;
                        }
                    }   
                    else if(numero == board->pieces[linha + 1][coluna].cor){
                        posAdj = 3;
                        pecaIgual = 2;
                    }
                    else{
                        verifica = 0;
                    }
                }
                if(board->pieces[linha - 1][coluna].formato != ' '){
                    if(peca == board->pieces[linha - 1][coluna].formato){
                        if(numero == board->pieces[linha - 1][coluna].cor){
                            verifica = 0;
                        }
                        else{
                            posAdj = 0;
                            pecaIgual = 1;
                        }
                    }
                    else if(numero == board->pieces[linha - 1][coluna].cor){
                        posAdj = 0;
                        pecaIgual = 2;
                    }
                    else{
                        verifica = 0;
                    }
                }
            }
        }
        //Caso em que a posição não é em nenhuma borda do tabuleiro
        else{
            if(board->pieces[linha][coluna + 1].formato == ' ' && board->pieces[linha][coluna - 1].formato == ' ' && board->pieces[linha + 1][coluna].formato == ' ' && board->pieces[linha - 1][coluna].formato == ' '){
                verifica = 0;
            }
            else{
                //Verificando se o formato e a cor da peça é igual à sua vizinha
                //Caso seja, a jogada é inválida
                if(board->pieces[linha][coluna + 1].formato != ' '){
                    if(peca == board->pieces[linha][coluna + 1].formato){
                        if(numero == board->pieces[linha][coluna + 1].cor){
                            verifica = 0;
                        }
                        else{
                            posAdj = 2;
                            pecaIgual = 1;
                        }
                    }
                    else if(numero == board->pieces[linha][coluna + 1].cor){
                        posAdj = 2;
                        pecaIgual = 2;
                    }
                    else{
                        verifica = 0;
                    } 
                }
                if(board->pieces[linha][coluna - 1].formato != ' '){
                    if(peca == board->pieces[linha][coluna - 1].formato){
                        if(numero == board->pieces[linha][coluna - 1].cor){
                            verifica = 0;
                        }
                        else{
                            posAdj = 1;
                            pecaIgual = 1;
                        }
                    }
                    else if(numero == board->pieces[linha][coluna - 1].cor){
                        posAdj = 1;
                        pecaIgual = 2;
                    }
                    else{
                        verifica = 0;
                    }
                }
                if(board->pieces[linha + 1][coluna].formato != ' '){
                    if(peca == board->pieces[linha + 1][coluna].formato){
                        if(numero == board->pieces[linha + 1][coluna].cor){
                            verifica = 0;
                        }
                        else{
                            posAdj = 3;
                            pecaIgual = 1;
                        }
                    }
                    else if(numero == board->pieces[linha + 1][coluna].cor){
                        posAdj = 3;
                        pecaIgual = 2;
                    }
                    else{
                        verifica = 0;
                    }
                }
                if(board->pieces[linha - 1][coluna].formato != ' '){
                    if(peca == board->pieces[linha - 1][coluna].formato){
                        if(numero == board->pieces[linha - 1][coluna].cor){
                            verifica = 0;
                        }
                        else{
                            posAdj = 0;
                            pecaIgual = 1;
                        }
                    }
                    else if(numero == board->pieces[linha - 1][coluna].cor){
                        posAdj = 0;
                        pecaIgual = 2;
                    }
                    else{
                        verifica = 0;
                    }
                }
            }
        }
    }
    //Verifica se a linha ou coluna já é de uma cor ou formato
    int aux_linha, aux_coluna;
    if(verifica == 1){
        if(posAdj == 0){
            aux_linha = linha - 2;
            aux_coluna = coluna;
            while(aux_linha > 0 && board->pieces[aux_linha][aux_coluna].formato != ' ' && board->pieces[aux_linha][aux_coluna].cor != ' '){
                if(pecaIgual == 1){
                    if(board->pieces[aux_linha][aux_coluna].formato != peca){
                        verifica = 0;
                        break;
                    }
                    else if(board->pieces[aux_linha][aux_coluna].cor == numero){
                        verifica = 0;
                        break;
                    }
                }
                else{
                    if(board->pieces[aux_linha][aux_coluna].cor != numero){
                        verifica = 0;
                        break;
                    }
                    else if(board->pieces[aux_linha][aux_coluna].formato == peca){
                        verifica = 0;
                        break;
                    }
                }
                aux_linha--;
            }
        }
        else if(posAdj == 1){
            aux_linha = linha;
            aux_coluna = coluna - 2;
            while(aux_coluna > 0 && board->pieces[aux_linha][aux_coluna].formato != ' ' && board->pieces[aux_linha][aux_coluna].cor != ' '){
                if(pecaIgual == 1){
                    if(board->pieces[aux_linha][aux_coluna].formato != peca){
                        verifica = 0;
                        break;
                    }
                    else if(board->pieces[aux_linha][aux_coluna].cor == numero){
                        verifica = 0;
                        break;
                    }
                }
                else{
                    if(board->pieces[aux_linha][aux_coluna].cor != numero){
                        verifica = 0;
                        break;
                    }
                    else if(board->pieces[aux_linha][aux_coluna].formato == peca){
                        verifica = 0;
                        break;
                    }
                }
                aux_coluna--;
            }
        }
        else if(posAdj == 2){
            aux_linha = linha;
            aux_coluna = coluna + 2;
            while(aux_coluna < board->columns && board->pieces[aux_linha][aux_coluna].formato != ' ' && board->pieces[aux_linha][aux_coluna].cor != ' '){
                if(pecaIgual == 1){
                    if(board->pieces[aux_linha][aux_coluna].formato != peca){
                        verifica = 0;
                        break;
                    }
                    else if(board->pieces[aux_linha][aux_coluna].cor == numero){
                        verifica = 0;
                        break;
                    }
                }
                else{
                    if(board->pieces[aux_linha][aux_coluna].cor != numero){
                        verifica = 0;
                        break;
                    }
                    else if(board->pieces[aux_linha][aux_coluna].formato == peca){
                        verifica = 0;
                        break;
                    }
                }
                aux_coluna++;
            }
        }
        else if(posAdj == 3){
            aux_linha = linha + 2;
            aux_coluna = coluna;
            while(aux_linha < board->rows && board->pieces[aux_linha][aux_coluna].formato != ' ' && board->pieces[aux_linha][aux_coluna].cor != ' '){
                if(pecaIgual == 1){
                    if(board->pieces[aux_linha][aux_coluna].formato != peca){
                        verifica = 0;
                        break;
                    }
                    else if(board->pieces[aux_linha][aux_coluna].cor == numero){
                        verifica = 0;
                        break;
                    }
                }
                else{
                    if(board->pieces[aux_linha][aux_coluna].cor != numero){
                        verifica = 0;
                        break;
                    }
                    else if(board->pieces[aux_linha][aux_coluna].formato == peca){
                        verifica = 0;
                        break;
                    }
                }
                aux_linha++;
            }
        }
    }
    if(verifica == 1){
        int tam = (*n_jogada) * 2, tam_aux = ((*n_jogada) - 1) * 2, i;
        if((*n_jogada) > 1){
            //Limitando a nova jogada à mesma linha ou coluna das jogadas anteriores
            if((*n_jogada) > 2){
                if(todasJogadas[0] == todasJogadas[2]){
                    if(linha != todasJogadas[2]){
                        verifica = 0;
                    }
                }
                else if(todasJogadas[1] == todasJogadas[3]){
                    if(coluna != todasJogadas[3]){
                        verifica = 0;
                    }
                }
            }
        }
        //Acresecentando a nova jogada ao vetor
        todasJogadas[tam - 2] = linha;
        todasJogadas[tam - 1] = coluna;
    }
    return verifica;
}

//Função que realiza a leitura e interpretação dos comandos e direciona para as funções de trocar, jogar ou passar
void leituraComandos(pont board, pontDeque monte, pontJogadores jog, pontJogadores inicio, char cheat)
{
    //Definição das strings para comparação com os comandos lidos
    char trocar[] = "TROCAR";
    char jogar[] = "JOGAR";
    char passar[] = "PASSAR";
    char msg[256];
    int recebidos, enviados;

    int aux = 0, num_jogada = 1, *n_jogada;
    n_jogada = &num_jogada;     //Ponteiro que aponta para a variável que armazena o número da jogada atual do jogador
    int *todasJogadas = (int *)malloc(12 * sizeof(int)); //Vetor que armazena as posições das jogadas do jogador
    
    //Inteiro utilizado para saber quantos pontos forma obtidos pelo jogador nessa rodada
    int pontosAtual = jog->pontos;

    while(aux != -1)
    {   
        if(aux == 0 || jog->qtdPieces == 0){
            jog->qtdPieces = 6;     //Setando a quantidade de peças iniciais somente em cada nova jogada
            *n_jogada = 1;          //Setando uma nova jogada 
        }
        //Imprime na tela as informações da interface
        //send(jog->numSocket,"LIMPAR",7,0);
        printBoard(board, inicio);
        sprintf(msg, "===========\n");
        send(jog->numSocket, msg, strlen(msg), 0);
        printarJog(jog);

        //Variáveis para armazenar os comando dado e a parte do comando que representa a função
        char comando[26];
        char funcao[7];
        int i = 0;

        //A variável serve para controlar o tipo de interface que varia de acordo com o primeiro comando dado
        //Aux == 0: interface com a opção de troca
        if(aux == 0)
        {
            sprintf(msg, "Opcoes: trocar p1 [p2 p3...] | jogar p1 x y | passar\n");
            send(jog->numSocket, msg, strlen(msg), 0);
            recebidos = recv(jog->numSocket, comando, 25, 0);
            comando[recebidos] = '\0';
            //fgetss(comando, 25, "Comando");

            //Aqui converte-se as letras minúsculas para maiúsculas a fim de compará-las com as strings definidas de comandos
            toUpper(comando);

            //Aqui é separado a funçaõ do comando em si
            while(comando[i] != ' ' && i < 6)
            {
                funcao[i] = comando[i];
                i++;
            }
            funcao[i] = '\0';

            //Caso seja uma troca
            if(!strcmp(funcao, trocar))
            {
                //auxTrocar repsenta a posição do formato das peças a serem trocadas
                int auxTrocar = i+1;
                int tamString = strlen(comando);
                //Vetor para armazenar as informações a serem trocadas
                piece pecasTroca[6];
                int contPecas = 0;
                while(auxTrocar+1 < tamString)
                {
                    //Verifica-se a peça é válida
                    if(verificaPeca(comando[auxTrocar], comando[auxTrocar+1]))
                    {
                        pecasTroca[contPecas].formato = comando[auxTrocar];
                        pecasTroca[contPecas].cor = comando[auxTrocar+1];
                        contPecas++;
                        aux = -1;

                        //Incrementa-se 3 para que a posição seja o do próximo formato a ser trocado
                        auxTrocar += 3;
                    }
                    else
                    {   
                        sprintf(msg, "Comando invalido!\n");
                        send(jog->numSocket, msg, strlen(msg), 0);
                        aux = 0;
                        break;
                    }
                }
                //Caso em que as peças a serem trocadas são todas válidas
                if(aux == -1)
                {
                    //A função verificaTroca realiza as trocas se todas as peças estão realmente entre as peças do jogador
                    aux = verificaTroca(monte, jog, pecasTroca, contPecas);
                }
            }
            else if(!strcmp(funcao, jogar))
            {
                //Lendo a peça e a posição em que será jogada
                int linha, coluna, j, cont = 0, k = 0;
                char jogada[4];
                for(j = 9; j < 24; j++){
                    if(comando[j] == '\0' || comando[j] == ' '){
                        break;
                    }
                    jogada[cont] = comando[j];
                    cont++;
                }
                jogada[cont] = '\0';
                if(cont == 1){
                    linha = jogada[0] - '0';    //Transformando a linha, de char para int
                }
                else{
                    linha = atoi(jogada);       //Transformando a linha, de string para int
                }
                j++;
                cont = 0;
                for(k = j; k < 24; k++){
                    if(comando[k] == '\0' || comando[k] == ' '){
                        break;
                    }
                    jogada[cont] = comando[k];
                    cont++;
                }
                jogada[cont] = '\0';
                if(cont == 1){
                    coluna = jogada[0] - '0';   //Transformando a coluna, de char para int
                }
                else{
                    coluna = atoi(jogada);      //Transformando a coluna, de string para int
                }
                if(cheat == 'S'){
                    cheatMode(board, monte, jog, comando[6], comando[7], linha, coluna, n_jogada, todasJogadas);
                }
                else{
                    joga(board, monte, jog, comando[6], comando[7], linha, coluna, n_jogada, todasJogadas);    //Chamando a função para jogar
                }
                aux = 2;
            }
            //Para passar basta sair do loop com aux = -1
            else if(!strcmp(funcao, passar))
            {
                aux = -1;
            }
            else
            {
                sprintf(msg, "Comando invalido!\n");
                send(jog->numSocket, msg, strlen(msg), 0);
            }
        }
        //Aqui temos a segunda interface que aparece quando o jogador posiciona alguma peça no tabuleiro
        else if(aux == 2 && jog->qtdPieces != 0)
        {   
            sprintf(msg, "Opcoes: jogar p1 x y | passar\n");
            send(jog->numSocket, msg, strlen(msg), 0);
            recebidos = recv(jog->numSocket, comando, 25, 0);
            comando[recebidos] = '\0';
            //fgetss(comando, 24, "Comando");
            toUpper(comando);
            while(comando[i] != ' ' && i < 7)
            {
                funcao[i] = comando[i];
                i++;
            }
            funcao[i] = '\0';
            if(!strcmp(funcao, jogar))
            {
                //Lendo a peça e a posição em que será jogada
                int linha, coluna, j, cont = 0, k = 0;
                char jogada[4];
                for(j = 9; j < 24; j++){
                    if(comando[j] == '\0' || comando[j] == ' '){
                        break;
                    }
                    jogada[cont] = comando[j];
                    cont++;
                }
                jogada[cont] = '\0';
                if(cont == 1){
                    linha = jogada[0] - '0';    //Transformando a linha, de char para int
                }
                else{
                    linha = atoi(jogada);       //Transformando a linha, de string para int
                }
                j++;
                cont = 0;
                for(k = j; k < 24; k++){
                    if(comando[k] == '\0' || comando[k] == ' '){
                        break;
                    }
                    jogada[cont] = comando[k];
                    cont++;
                }
                jogada[cont] = '\0';
                if(cont == 1){
                    coluna = jogada[0] - '0';   //Transformando a coluna, de char para int
                }
                else{
                    coluna = atoi(jogada);      //Transformando a coluna, de string para int
                }
                if(cheat == 'S'){
                    cheatMode(board, monte, jog, comando[6], comando[7], linha, coluna, n_jogada, todasJogadas);
                }
                else{
                    joga(board, monte, jog, comando[6], comando[7], linha, coluna, n_jogada, todasJogadas);    //Chamando a função para jogar
                }
                aux = 2;
            }
            //A função passar depois que o jogador realiza pelo menos um jogada é o momento em que a função de contar pontos é acionada, pois é mais simples contar os pontos com todas as jogadas já realizadas
            //É realizada também a reposição das peças utilizadas pelo jogador na rodada
            else if(!strcmp(funcao, passar))
            {
                //Aqui conta-se os pontos
                contarPontos(jog, todasJogadas, (num_jogada-1)*2, board);
                //Aqui realiza-se as reposições das peças
                if(jog->qtdPieces < 6){
                    reporPiecesJog(jog, monte);
                }
                aux = -1;   
            }
            else
            {
                sprintf(msg, "Comando invalido!\n");
                send(jog->numSocket, msg, strlen(msg), 0);
            }
        }
    }
    //Aqui é mostrado a pontuação do jogador e a pontuação acumulada na rodada
   //sprintf(msg, "");
    //send(jog->numSocket, msg, strlen(msg), 0);
    sprintf(msg, "***********************************************\n*** Pontuacao %s: %d / Pontuacao acumulada: %d\n*********************************************\n", jog->nome, jog->pontos, (jog->pontos-pontosAtual));
    enviados = send(jog->numSocket, msg, strlen(msg), 0);
    //sprintf(msg, "*********************************************\n");
    //send(jog->numSocket, msg, strlen(msg), 0);
    free(todasJogadas);
}

//Função para leitura de string que não tem problema de buffer e não dá segfault se passar do limite
//str é vetor onde quer armazenar e n é a última posição do possível do vetor
void fgetss(char str[], int n, char tipoComando[])
{
  char a;
  int i;
  //Aqui são lidos os caracteres possíveis e armazenadas na string
  for(i = 0; i < n; i++)
  {
    a = getc(stdin);
    if(a == '\n')
    {
      str[i] = '\0';
      break;
    }
    str[i] = a;
  }
  i = 0;
  //Aqui são lidos os caracteres excedentes
  while(a != '\n')
  {
    if(i == 1)
    {
      printf("%s maior que o limite! Serao considerados apenas os primeiros %d caracteres.\n", tipoComando, n);
      str[n] = '\0';
    }
    a = getc(stdin);
    i++;
  }
}

//Função responsável por converter as letras minúsculas em maiúsculas de um string
void toUpper(char str[])
{
    int tam = strlen(str);
    for(int i = 0; i <= tam; i++){
        //Identifica-se que é uma letra minúscula pelo código ASCII
        if((str[i] >= 97 ) && (str[i] <= 122 ))
        {
            //Aqui é feita a conversão
            str[i] = str[i] - 32;            
        }
    }
}

//Retorna 1 se os caracteres de formato e cor forem válidos e 0 para o contrário
int verificaPeca(char formato, char cor)
{
    //Definição dos vetores contendo os formatos e cores possíveis
    char formatos[] = {'Q','C','L', 'E', 'F', 'S'};
    char cores[] = {'1', '2', '3', '4', '5', '6'};

    int resFormato = 0, resCor = 0, i;
    //Verificação do formato
    for(i = 0; i < 6 && resFormato == 0; i++)
    {
        if(formatos[i] == formato)
        {
            resFormato = 1;
        }
    }
    //Verificação da cor
    for(i = 0; i < 6 && resCor == 0; i++)
    {
        if(cores[i] == cor)
        {
            resCor = 1;
        }
    }
    //Retorna 1 somente se o formato e cor forem válidos
    if(resCor == 1 && resFormato == 1)
    {
        return 1;
    }
    return 0;
}

//Função que realiza as trocas somente se todas as peças a serem trocadas são peaçs do jogador
//As peças a serem trocadas estão contidas no vetor pecas
int verificaTroca(pontDeque monte, pontJogadores jog, piece pecas[], int qtdPecasTtroca)
{
    char msg[256];
    int cont = 0;

    //Vetor que armazena as posições do vetor de peças do jogador que estão as peças que serão trocadas
    int posTroca[6];

    //For para identificar as peças a serem trocadas no vetor de peças do jogador
    for(int i = 0; i < qtdPecasTtroca; i++)
    {
        for(int j = 0; j < jog->qtdPieces; j++)
        {
            if(pecas[i].formato == jog->piecesJogador[j].formato)
            {
                if(pecas[i].cor == jog->piecesJogador[j].cor)
                {
                    posTroca[cont] = j;
                    cont++;
                }
            }
        }
    }
    //Verificação se todas as trocas a serem realizadas são possíveis
    if(cont >= qtdPecasTtroca)
    {
        //Realiza-se as trocas utilizando a funçaõ trocaPiecesJog
        sprintf(msg, "Realizando troca de: ");
        send(jog->numSocket, msg, strlen(msg), 0);
        int k;
        for(k = 0; k < qtdPecasTtroca; k++)
        {
            sprintf(msg, "[%c%c] ", pecas[k].formato, pecas[k].cor);
            send(jog->numSocket, msg, strlen(msg), 0);
        }
        sprintf(msg, "\n");
        send(jog->numSocket, msg, strlen(msg), 0);
        for(k = 0; k < cont; k++)
        {
            trocarPiecesJog(monte, jog, posTroca[k]);
        }
        return -1;
    }
    //Caso em que pelo menos uma troca não é possível
    else
    {
        sprintf(msg,"Pecas invalidas para troca!\n");
        send(jog->numSocket, msg, strlen(msg), 0);
        return 0;
    }
    
}

//Função para alocar memória para os jogadores, distribuir peças as peça e zerar os pontos
pontJogadores iniciarJogs(pontDeque monte, int qtdJogs, int sockets[2])
{
    int i = 0;
    //Ponteiro necessários para organizar a ordem dos jogadores
    pontJogadores jogador, jogAnterior = NULL, inicioJog;

    while(i < qtdJogs)
    {
        //Aloca-se memória
        jogador = (pontJogadores)malloc(sizeof(jogadores));
        //Caso do primeiro jogador
        if(i == 0)
        {
            inicioJog = jogador;
        }
        //Caso para os outros jogadores
        if(jogAnterior != NULL)
        {
            jogAnterior->proxJog = jogador;
        }
        //Distribuição das peças e zera-se os pontos
        iniciarPiecesJog(monte, jogador);
        jogador->pontos = 0;
        jogador->proxJog = NULL;
        jogador->numSocket = sockets[i];
        jogAnterior = jogador;
        i++;
    }
    return inicioJog;
}

//Função para selecionar o vencedor. O primeiro critério é a maior quantidade de pontos. O segundo critério, se houver empate, é a menor quantidade de peças disponíveis no vetor de peças do jogador.
//Se houver empate novamente, os jogadores empatados são declarados vencedores
void selecionarVencedor(pontJogadores inicio)
{
    //Inteiros que armazeranarão, respectivamente, a maior quantidade de pontos, a quantidade de peças do jogador com a maior quantidade de peças e a quantidade de vencedores
    int maiorPontos = 0, qtdPiecesVencedor = 0, qtdVencedores = 0;

    char nome[21];
    char msg[256];
    pontJogadores jogAtual = inicio;

    //Percorre-se todos os jogadores
    while(jogAtual != NULL)
    {
        //Caso em que a quantidade de pontos desse jogador é maior que a maior quantidade de pontos até então
        if(jogAtual->pontos > maiorPontos)
        {
            maiorPontos = jogAtual->pontos;
            qtdPiecesVencedor = jogAtual->qtdPieces;
            qtdVencedores = 1;
        }
        //Caso em que os pontos são iguais
        else if(jogAtual->pontos == maiorPontos)
        {
            //Caso em que a quantidade de peças desse jogador é menor que a do jogador com o qual ele está empatado em pontos
            if(jogAtual->qtdPieces < qtdPiecesVencedor)
            {
                qtdPiecesVencedor = jogAtual->qtdPieces;
                qtdVencedores = 1;
            }
            //Caso em que são iguais, aumenta-se o nuemro de vencedores
            else if(jogAtual->qtdPieces == qtdPiecesVencedor)
            {
                qtdVencedores++;
            }
        }
        jogAtual = jogAtual->proxJog;
    }

    jogAtual = inicio;

    //Imprime na tela o vencedor ou vencedores
    while(jogAtual != NULL)
    {
        if(jogAtual->pontos == maiorPontos && jogAtual->qtdPieces == qtdPiecesVencedor)
        {
            sprintf(msg, "Você ganhou! Acumulou %d pontos e sobraram %d pecas\n", jogAtual->pontos, jogAtual->qtdPieces);
            send(jogAtual->numSocket, msg, strlen(msg), 0);
        }
        else{
            sprintf(msg, "Infelizmente não foi dessa vez ;-;\n");
            send(jogAtual->numSocket, msg, strlen(msg), 0);
        }
        jogAtual = jogAtual->proxJog;
    }
}

//Função que repõe as peças de um jogador após pelo menos uma jogada
void reporPiecesJog(pontJogadores jog, pontDeque monte)
{
    while(monte->qtdCartas > 0 && jog->qtdPieces < 6)
    {   
        //Como depois de uma jogada, as peças são realocadas para a esquerda, a quantidade de peças do jogador também é a primeira posição vazia

        //Aqui as informações da carta no início do monte são copiadas para a primeira posição vazia
        jog->piecesJogador[jog->qtdPieces].formato = monte->inicio->info.formato;
        jog->piecesJogador[jog->qtdPieces].cor = monte->inicio->info.cor;
        jog->qtdPieces++;

        //Remanejamento do ponteiro que aponta para a carta incial do motne
        pontCarta temp = monte->inicio;
        monte->inicio = monte->inicio->prox;
        monte->qtdCartas--;
        free(temp);
    }
}

//Função que realiza a jogada quando o cheat mode foi selecionado: verifica se a peça está disponível no vetor de peças do jogador, retira do monte caso não esteja,
//Verifica se posição é válida, armazena a posição jogada no vetor todasJogadas, incrementa o número da jogada e por fim realoca as peças do jogador para a direita
void cheatMode(pont board, pontDeque monte, pontJogadores jog, char peca, char numero, int linha, int coluna, int *n_jogada, int *todasJogadas)
{  
    char msg[256];
    int i, aux = 0, pos = -1;
    if(linha >= board->rows || coluna >= board->columns || linha < 0 || coluna < 0 || board->pieces[linha][coluna].cor != ' ' || board->pieces[linha][coluna].formato != ' '){
        aux = 1;
    }
    else{
        //Verifica se a peça jogada está entre as peças do jogador
        for(i = 0; i < jog->qtdPieces; i++)
        {   
            if(peca == jog->piecesJogador[i].formato && numero == jog->piecesJogador[i].cor)
            {
                aux = 0;
                pos = i;
                break;
            }
            else{
                aux = 1;
            }
        }
        //Se a peça não estiver entre as peças do jogador, procura no monte
        if(aux == 1){
            pontCarta carta = monte->inicio;
            pontCarta anterior = NULL;
            pontCarta apagar;
            while(carta != NULL && monte->qtdCartas != 0)
            {   
                if(carta->info.formato == peca && carta->info.cor == numero)
                {
                    if(anterior == NULL)
                    {
                        monte->inicio = carta->prox;
                    }
                    else
                    {
                        anterior->prox = carta->prox;
                    }
                    aux = 0;
                    apagar = carta;
                    free(apagar);
                    carta = NULL;
                    monte->qtdCartas--;
                    break;
                }
                else
                {
                    aux = 1;
                    anterior = carta;
                    carta = carta->prox;
                }
            }
        }
    }    
    int verifica = verificarJogada(board, peca, numero, linha, coluna, n_jogada, todasJogadas);
    if(aux == 0 && verifica == 1){
        board->pieces[linha][coluna].formato = peca; 
        board->pieces[linha][coluna].cor = numero;
        reallocBoard(board, linha, coluna, (*n_jogada*2), todasJogadas);
        //Caso a peça tenha sido encontrada no deck do jogador
        if(pos != -1){
            //Retirando a peça jogada das peças do jogador
            int k = 0;
            for(i = 0; i < jog->qtdPieces; i++){
                if(i != pos){
                    jog->piecesJogador[k].formato = jog->piecesJogador[i].formato;
                    jog->piecesJogador[k].cor = jog->piecesJogador[i].cor;
                    k++;
                }
            }
            jog->piecesJogador[k].formato = ' ';
            jog->piecesJogador[k].cor = ' ';
            jog->qtdPieces--;
        }
        (*n_jogada)++;      //Atualizando o número da jogada
    }
    else{
        sprintf(msg, "***Jogada invalida***\n");
        send(jog->numSocket, msg, strlen(msg), 0);
    }
}

//Função que realiza a contagem de pontos, depende do vetor que contém todas as posições que o jogador colocou uma peça
void contarPontos(pontJogadores jog, int *todasJogadas, int tam, pont board)
{
    //Inteiros que se forem iguais a iguais a zero, representam que as peças foram organizadas em linha ou em coluna
    int jogadaHorizontal, jogadaVertical;

    //Armazena-se as coordenadas da última posição jogada
    int ultimaPosicaoH = todasJogadas[tam-2];
    int ultimaPosicaoV = todasJogadas[tam-1];

    //Os inteiros representam, respectivamente, os pontos acumulados na rodada, os pontos acumulados na horizontal e pontos acumulados na vertical
    int auxPontos = 0, auxPontosH = 0, auxPontosV = 0;

    //Caso em que foi realizado pelo menos uma jogada
    if(tam >= 4)
    {
        //Cálculo para sabe se a jogada foi horizontal ou vertical
        jogadaHorizontal = todasJogadas[tam-2] - todasJogadas[tam-4];
        jogadaVertical = todasJogadas[tam-1] - todasJogadas[tam-3];

        //Caso em que as peças foram colocadas em linha
        if(jogadaHorizontal == 0)
        {
            //Cálculo dos pontos acumulados apenas com as peças jogadas, soma-se 1 para contar a última posição
            auxPontosH = contarPosicoesHorizontal(board, ultimaPosicaoH, ultimaPosicaoV) + 1;
    
            //Aqui verfica-se se um qwirkle foi realizado
            if( auxPontosH == 6)
            {
                auxPontos = auxPontosH*2;
            }
            else
            {
                auxPontos = auxPontosH;
            }

            int k = 0;
            //Aqui conta-se os pontos acumulados na vertical de cada posição colocada na horizontal 
            while(k < tam)
            {
                //Não é somado 1, pois os pontos na vertical são contabilizados se houver pelo menos uma outra peça conectada à coordenada atual
                auxPontosV = contarPosicoesVertical(board, todasJogadas[k], todasJogadas[k+1]);

                //Caso em que há um qwirkle
                if(auxPontosV == 5)
                {
                    auxPontos += (auxPontosV+1)*2;
                }
                else if(auxPontosV >= 1)
                {
                    auxPontos += auxPontosV+1;
                }

                //Cada duas posições do vetor de todas as jogadas representa uma coordenada, por causa disso o incremento de 2
                k+=2;
            }
            jog->pontos += auxPontos;
        }
        //Caso em que as peças foram posicionadas em coluna
        else if(jogadaVertical == 0)
        {
            //Conta-se os pontos acumulados na vertical, contando a posição em si
            auxPontosV = contarPosicoesVertical(board, ultimaPosicaoH, ultimaPosicaoV) + 1;
    
            //Caso em que há qwirkle nas peças posicionadas
            if( auxPontosV == 6)
            {
                auxPontos = auxPontosV*2;
            }
            else
            {
                auxPontos = auxPontosV;
            }
            int k = 0;

            //Conta-se os pontos na horizontal de cada peça posicionada
            while(k < tam)
            {
                auxPontosH = contarPosicoesHorizontal(board, todasJogadas[k], todasJogadas[k+1]);
                if(auxPontosH == 5)
                {
                    auxPontos += (auxPontosH+1)*2;
                }
                else if(auxPontosH >= 1)
                {
                    auxPontos += auxPontosH+1;
                }
                k+=2;
            }
            jog->pontos += auxPontos;
        }
    }
    //Caso em que uma peça foi posicionada
    else
    {
        //Conta os pontos na horizontal e a peça posionada
        auxPontosH = contarPosicoesHorizontal(board, ultimaPosicaoH, ultimaPosicaoV)+1;
        //Conta os pontos na vertical sem contar a peça, já que ela já foi contabilizada
        auxPontosV = contarPosicoesVertical(board, ultimaPosicaoH, ultimaPosicaoV);

        //Caso em que há qwirkle na horizontal
        if(auxPontosH == 6)
        {
            auxPontos = auxPontosH*2;
        }
        else
        {
            auxPontos = auxPontosH;
        }
        //Caso em que há qwirkle na vertical
        if(auxPontosV == 5)
        {
            auxPontos += (auxPontosV+1)*2;
        }
        else if(auxPontosV >= 1)
        {
            auxPontos += auxPontosV+1;
        }
        jog->pontos += auxPontos;
    }
}

//Função que retorna a quantidade posições conectadas na linha das coordenadas passadas como parâmetro, não conta a posição nas coordenadas
int contarPosicoesHorizontal(pont board, int posHInicial, int posVInicial)
{
    int pontos = 0;

    int controlePosicao = posVInicial + 1;
    //Verifica a quantidade de posições conectadas à direita
    while(board->pieces[posHInicial][controlePosicao].formato != ' ')
    {
        controlePosicao++;
        pontos++;
    }

    controlePosicao = posVInicial - 1;
    //Verifica a quantidade de posições conectadas à esquerda
    while(board->pieces[posHInicial][controlePosicao].formato != ' ')
    {
        controlePosicao--;
        pontos++;
    }
    return pontos;
}

//Função que retorna a quantidade posições conectadas na coluna das coordenadas passadas como parâmetro, não conta a posição nas coordenadas
int contarPosicoesVertical(pont board, int posHInicial, int posVInicial)
{
    int pontos = 0;

    int controlePosicao = posHInicial + 1;
    //Verifica a quantidade de posições conectadas abaixo
    while(board->pieces[controlePosicao][posVInicial].formato != ' ')
    {
        controlePosicao++;
        pontos++;
    }
    
    controlePosicao = posHInicial - 1;
    //Verifica a quantidade de posições conectadas em cima
    while(board->pieces[controlePosicao][posVInicial].formato != ' ')
    {
        controlePosicao--;
        pontos++;
    }
    return pontos;
}

//Função que imprime o tabuleiro e coordenadas para auxiliar na inserção
void printBoard(pont board, pontJogadores inicio)
{
    char formato, cor;
    char msg[256];
    pontJogadores jogAtual = inicio;
    while(jogAtual != NULL){
        //send(jogAtual->numSocket,"LIMPAR", 6,0);
        //Impressão das coordenadas das colunas
        memset(msg, 0, 256);
        sprintf(msg,"   ");
        send(jogAtual->numSocket, msg, strlen(msg), 0);
        
        for(int k = 0; k < board->columns; k++)
        {
            memset(msg, 0, 256);
            sprintf(msg,"  %d  ", k);
            send(jogAtual->numSocket, msg, strlen(msg), 0);
        }
        memset(msg, 0, 256);
        sprintf(msg, "\n");
        send(jogAtual->numSocket,msg,strlen(msg),0);
        //Impressão das peças
        for(int i=0; i<board->rows; i++)
        {
            //Impressão do número da linha
            memset(msg, 0, 256);
            sprintf(msg,"%d  ", i);
            send(jogAtual->numSocket,msg,strlen(msg),0);
            
            for(int j=0; j<board->columns; j++)
            {
                formato = board->pieces[i][j].formato;
                cor = board->pieces[i][j].cor;
                memset(msg, 0, 256);
                sprintf(msg, "[%c%c] ", formato, cor);
                send(jogAtual->numSocket,msg,strlen(msg),0);
                
            }
            memset(msg, 0, 256);
            sprintf(msg, "\n");
            send(jogAtual->numSocket,msg,strlen(msg),0);
            
        }
        jogAtual = jogAtual->proxJog;
    }
}
