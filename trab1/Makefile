jogadores.o : jogadores.c
	gcc -o jogadores.o -c jogadores.c
deck.o : deck.c
	gcc -o deck.o -c deck.c
jogo.o : jogo.c
	gcc -o jogo.o -c jogo.c
server.o : simple_server.c
	gcc -o server.o -c simple_server.c
cliente : simple_client.c 
	gcc -o cliente simple_client.c -pthread
main : server.o jogo.o deck.o jogadores.o
	gcc -o main server.o jogo.o deck.o jogadores.o -pthread
all : main cliente
run : main
	./main
clean :
	rm *.o main cliente