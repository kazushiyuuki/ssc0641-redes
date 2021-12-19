main.o: main.cpp
	g++ -o main.o -c main.cpp
main: main.o
	g++ -o main main.o
all: main
run: main
	./main
clean: 
	rm *.o main