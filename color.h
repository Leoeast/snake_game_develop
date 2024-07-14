#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include "curses.h"

#define food_color 1
#define snake_color 2
#define board_1_color 3
#define point_color 4
#define board_0_color 5
#define difficulty_color 6
#define information_color 7
#define board_2_color 8
#define life_color 9
#define rednum 10
#define score_one_color 11
#define block_color 12

//init_color(COLOR_BLUE , 19 , 70 , 149);
int init_game_color(){
    start_color();
    init_pair(food_color , COLOR_RED , COLOR_RED);//food
    init_pair(snake_color , COLOR_GREEN , COLOR_BLACK);//snake
    init_pair(board_1_color , COLOR_WHITE , COLOR_BLUE); //board 1 background
    init_pair(point_color , COLOR_WHITE , COLOR_YELLOW); //points
    init_pair(board_0_color , COLOR_WHITE , COLOR_YELLOW);//board 0 background
    init_pair(difficulty_color , COLOR_RED , COLOR_YELLOW);//difficulty
    init_pair(information_color , COLOR_BLUE , COLOR_YELLOW);//information
    init_pair(board_2_color , COLOR_WHITE , COLOR_RED);//board 2 background
    init_pair(life_color , COLOR_RED , COLOR_BLACK);//life
    init_pair(rednum , COLOR_YELLOW , COLOR_BLUE);//red food
    init_pair(score_one_color , COLOR_GREEN , COLOR_WHITE);//red food
    init_pair(block_color , COLOR_BLACK , COLOR_BLUE);//block
    return 0;
}


#endif // COLOR_H_INCLUDED
