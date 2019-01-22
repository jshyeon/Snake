snake_exe : snake.o main.o
	g++ -o snake_exe snake.o main.o

snake.o : snake.h snake.cpp
	g++ -c -o snake.o snake.cpp

main.o : main.cpp
	g++ -c -o main.o main.cpp

clean :
	rm *.o snake_exe
