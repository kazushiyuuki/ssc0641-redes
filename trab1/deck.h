#ifndef JOGO_H
#define JOGO_H
#include "jogo.h"
#endif


//Estrutura carta que contem todas as informações de uma peça e possui um ponteiro para a próxima carta(essencial para a estrutura deque)
typedef struct aux
{
    piece info;
    struct aux * prox;
}carta, * pontCarta;

//Estrutura deque contém os ponteiros para a última e primeira carta, o que facilita as trocas e distribuições das mesmas
typedef struct
{
    pontCarta inicio;
    pontCarta fim;
    int qtdCartas;
}deque;

typedef deque * pontDeque;

pontDeque iniciarDeque();

pontDeque gerarMonte();

void resetarDeque(pontDeque deque);