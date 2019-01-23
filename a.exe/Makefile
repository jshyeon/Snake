a.exe : snake.o main.o
	g++ -pthread -Iinclude -o a.exe main.o snake.o `pkg-config --libs allegro-5 allegro_primitives-5`


main.o : main.cpp
	g++ -pthread -c -o main.o main.cpp

snake.o : snake.cpp
	g++ -c -o snake.o snake.cpp
