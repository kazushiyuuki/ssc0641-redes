
//Definição da estrutura que representa a peça
typedef struct
{
    char formato;
    char cor;
}piece;

//Definição do tabuleiro, com uma "matriz" de peças, número de colunas e de linhas
typedef struct 
{
    piece ** pieces;
    int rows;
    int columns;
}board;

typedef board * pont;

pont startBoard();

void reallocBoard(pont board, int linha, int coluna, int tam, int *todasJogadas);

void resetBoard(pont board);

//void printBoard(pont board);