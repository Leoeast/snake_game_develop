snakegame: main.o game.o snake.o music.o
	g++ -o snakegame main.o game.o snake.o music.o -L. pdcurses.lib -lwinmm
main.o: main.cpp game.h
	g++ -c main.cpp
music.o:music.cpp -lwinmm
	g++ -c music.cpp
game.o: game.cpp snake.h music.h color.h
	g++ -c game.cpp 
snake.o: snake.cpp
	g++ -c snake.cpp
clean:
	del *.o 
	del snakegame.exe
	del record.dat
	del gamefile.txt
