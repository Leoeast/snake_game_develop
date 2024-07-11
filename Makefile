snakegame: main.o game.o snake.o
	g++ -o snakegame main.o game.o snake.o -L. pdcurses.lib
main.o: main.cpp game.cpp
	g++ -c main.cpp
game.o: game.cpp snake.cpp
	g++ -c game.cpp
snake.o: snake.cpp
	g++ -c snake.cpp
clean:
	del *.o 
	del snakegame.exe
	del record.dat
