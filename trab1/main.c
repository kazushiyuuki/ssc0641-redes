/*#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"jogadores.h"
#include<ctype.h>

int main()
{
    //Inteiro que controla o jogar novamente
    int controleJogar = 1;

    while(controleJogar == 1)
    {
        //Inicia-se o tabuleiro e o monte
        pont board = startBoard();
        pontDeque monte = iniciarDeque();

        int qtdJogs;
        char aux[2];

        //Leitura da quantidade de jogadores
        printf("Digite o numero de jogadores: ");
        fgetss(aux, 1, "Numero de jogadores");
        while(isdigit(aux[0]) == 0)
        {
            printf("Numero invalido! Digite novamente o número de jogadores: ");
            fgetss(aux, 1, "Numero de jogadores");
        }
        qtdJogs = atoi(aux);
        
        //Aqui são iniciados os jogadores
        pontJogadores inicio =  iniciarJogs(monte, qtdJogs);

        pontJogadores jogAtual;
        jogAtual = inicio;
        int i = 1;

        //Percorre-se todos os jogadores para armazenar os nomes de cada um deles
        while(jogAtual != NULL)
        {
            char nome[21];
            printf("Digite o nome do jogador %d: ", i);
            fgetss(nome, 20, "Nome");
            strcpy(jogAtual->nome, nome);
            jogAtual = jogAtual->proxJog;
            i++;
        }

        //Verificando se o usuário quer jogar em cheat mode
        printf("Cheat mode (S/N): ");
        char cheat;
        scanf(" %c", &cheat);
        if(cheat == 's' || cheat == 'n'){
            cheat = cheat - 32;
        }
        while(cheat != 'S' && cheat != 'N'){
            printf("Cheat mode (S/N): ");
            scanf(" %c", &cheat);
            if(cheat == 's' || cheat == 'n'){
                cheat = cheat - 32;
            }
        }
        getchar();
        
        //Estrutura de repetição enquanto houver cartas no monte
        while(monte->qtdCartas != 0)
        {
            //Aqui percorre-se cada um dos jogadores
            jogAtual = inicio;
            while(jogAtual != NULL)
            {
                //Leitura de comandos
                leituraComandos(board, monte, jogAtual, cheat);
                jogAtual = jogAtual->proxJog;
            }
        }

        //Função para selecionar o vencedor
        selecionarVencedor(inicio);
        
        //Verificas se o usuário deseja jogar novamente
        char jogarNov[2];
        printf("Deseja jogar novamente (S/N): ");
        fgetss(jogarNov, 1, "Comando para jogar novamente");
        toUpper(jogarNov);
        if(jogarNov[0] == 'S')
        {
            controleJogar = 1;
        }
        else
        {
            controleJogar = 0; 
        }
        
    }
}*/