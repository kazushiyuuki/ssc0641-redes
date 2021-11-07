#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

int meu_socket;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  
struct sockaddr_in addr1;
void *sendmessage();
void str_overwrite_stdout();
void *listener();
int done=1; 

int main(){
    int jogador, porta;
    printf("Digite qual o seu número de jogador: \n");
    scanf("%d",&jogador);
    if(jogador == 1) porta = 21000;
    else porta = 21001;

    meu_socket = socket(AF_INET,SOCK_STREAM,0);
    if(meu_socket == -1){
        printf("Erro ao criar o socket!\n");
        return 1;
    }

    addr1.sin_family      = AF_INET;
    addr1.sin_port        = htons(porta);
    addr1.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(&addr1.sin_zero,0,sizeof(addr1.sin_zero));

    printf("Tentando se conectar ao servidor...\n");

    if(connect(meu_socket,(struct sockaddr*)&addr1,sizeof(addr1)) == -1){
        printf("Erro ao se conectar!\n");
        return 1;
    }

    printf("Conectado!\n\n");

    /*
    char controle[24];
    int recebeu;
    recebeu = recv(meu_socket,controle,24,0);  
    controle[recebeu] = '\0';
    while (strcmp(controle,"OK") != 0){
        printf("PRESO\n");
    }*/
    /*
    getyx(saida, line, col);
    mvwprintw(saida, line, col+1, "INICIO DO JOGO\n\n\n");
    wrefresh(saida);*/

    pthread_t threads[2];
    void *status;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&threads[0], &attr, listener, NULL);
    pthread_create(&threads[1], &attr, sendmessage, NULL);

    while(done){}
    return 0;
}

void *listener(){
    int recebidos, line, col;
    char resposta[256];
    do{
        //pthread_mutex_lock(&mutex);
        
        recebidos = recv(meu_socket,resposta,256,0);
        resposta[recebidos] = '\0';
        printf("%s",resposta);
        str_overwrite_stdout();
        //pthread_mutex_unlock(&mutex);
    }while(recebidos != -1); 
}



void *sendmessage(){
    int  enviados;
    char mensagem[256];

    do{  
        str_overwrite_stdout();
        fgets(mensagem, 256, stdin);
        mensagem[strlen(mensagem)-1] = '\0';
        enviados = send(meu_socket,mensagem,strlen(mensagem),0);
        //pthread_mutex_unlock(&mutex);

    }while(strcmp(mensagem,"exit")!=0);

    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
    close(meu_socket);         
    done=0;
}

void str_overwrite_stdout() {
    //printf(" ");
    fflush(stdout);
}