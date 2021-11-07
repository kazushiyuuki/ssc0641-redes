#include"jogo.h"
#include<stdio.h>
#include<malloc.h>

//Função para iniciar o tabuleiro, com uma única posição vazia
pont startBoard()
{
    pont board = (pont)malloc(sizeof(board));
    board->rows = 1;
    board->columns = 1;
    board->pieces = (piece**)malloc(sizeof(piece*));
    board->pieces[0] = (piece*)malloc(sizeof(piece));
    board->pieces[0][0].cor = ' ';
    board->pieces[0][0].formato = ' ';
    return board;
}

//Função que realoca o tabuleiro, aumentando as linhas ou colunas caso a última peça colocada esteja em uma das extremidades
//Além disso, atualiza-se as posições do vetor todasJogadas para corresponder às novas coordenadas do tabuleiro
void reallocBoard(pont board, int linha, int coluna, int tam, int *todasJogadas)
{
    int i, j;
    int linhasIniciais = board->rows;
    int colunasIniciais = board->columns;

    //Caso em que última peça foi adicionada na primeira linha
    if(linha == 0)
    {
        //Aumenta-se a quantidade de linhas do tabuleiro e aloca-se mais memória para essa nova linha
        board->rows++;
        board->pieces = realloc(board->pieces, (board->rows)*sizeof(piece*));
        board->pieces[board->rows-1] = (piece*)malloc(board->columns*sizeof(piece));

        //Como a memória é alocada no final, as linhas são reposicionadas para baixo 
        for(i = board->rows-1; i > 0; i--)
        {
            piece *aux = board->pieces[i];
            board->pieces[i] = board->pieces[i-1];
            board->pieces[i-1] = aux;
        }

        //Aqui as novas posições alocadas são preenchidas com espaços
        for(j = 0; j < board->columns; j++)
        {
            board->pieces[i][j].cor = ' ';
            board->pieces[i][j].formato = ' ';
        }
        int k = 0;

        //Aqui são atualizadas as coordenadas das linhas no vetor que contém todas as jogadas
        while(k < tam)
        {
            todasJogadas[k]++;
            k+=2;
        }
    }

    //Caso em que última peça foi posicionada na última linha
    if(linha == linhasIniciais - 1)
    {
        //Incrementa-se quantidade de linhas e aloca-se mais um linha
        board->rows++;
        board->pieces = realloc(board->pieces, (board->rows)*sizeof(piece*));
        board->pieces[board->rows-1] = (piece*)malloc(board->columns*sizeof(piece));

        //Como esse novo espaço é alocado no final, basta preencher a nova linha com espaços
        for(j = 0; j < board->columns; j++)
        {
            board->pieces[board->rows-1][j].cor = ' ';
            board->pieces[board->rows-1][j].formato = ' ';
        }
    }

    //Caso em que última peça foi inserida na primeira coluna
    if(coluna == 0)
    {

        //Incremento na quantidade de colunas
        board->columns++;

        //Realocação de memória que a quantidade de posições em cada vetor de peças, há também o reposicionamento das peças para a direita e preenchimento das novas posiçoes com espaços
        for(i = 0; i < board->rows; i++)
        {
            board->pieces[i] = realloc(board->pieces[i], (board->columns)*sizeof(piece));
            for(j = board->columns-1; j > 0; j--)
            {
                piece aux = board->pieces[i][j];
                board->pieces[i][j] = board->pieces[i][j-1];
                board->pieces[i][j-1] = aux;
            }
            board->pieces[i][j].cor = ' ';
            board->pieces[i][j].formato = ' ';
        }
        int w = 1;

        //Incrementos para corrigir as coordenadas contidas no vetor de todas as jogadas
        while(w < tam)
        {
            todasJogadas[w]++;
            w+=2;
        }
    }
    //Caso em que a última peça é colocada na última posição
    if(coluna == colunasIniciais-1)
    {
        //Apenas aloca-se mais uma posição em cada vetor e a preenche com espaços
        board->columns++;
        for(i = 0; i < board->rows; i++)
        {
            board->pieces[i] = realloc(board->pieces[i], (board->columns)*sizeof(piece));
            board->pieces[i][board->columns-1].cor = ' ';
            board->pieces[i][board->columns-1].formato = ' ';
        }
    }
}

//Função para liberar a memória dedicada para o tabuleiro
void resetBoard(pont board)
{
    for(int i=0; i < board->rows; i++)
    {
        free(board->pieces[i]);
    }
    free(board->pieces);
    free(board);
}

//Função que imprime o tabuleiro e coordenadas para auxiliar na inserção
/*void printBoard(pont board)
{
    char formato, cor;

    //Impressão das coordenadas das colunas
    printf("  ");

    for(int k = 0; k < board->columns; k++)
    {
        printf(" %d   ", k);
    }
    printf("\n");
    //Impressão das peças
    for(int i=0; i<board->rows; i++)
    {
        //Impressão do núemro da linha
        printf("%d ", i);
        for(int j=0; j<board->columns; j++)
        {
            formato = board->pieces[i][j].formato;
            cor = board->pieces[i][j].cor;
            printf("[%c%c] ", formato, cor);
        }
        printf("\n");
    }
}
*/