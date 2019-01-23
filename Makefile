snake_exe : main.o snake.o
	g++ -o snake_exe main.o snake.o -lallegro -lpthread -lallegro_primitives

main.o : main.cpp
	g++ -c -o main.o main.cpp 

snake.o : snake.h snake.cpp
	g++ -c -o snake.o snake.cpp

server : server.c
	gcc -o server server.c -lpthread

clean :
	rm *.o snake_exe
