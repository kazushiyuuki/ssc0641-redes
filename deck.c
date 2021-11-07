#include"deck.h"
#include<malloc.h>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>


/*Função que inicia o deque, alocando nessa estrutura todas as 108 peças possíveis*/
pontDeque iniciarDeque()
{
    //Alocação do estrutura deque
    pontDeque aux = (pontDeque)malloc(sizeof(deque));
    aux->fim = NULL;
    aux->inicio = NULL;
    aux->qtdCartas = 0;

    /*Função gerarMonte gera um deque que possui as informaçõe das 108 peças possíveis. No entanto, ele em ordem Q1, Q2, ..., Q6, C1, C2...*/
    pontDeque monte = gerarMonte();

    //Gera sequências aleatórias para cada execução
    srand(time(0));
    int passos;

    //Variáveis que auxiliam na organização dos ponteiros para que não haja perdas de cartas por causa de perdas de ponteiros
    pontCarta cartaMonte, cartaAnterior;

    //Nesse while seleciona-se uma carta aleatória do monte e a coloca no deque
    while(monte->qtdCartas != 0) 
    {
        //Gera-se um número aleatório que representa quantas cartas serão passadas até que a carta sorteada seja selecionada
        if(monte->qtdCartas != 1)
        {
            passos = rand()%(monte->qtdCartas - 1);
        }
        else
        {
            passos = 0;
        }
        cartaAnterior = NULL;
        cartaMonte = monte->inicio;

        //Aqui as variáveis recebem os ponteiros para que quando a carta selecionada seja retirada, a carta anterior aponte para a carta apontada pela carta selecionada
        while(passos != 0)
        {
            cartaAnterior = cartaMonte;
            cartaMonte = cartaMonte->prox;
            passos--;
        }
        //Caso em que a carta selecionada é a primeira
        if(cartaAnterior == NULL)
        {
            monte->inicio = monte->inicio->prox;
            monte->qtdCartas--;
        }
        else
        {
            cartaAnterior->prox = cartaMonte->prox;
            monte->qtdCartas--;
        }
        //Caso em que o deque está vazio
        if(aux->inicio == NULL)
        {
            aux->inicio = cartaMonte;
            aux->fim = cartaMonte;
            aux->fim->prox = NULL;
            aux->qtdCartas++;
        }
        else
        {
            pontCarta temp = aux->inicio;
            aux->inicio = cartaMonte;
            aux->inicio->prox = temp;
            aux->qtdCartas++;
        }
        
    }
    return aux;
}

//Essa função gera um deque com todas as cartas possíveis. A ordem gerada é ade de todas as cores possíveis por formato e se repete 3 vezes
pontDeque gerarMonte()
{
    // Q - Quadrado
    // C - Circulo
    // L - Losango
    // E - Estrela 4 pontas
    // F - Flor
    // S - Superestrela...
    //Vetores utilizados para preencher as cartas do monte
    char formato[] = {'Q','C','L', 'E', 'F', 'S'};
    char cor[] = {'1', '2', '3', '4', '5', '6'};

    pontDeque monte = (pontDeque)malloc(sizeof(deque));
    pontCarta cartaAtual, cartaAnterior = NULL;

    //Este for representa as repetições de cada carta
    for(int m = 0; m < 3; m++)
    {
        //Este for representa todos os formatos
        for(int j = 0; j < 6; j++)
        {
            //Este for representa todas as cores
            for(int k = 0; k < 6; k++)
            {
                cartaAtual = (pontCarta)malloc(sizeof(carta));
                if(cartaAnterior != NULL)
                {
                    cartaAnterior->prox = cartaAtual;
                }
                else
                {
                    monte->inicio = cartaAtual;
                }
                cartaAtual->info.formato = formato[j];
                cartaAtual->info.cor = cor[k];
                cartaAnterior = cartaAtual;
                monte->qtdCartas++;
                monte->fim = cartaAtual;
            }
        }
    }
    return monte;
}

//Essa função desaloca a memória alocada para um deque
void resetarDeque(pontDeque deque)
{
    pontCarta aux = deque->inicio;
    pontCarta apagar;
    //Aqui desaloca-se a memória reservada para cada carta
    while(aux != NULL && deque->qtdCartas != 0)
    {
        apagar = aux;
        if(aux->prox != NULL)
        {
            aux = aux->prox;
        }
        free(apagar);
        deque->qtdCartas--;
    }
    free(deque);
}