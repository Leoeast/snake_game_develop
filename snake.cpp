#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "snake.h"


SnakeBody::SnakeBody()
{
    mX = -1;
    mY = -1;
}


SnakeBody::SnakeBody(int x, int y): mX(x), mY(y)
{
}

int SnakeBody::getX() const
{
    return mX;
}

int SnakeBody::getY() const
{
    return mY;
}

bool SnakeBody::operator == (const SnakeBody& snakeBody)
{
    return (this->getX() == snakeBody.getX() && this->getY() == snakeBody.getY());
}

Snake::Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength): mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mInitialSnakeLength(initialSnakeLength)
{
    this->initializeSnake();
    this->setRandomSeed();
}
//input initialize snake
Snake::Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength , std::ifstream& input): mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mInitialSnakeLength(initialSnakeLength)
{

    int snake_x , snake_y;
        while(input >> snake_x && input >> snake_y){
            SnakeBody sn(snake_x , snake_y);
            this->getSnake().push_back(sn);
        }
    this->setRandomSeed();
}

int Snake::get_direction()
{
    switch(mDirection)
    {
    case Direction::Up:
        return 0;
        break;
    case Direction::Down:
        return 1;
        break;
    case Direction::Left:
        return 2;
        break;
    default:
        return 3;
        break;
    }
}

void Snake::set_direction(int& new_direc)
{
    switch(new_direc)
    {
    case 0:
        mDirection = Direction::Up;
        break;
    case 1:
        mDirection = Direction::Down;
        break;
    case 2:
        mDirection = Direction::Left;
        break;
    default:
        mDirection = Direction::Right;
        break;
    }
}

void Snake::setRandomSeed()
{
    // use current time as seed for random generator
    std::srand(std::time(nullptr));
}

void Snake::initializeSnake()
{
    // Instead of using a random initialization algorithm
    // We always put the snake at the center of the game mWindows
    int centerX = this->mGameBoardWidth / 2;
    int centerY = this->mGameBoardHeight / 2;

    for (int i = 0; i < this->mInitialSnakeLength; i ++)
    {
        this->mSnake.push_back(SnakeBody(centerX, centerY + i));
    }
    this->mDirection = Direction::Up;
}

bool Snake::isPartOfSnake(int x, int y)
{
    SnakeBody temp = SnakeBody(x, y);
    for (int i = 0; i < this->mSnake.size(); i ++)
    {
        if (this->mSnake[i] == temp)
        {
            return true;
        }
    }
    return false;
}

/*
 * Assumption:
 * Only the head would hit wall.
 */
bool Snake::hitWall()
{
    SnakeBody& head = this->mSnake[0];
    int headX = head.getX();
    int headY = head.getY();
    if (headX <= 0 || headX >= this->mGameBoardWidth - 1)
    {
        return true;
    }
    if (headY <= 0 || headY >= this->mGameBoardHeight - 1)
    {
        return true;
    }
    return false;
}

/*
 * The snake head is overlapping with its body
 */
bool Snake::hitSelf()
{
    SnakeBody& head = this->mSnake[0];
    // Exclude the snake head
    for (int i = 1; i < this->mSnake.size(); i ++)
    {
        if (this->mSnake[i] == head)
        {
            return true;
        }
    }
    return false;
}


bool Snake::touchFood()
{
    SnakeBody newHead = this->createNewHead();
    if (this->mFood == newHead)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Snake::senseFood(SnakeBody food)
{
    this->mFood = food;
}

bool Snake::touchProp()
{
    if (this->mProp == this->mSnake[0])
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Snake::senseProp(SnakeBody prop)
{
    this->mProp = prop;
}

std::vector<SnakeBody>& Snake::getSnake()
{
    return this->mSnake;
}

bool Snake::changeDirection(Direction newDirection)
{
    switch (this->mDirection)
    {
        case Direction::Up:
        {
            if (newDirection == Direction::Left || newDirection == Direction::Right)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case Direction::Down:
        {
            if (newDirection == Direction::Left || newDirection == Direction::Right)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case Direction::Left:
        {
            if (newDirection == Direction::Up || newDirection == Direction::Down)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case Direction::Right:
        {
            if (newDirection == Direction::Up || newDirection == Direction::Down)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    return false;
}


SnakeBody Snake::createNewHead()
{
    SnakeBody& head = this->mSnake[0];
    int headX = head.getX();
    int headY = head.getY();
    int headXNext;
    int headYNext;

    switch (this->mDirection)
    {
        case Direction::Up:
        {
            headXNext = headX;
            headYNext = headY - 1;
            break;
        }
        case Direction::Down:
        {
            headXNext = headX;
            headYNext = headY + 1;
            break;
        }
        case Direction::Left:
        {
            headXNext = headX - 1;
            headYNext = headY;
            break;
        }
        case Direction::Right:
        {
            headXNext = headX + 1;
            headYNext = headY;
            break;
        }
    }

    SnakeBody newHead = SnakeBody(headXNext, headYNext);

    return newHead;
}

/*
 * If eat food, return true, otherwise return false
 */
bool Snake::moveFoward()
{
    if (this->touchFood())
    {
        SnakeBody newHead = this->mFood;
        this->mSnake.insert(this->mSnake.begin(), newHead);
        return true;
    }
    else
    {
        this->mSnake.pop_back();
        SnakeBody newHead = this->createNewHead();
        this->mSnake.insert(this->mSnake.begin(), newHead);
        return false;
    }
}

bool Snake::checkCollision(Block& mblock)
{
    if (this->hitWall() || this->hitSelf() || this->hitBlock(mblock))
    {
        return true;
    }
    else
    {
        return false;
    }
}


int Snake::getLength()
{
    return this->mSnake.size();
}

void Snake::senseSroad(ScoreRoad& srd)
{
    sroad = srd;
}


bool Snake::touchRoad()
{
    return (sroad.isPartOfScore(this->mSnake[0].getX() , this->mSnake[0].getY()));
}


bool Snake::hitBlock(Block& mblock)
{
    int head_x = mSnake[0].getX();
    int head_y = mSnake[0].getY();

    if(mblock.isPartOfBlock(head_x , head_y))return true;

    return false;
}


