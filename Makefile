snakegame: main.o game.o snake.o music.o score_road.o block.o
	g++ -o snakegame main.o game.o snake.o music.o score_road.o block.o -L. pdcurses.lib -lwinmm
main.o: main.cpp game.h
	g++ -c main.cpp
score_road.o:score_road.cpp
	g++ -c score_road.cpp
block.o:block.cpp
	g++ -c block.cpp
music.o:music.cpp -lwinmm
	g++ -c music.cpp
game.o: game.cpp snake.h music.h score_road.h block.h
	g++ -c game.cpp 
snake.o: snake.cpp score_road.h block.h
	g++ -c snake.cpp
clean:
	del *.o 
	del snakegame.exe
	del record.dat
	del 	gamefile.txt
