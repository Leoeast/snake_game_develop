#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <fstream>
#include "score_road.h"
#include "block.h"

enum class Direction
{
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3,
};

class SnakeBody
{
public:
    SnakeBody();
    SnakeBody(int x, int y);
    int getX() const;
    int getY() const;
    bool operator == (const SnakeBody& snakeBody);
private:
    int mX;
    int mY;
};

// Snake class should have no depency on the GUI library
class Snake
{
public:
    //Snake();
    Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength);
    Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength ,std::ifstream& input);
    // Set random seed
    void setRandomSeed();
    // Initialize snake
    void initializeSnake();
    // Check if the snake is on the coordinate
    // bool isSnakeOn(int x, int y);
    // Checking API for generating random food
    bool isPartOfSnake(int x, int y);
    //prop
    void senseProp(SnakeBody prop);
    bool touchProp();
    //usual food
    void senseFood(SnakeBody food);
    bool touchFood();

    bool hitWall();
    bool hitSelf();
    //check if hit block
    bool hitBlock(Block& mblock);
    bool checkCollision(Block& mblock);

    bool changeDirection(Direction newDirection);
    std::vector<SnakeBody>& getSnake();
    int getLength();
    SnakeBody createNewHead();
    bool moveFoward();
    //read and write
    int get_direction();
    void set_direction(int& new_direc);
    //score road
    void senseSroad(ScoreRoad& srd);
    bool touchRoad();

private:
    const int mGameBoardWidth;
    const int mGameBoardHeight;
    // Snake information
    const int mInitialSnakeLength;
    Direction mDirection;
    SnakeBody mFood;
    SnakeBody mProp;//prop information
    std::vector<SnakeBody> mSnake;
    ScoreRoad sroad;
};

#endif
