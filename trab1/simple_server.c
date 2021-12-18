#ifndef JOGADORES_H
#define JOGADORES_H
#include "jogadores.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

void *nomeiaJogador(void *jogador);
int sock_cliente1, sock_cliente2;
struct sockaddr_in addr1, addr2;
pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;
//void *sendmessage(int sock_cliente, char *msg);
void *listener();
int done = 1;

int main(int argc, char *argv[])
{

    int socket1, socket2;
    socket1 = socket(AF_INET, SOCK_STREAM, 0);
    socket2 = socket(AF_INET, SOCK_STREAM, 0);

    /*
        socket(dominio,tipo,protocolo)
        AF_INET -> Internet
        SOCK_STREAM -> Usado por tcp
    */
    if (socket1 == -1 || socket2 == -1)
    {
        printf("Erro ao criar socket\n\n");
        return 1;
    }
    /*
        Configurando a struct addr
        struct sockaddr_in {
            short            sin_family;   // e.g. AF_INET
            unsigned short   sin_port;     // e.g. htons(3490)
            struct in_addr   sin_addr;     // see struct in_addr, below
            char             sin_zero[8];  // zero this if you want to
        };

        struct in_addr {
            unsigned long s_addr;  // load with inet_aton()
        };
    */
    addr1.sin_family = AF_INET;
    addr1.sin_port = htons(21000); //host to network short
    addr1.sin_addr.s_addr = INADDR_ANY;
    memset(&addr1.sin_zero, 0, sizeof(addr1.sin_zero)); //setando todo sin_zero como zero

    addr2.sin_family = AF_INET;
    addr2.sin_port = htons(21001);
    addr2.sin_addr.s_addr = INADDR_ANY;
    memset(&addr2.sin_zero, 0, sizeof(addr2.sin_zero));

    if (bind(socket1, (struct sockaddr *)&addr1, sizeof(addr1)) == -1 || bind(socket2, (struct sockaddr *)&addr2, sizeof(addr2)) == -1)
    {
        printf("Erro na função bind");
        return 1;
    }
    /*
        A função bind()

        int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

        Quando um socket é criado existe um espaço para o nome mas 
        n tem nenhum endereço atribuido a ele. A função bind atribui
        o endereço especificado por addr pelo socket referenciado por
        sockfd
    */

    if (listen(socket1, 1) == -1 || listen(socket2, 1) == -1)
    {
        printf("Erro na função listen");
        return 1;
    }
    /*
        A função listen()

        int listen(int sockfd, int backlog);

        A função listen() marca o socket marcado por sockfd como
        um socket passivo, isso é, um socket que vai aceitar requests de
        conexões usando a função accept
    */

    /*
        Após inicializar o socket e defini-lo como passivo
        esperamos um cliente
    
    */
    printf("Aguardando cliente 1 \n");
    sock_cliente1 = accept(socket1, 0, 0);
    printf("Aguardando cliente 2 \n");
    sock_cliente2 = accept(socket2, 0, 0);

    if (sock_cliente1 == -1 || sock_cliente2 == -1)
    {
        printf("Erro na função accept\n");
        return 1;
    }

    int sockets[2];
    sockets[0] = sock_cliente1;
    sockets[1] = sock_cliente2;

    //printf("Jogadores conectados!");

    /*
    char teste[2] = "OK";
    send(sock_cliente1,teste,strlen(teste),0);
    send(sock_cliente2,teste,strlen(teste),0);
    printf("OKs enviados \n");
*/

    //Inteiro que controla o jogar novamente
    int controleJogar = 1;

    while (controleJogar == 1)
    {
        //Inicia-se o tabuleiro e o monte
        pont board = startBoard();
        pontDeque monte = iniciarDeque();

        int qtdJogs = 2;
        char aux[2];

        /*//Leitura da quantidade de jogadores
        printf("Digite o numero de jogadores: ");
        fgetss(aux, 1, "Numero de jogadores");
        while(isdigit(aux[0]) == 0)
        {
            printf("Numero invalido! Digite novamente o número de jogadores: ");
            fgetss(aux, 1, "Numero de jogadores");
        }
        qtdJogs = atoi(aux);*/

        //Aqui são iniciados os jogadores
        pontJogadores inicio = iniciarJogs(monte, qtdJogs, sockets);

        pontJogadores jogAtual;
        jogAtual = inicio;
        int i = 1;

        /*//Percorre-se todos os jogadores para armazenar os nomes de cada um deles
        while(jogAtual != NULL)
        {
            char nome[21];
            char *msg = "Digite o seu nome:";
            send(jogAtual->numSocket,msg,strlen(msg),0);
            //send(jogAtual->numSocket,"UNLOCK",7,0);
            recv(jogAtual->numSocket,nome,21,0);
            nome[20] = '\0';
            strcpy(jogAtual->nome, nome);
            jogAtual = jogAtual->proxJog;
            i++;
        }*/

        pthread_t threads[2];
        void *status;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_create(&threads[0], &attr, nomeiaJogador, jogAtual);
        pthread_create(&threads[1], &attr, nomeiaJogador, jogAtual->proxJog);
        int t1 = pthread_join(threads[0], NULL);
        int t2 = pthread_join(threads[1], NULL);

        send(sock_cliente1, "Iniciando o jogo\n", 18, 0);
        send(sock_cliente2, "Iniciando o jogo\n", 18, 0);

        char cheat = 'N';
        /*
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
        getchar();*/

        //Estrutura de repetição enquanto houver cartas no monte
        while (monte->qtdCartas != 0)
        {
            //Aqui percorre-se cada um dos jogadores
            jogAtual = inicio;
            while (jogAtual != NULL)
            {
                char msg[256];
                sprintf(msg, "Vez do jogador %s!\n", jogAtual->nome);
                if (jogAtual->proxJog == NULL)
                {
                    send(inicio->numSocket, msg, strlen(msg), 0);
                }
                else{
                    send(jogAtual->proxJog->numSocket, msg, strlen(msg), 0);
                }
                //Leitura de comandos
                leituraComandos(board, monte, jogAtual, inicio, cheat);
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
        if (jogarNov[0] == 'S')
        {
            controleJogar = 1;
        }
        else
        {
            controleJogar = 0;
        }
    }

    //-------------------------------------------------------------------------//
    /*    pthread_t threads[2]; //criando as threads
    void *status;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    pthread_create(&threads[0], &attr, sendmessage, NULL);
    pthread_create(&threads[1], &attr, listener, NULL);

    while(done){}*/
    //-------------------------------------------------------------------------//
    return 0;
}

/*void *sendmessage(int sock_cliente, char * msg){
    int enviados;
    do{  
        //printf("Server: ");
        //fgets(mensagem,256,stdin);
        //mensagem[strlen(mensagem)-1] = '\0';
        enviados = send(sock_cliente,msg,strlen(mensagem),0);
    }while(strcmp(mensagem,"exit")!=0);

}*/

void *listener()
{
    int recebidos;
    char resposta[256];
    do
    {
        recebidos = recv(sock_cliente1, resposta, 256, 0); /* Recebe mensagem do cliente */
        resposta[recebidos] = '\0';
        printf("\n Cliente: %s\n", resposta);
    } while (strcmp(resposta, "exit") != 0);
    pthread_mutex_destroy(&mutexsum);
    pthread_exit(NULL);
    done = 0;
}

void *nomeiaJogador(void *args)
{
    pontJogadores jog = (pontJogadores) args;
    char nome[21];
    char *msg = "Digite o seu nome:";
    send(jog->numSocket, msg, strlen(msg), 0);
    recv(jog->numSocket, nome, 21, 0);
    nome[20] = '\0';
    strcpy(jog->nome, nome);
    pthread_exit(NULL);
}