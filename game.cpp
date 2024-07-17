#include <string>
#include <iostream>
#include <cmath>

// For terminal delay
#include <chrono>
#include <thread>

#include <fstream>
#include <algorithm>

#include "game.h"
#include "music.h"
#include "color.h"
#include "score_road.h"
#include "block.h"

Game::Game()
{
    // Separate the screen to three windows
    this->mWindows.resize(3);
    initscr();
    // If there wasn't any key pressed don't wait for keypress
    nodelay(stdscr, true);
    // Turn on keypad control
    keypad(stdscr, true);
    // No echo for the key pressed
    noecho();
    // No cursor show
    curs_set(0);
    // Get screen and board parameters
    getmaxyx(stdscr, this->mScreenHeight, this->mScreenWidth);
    this->mGameBoardWidth = this->mScreenWidth - this->mInstructionWidth;
    this->mGameBoardHeight = this->mScreenHeight - this->mInformationHeight;

    this->createInformationBoard();
    this->createGameBoard();
    this->createInstructionBoard();

    // Initialize the leader board to be all zeros
    this->mLeaderBoard.assign(this->mNumLeaders, 0);
}

Game::~Game()
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        delwin(this->mWindows[i]);
    }
    endwin();
}

void Game::createInformationBoard()
{
    int startY = 0;
    int startX = 0;
    this->mWindows[0] = newwin(this->mInformationHeight, this->mScreenWidth, startY, startX);
}

void Game::renderInformationBoard() const
{
    wbkgd(mWindows[0] , COLOR_PAIR(board_0_color));//render information board color
    wattron(this->mWindows[0] , COLOR_PAIR(information_color));
    mvwprintw(this->mWindows[0], 1, 1, "Welcome to The Snake Game!");
    mvwprintw(this->mWindows[0], 2, 1, "This is by Leo.");
    mvwprintw(this->mWindows[0], 3, 1, "Github : Leoeast!");
    mvwprintw(this->mWindows[0], 4, 1, "Implemented using C++ and libncurses library.");
    wrefresh(this->mWindows[0]);
    wattroff(this->mWindows[0] , COLOR_PAIR(information_color));
}

void Game::createGameBoard()
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth - this->mInstructionWidth, startY, startX);
}

void Game::renderGameBoard() const
{
    wrefresh(this->mWindows[1]);
}

void Game::createInstructionBoard()
{
    int startY = this->mInformationHeight;
    int startX = this->mScreenWidth - this->mInstructionWidth;
    this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mInstructionWidth, startY, startX);
}

void Game::renderInstructionBoard() const
{
    wbkgd(mWindows[2] , COLOR_PAIR(board_2_color));
    mvwprintw(this->mWindows[2], 1, 1, "Manual");

    //mvwprintw(this->mWindows[2], 3, 1, "Up: W");
    //mvwprintw(this->mWindows[2], 4, 1, "Down: S");
    //mvwprintw(this->mWindows[2], 5, 1, "Left: A");
    //mvwprintw(this->mWindows[2], 6, 1, "Right: D");
    mvwprintw(this->mWindows[2], 3, 1, "Escape: ESC");

    mvwprintw(this->mWindows[2], 5, 1, "Life");

    mvwprintw(this->mWindows[2], 7, 1, "Difficulty");
    mvwprintw(this->mWindows[2], 9, 1, "Points");
    mvwprintw(this->mWindows[2], 11, 1, "Red Food");

    wrefresh(this->mWindows[2]);
}


void Game::renderLeaderBoard() const
{
    // If there is not too much space, skip rendering the leader board
    if (this->mScreenHeight - this->mInformationHeight - 16 - 2 < 3 * 2)
    {
        return;
    }
    mvwprintw(this->mWindows[2], 16, 1, "Leader Board");
    std::string pointString;
    std::string rank;
    for (int i = 0; i < std::min(this->mNumLeaders, this->mScreenHeight - this->mInformationHeight - 16 - 2); i ++)
    {
        pointString = std::to_string(this->mLeaderBoard[i]);
        rank = "#" + std::to_string(i + 1) + ":";
        mvwprintw(this->mWindows[2], 16 + (i + 1), 1, rank.c_str());
        mvwprintw(this->mWindows[2], 16 + (i + 1), 5, pointString.c_str());
    }
    wrefresh(this->mWindows[2]);
}

int Game::renderEscMenu() const
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Restart", "Quit" , "Continue" , "Save" , "Help"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Your Score:");
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 2, 1, pointString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, menuItems[1].c_str());
    mvwprintw(menu, 2 + offset, 1, menuItems[2].c_str());
    mvwprintw(menu, 3 + offset, 1, menuItems[3].c_str());
    mvwprintw(menu, 4 + offset, 1, menuItems[4].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    return index;
}


bool Game::renderRestartMenu() const
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Restart", "Quit"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Your Final Score:");
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 2, 1, pointString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, menuItems[1].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}

void Game::renderHelp()
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);

    mvwprintw(menu, 1, 1, "hahaha");
    mvwprintw(menu, 2, 1, "lalala");
    mvwprintw(menu, 3, 1, "xixixi");

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();

        if(key == 27)break;

        wrefresh(menu);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);
}
void Game::renderGameParameter() const
{
    wattron(this->mWindows[2] , COLOR_PAIR(point_color));//render point
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(this->mWindows[2], 9, 10, pointString.c_str());
    wattroff(this->mWindows[2] , COLOR_PAIR(point_color));

    wattron(this->mWindows[2] , COLOR_PAIR(difficulty_color));//render difficulty
    std::string difficultyString = std::to_string(this->mDifficulty);
    mvwprintw(this->mWindows[2], 7, 14, difficultyString.c_str());
    wattroff(this->mWindows[2] , COLOR_PAIR(difficulty_color));

    wattron(this->mWindows[2] , COLOR_PAIR(life_color));//render life
    std::string lifestring = std::to_string(this->mLife);
    mvwprintw(this->mWindows[2], 5, 8, lifestring.c_str());
    wattroff(this->mWindows[2] , COLOR_PAIR(life_color));

    wattron(this->mWindows[2] , COLOR_PAIR(rednum));//render red food
    std::string rednumstring = std::to_string(this->mRedFoodNum);
    mvwprintw(this->mWindows[2], 11, 10, rednumstring.c_str());
    wattroff(this->mWindows[2] , COLOR_PAIR(rednum));

    wrefresh(this->mWindows[2]);
}


void Game::initializeGame()
{
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
    this->mBlock = Block();
    this->createRamdonFood();
    this->createRandomScore();
    this->mPtrSnake->senseFood(this->mFood);
    this->mPtrSnake->senseSroad(this->mRoad);
    this->mDifficulty = 0;
    this->mPoints = 0;
    this->mLife = 1;
    this->mRedFoodNum = 0;
    this->mDelay = this->mBaseDelay;
}

void Game::createRamdonFood()
{
    std::vector<SnakeBody> availableGrids;
    for (int i = 2; i < this->mGameBoardHeight - 2; i ++)
    {
        for (int j = 1; j < this->mGameBoardWidth - 1; j ++)
        {
            if(this->mPtrSnake->isPartOfSnake(j, i) || this->mRoad.isPartOfScore(j , i) || this->mBlock.isPartOfBlock(j ,i))
            {
                continue;
            }
            else
            {
                availableGrids.push_back(SnakeBody(j, i));
            }
        }
    }

    // Randomly select a grid that is not occupied by the snake
    int random_idx = std::rand() % availableGrids.size();
    this->mFood = availableGrids[random_idx];
}

void Game::renderFood() const
{
    wattron(this->mWindows[1] , COLOR_PAIR(food_color));
    mvwaddch(this->mWindows[1], this->mFood.getY(), this->mFood.getX(), this->mFoodSymbol);
    wrefresh(this->mWindows[1]);
    wattroff(this->mWindows[1] , COLOR_PAIR(food_color));
}

void Game::createRandomScoreHead(int& x , int& y)
{
    do{
        x = 1 + rand()%100;
        y = 2 + rand()%20;

    }
    while((x == this->mFood.getX() && y == this->mFood.getY()) || this->mPtrSnake->isPartOfSnake(x, y) || this->mBlock.isPartOfBlock(x ,y ));
}
void Game::createRandomScore()
{
    int head_x , head_y;
    createRandomScoreHead(head_x , head_y);
    this->mRoad = ScoreRoad(head_x , head_y);
    for(int i = 1 ; i < road_length ; i ++)
    {
        int a_ord = this->mRoad.get_road()[this->mRoad.get_road().size() - 1].get_ord();
        int a_x = this->mRoad.get_road()[this->mRoad.get_road().size() - 1].get_x();
        int a_y = this->mRoad.get_road()[this->mRoad.get_road().size() - 1].get_y();
        int tep = std::rand() % 4;
        switch(tep)
        {
        case 0 :
            if(this->isEmpty(a_x  , a_y - 1))mRoad.get_road().push_back(ScoreOne(a_x , a_y - 1 , a_ord + 1 ));
            break;
        case 1 :
            if(this->isEmpty(a_x  , a_y + 1))mRoad.get_road().push_back(ScoreOne(a_x , a_y + 1 , a_ord + 1 ));
            break;
        case 2 :
            if(this->isEmpty(a_x - 1  , a_y ))mRoad.get_road().push_back(ScoreOne(a_x - 1 , a_y , a_ord + 1 ));
            break;
        default :
            if(this->isEmpty(a_x + 1 , a_y))mRoad.get_road().push_back(ScoreOne(a_x + 1 , a_y , a_ord + 1 ));
            break;

        }
    }
    mcompleteRoad = mRoad;
}

bool Game::inBound(int x , int y)
{
    if(x > 1 && x < mGameBoardWidth - 2 && y > 1 && y < mGameBoardHeight - 2)return true;
    return false;
}

bool Game::isEmpty(int x , int y)
{
    if(!inBound(x , y))return false;//check bound
    if(x == mFood.getX() && y == mFood.getY())return false;//check food
    if(this->mPtrSnake->isPartOfSnake(x, y))return false;//check snake
    if(this->mRoad.isPartOfScore(x , y))return false;//check road
    if(this->mBlock.isPartOfBlock(x , y))return false;//check block
    return true;
}

void Game::renderScore()
{
    int scoreLength = this->mRoad.get_length();
                //init_pair(1 , COLOR_CYAN , COLOR_CYAN);
                //attron(COLOR_PAIR(1));
    wattron(this->mWindows[1] , COLOR_PAIR(score_one_color));
    for (int i = 0; i < scoreLength; i ++)
    {
        char sym = this->mRoad.get_road()[i].get_ord() + '0';
        mvwaddch(this->mWindows[1], this->mRoad.get_road()[i].get_y(), this->mRoad.get_road()[i].get_x(), sym);
    }
    wrefresh(this->mWindows[1]);
    wattroff(this->mWindows[1] , COLOR_PAIR(score_one_color));
}

bool Game::Road_is_complete()
{
    return (mcompleteRoad == mRoad);
}

void Game::createBlock(int x , int y)
{
    int temp = this->mRoad.get_length();
    for(int i = 0 ; i < temp ; i ++)
    {
        if(x == this->mRoad.get_road()[i].get_x() && y == this->mRoad.get_road()[i].get_y())continue;
        this->mBlock.get_block_vec().push_back(BlockOne(this->mRoad.get_road()[i].get_x() , this->mRoad.get_road()[i].get_y()));
    }
}

void Game::createBlock()
{
    int temp = this->mRoad.get_length();
    for(int i = 0 ; i < temp ; i ++)
    {
        this->mBlock.get_block_vec().push_back(BlockOne(this->mRoad.get_road()[i].get_x() , this->mRoad.get_road()[i].get_y()));
    }
}

void Game::renderBlock()
{
    int blockLength = this->mBlock.get_length();
                //init_pair(1 , COLOR_CYAN , COLOR_CYAN);
                //attron(COLOR_PAIR(1));
    wattron(this->mWindows[1] , COLOR_PAIR(block_color));
    for (int i = 0; i < blockLength; i ++)
    {
        mvwaddch(this->mWindows[1], this->mBlock.get_block_vec()[i].get_y(), this->mBlock.get_block_vec()[i].get_x(), mBlockSymbol);
    }
    wrefresh(this->mWindows[1]);
    wattroff(this->mWindows[1] , COLOR_PAIR(block_color));
}

void Game::renderSnake() const
{
    int snakeLength = this->mPtrSnake->getLength();
    std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();

                //init_pair(1 , COLOR_CYAN , COLOR_CYAN);
                //attron(COLOR_PAIR(1));
    wattron(this->mWindows[1] , COLOR_PAIR(snake_color));
    for (int i = 0; i < snakeLength; i ++)
    {
        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);
    }
    wrefresh(this->mWindows[1]);
    wattroff(this->mWindows[1] , COLOR_PAIR(snake_color));
}

void Game::controlSnake(int& escfor) const
{
    int key;
    key = getch();
    switch(key)
    {
        case 'W':
        case 'w':
        case KEY_UP:
        {
            this->mPtrSnake->changeDirection(Direction::Up);
            break;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
            this->mPtrSnake->changeDirection(Direction::Down);
            break;
        }
        case 'A':
        case 'a':
        case KEY_LEFT:
        {
            this->mPtrSnake->changeDirection(Direction::Left);
            break;
        }
        case 'D':
        case 'd':
        case KEY_RIGHT:
        {
            this->mPtrSnake->changeDirection(Direction::Right);
            break;
        }
        case 27:
        {
            escfor = renderEscMenu();
            break;
        }
        default:
        {
            break;
        }
    }
}

void Game::renderBoards() const
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        werase(this->mWindows[i]);
    }
    this->renderInformationBoard();
    this->renderGameBoard();
    this->renderInstructionBoard();
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        box(this->mWindows[i], 0, 0);
        wrefresh(this->mWindows[i]);
    }
    this->renderLeaderBoard();
}


void Game::adjustDelay()
{
    this->mDifficulty = this->mPoints / 10;
    if (mPoints % 5 == 0)
    {
        this->mDelay = this->mBaseDelay * pow(0.85 , this->mDifficulty);
    }
}

void Game::adjustLife()
{
    if(this->mRedFoodNum == 5){
            mLife++;
            mRedFoodNum = 0;
    }
}

void Game::lifeRestart()
{
    this->mPtrSnake = nullptr;
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
    this->createRamdonFood();
    this->mPtrSnake->senseFood(this->mFood);
    this->createRandomScore();
    this->mPtrSnake->senseSroad(this->mRoad);
    this->mBlock = Block();
    this->mLife--;
}

bool Game::is_dead()
{
    return (mLife == 0)? true : false;
}

void Game::runGame(int& escfor)
{
    bool moveSuccess;
    int key;

    //open bgm

    open_bgm();


    while (true)
    {
        this->controlSnake(escfor);

        //restart or quit when gaming
        if(escfor == 0 || escfor ==1){ break;}
        //save when gaming
        else if(escfor == 3){escfor = -1 ; writeGameFile();}
        else if(escfor == 4){escfor = -1 ; renderHelp();}


        werase(this->mWindows[1]);
        box(this->mWindows[1], 0, 0);
        wbkgd(this->mWindows[1] , COLOR_PAIR(board_1_color));//render game board background color

        this->renderFood();
        this->renderScore();
        this->renderBlock();

        bool eatFood = this->mPtrSnake->moveFoward();
        bool collision = this->mPtrSnake->checkCollision(this->mBlock);
        bool eatScore = this->mPtrSnake->touchRoad();

        if(collision){this->lifeRestart();
            //check dead or not
            if (this->is_dead())break;
        else continue;
            }


        this->renderSnake();
        if (eatFood)
        {
            eat_sound();//sound when eating
            this->mPoints ++;
            this->createRamdonFood();
            this->mPtrSnake->senseFood(this->mFood);
            this->mRedFoodNum ++;
            this->adjustDelay();
            this->adjustLife();
        }
        //judge if hit score
        if(eatScore)
        {
            score_sound();
            this->mPoints ++;
            this->adjustDelay();
            if(!this->mRoad.is_head(mPtrSnake->getSnake()[0].getX() , mPtrSnake->getSnake()[0].getY())){
                    this->createBlock(mPtrSnake->getSnake()[0].getX() , mPtrSnake->getSnake()[0].getY());
                    this->createRandomScore();
            }
            else {
                if(this->mRoad.get_length() == 1)
                {
                    this->createRandomScore();
                    this->mPoints ++;
                }
                else{
                    this->mRoad.get_road().erase(this->mRoad.get_road().begin());
                }
            }
            this->mPtrSnake->senseSroad(mRoad);
        }
        //check road if complete
        else{
            if(!Road_is_complete()){createBlock();createRandomScore();this->mPtrSnake->senseSroad(mRoad);}
        }

        this->renderGameParameter();

        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));

        refresh();
    }

    stop_bgm();//stop bgm
    restart_sound();//sound when menu
}

void Game::startGame()
{
    //initialise the color pair
    initscr();
    start_color();

    init_game_color();

    refresh();
    bool choice;
    while (true)
    {
        int escForwhat = -1;


        this->readLeaderBoard();
        this->renderBoards();

        //judge how to initialize game
        if(this->GameFile_is_empty())this->initializeGame();
        else{
            if(this->whether_to_read_file())this->readGameFile();
            else this->initializeGame();
        }

        this->runGame(escForwhat);

        //quit when gaming
        if(escForwhat == 1)break;
        //restart when gaming
        else if(escForwhat == 0){
                this->mPtrSnake = nullptr;
                continue;}

        this->updateLeaderBoard();
        this->writeLeaderBoard();
        choice = this->renderRestartMenu();
        if (choice == false)
        {
            break;
        }
    }
}

// https://en.cppreference.com/w/cpp/io/basic_fstream
bool Game::readLeaderBoard()
{
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.in);
    if (!fhand.is_open())
    {
        return false;
    }
    int temp;
    int i = 0;
    while ((!fhand.eof()) && (i < mNumLeaders))
    {
        fhand.read(reinterpret_cast<char*>(&temp), sizeof(temp));
        this->mLeaderBoard[i] = temp;
        i ++;
    }
    fhand.close();
    return true;
}

bool Game::updateLeaderBoard()
{
    bool updated = false;
    int newScore = this->mPoints;
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        if (this->mLeaderBoard[i] >= this->mPoints)
        {
            continue;
        }
        int oldScore = this->mLeaderBoard[i];
        this->mLeaderBoard[i] = newScore;
        newScore = oldScore;
        updated = true;
    }
    return updated;
}

bool Game::writeLeaderBoard()
{
    // trunc: clear the data file
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.trunc | fhand.out);
    if (!fhand.is_open())
    {
        return false;
    }
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        fhand.write(reinterpret_cast<char*>(&this->mLeaderBoard[i]), sizeof(this->mLeaderBoard[i]));;
    }
    fhand.close();
    return true;
}

//write game process
bool Game::writeGameFile()
{

    //clear and open the file
    std::ofstream outputFile(this->mSavedFilePath , outputFile.trunc);


    //write
    if(outputFile.is_open()){
        outputFile << this->mDelay << std::endl;//write speed
        outputFile << this->mDifficulty << std::endl;//write difficulty
        outputFile << this->mPoints << std::endl; //write points
        outputFile << this->mLife << std::endl; //write life
        outputFile << this->mRedFoodNum << std::endl; //write red num
        outputFile << this->mFood.getX() << ' ' << this->mFood.getY() << std::endl; //write food position
        outputFile << this->mRoad.get_length() << std::endl; //write mroad length
        //write mroad one by one
        for(int i = 0 ; i < this->mRoad.get_length() ; i ++)
        {
            outputFile << this->mRoad.get_road()[i].get_x() << ' ' << this->mRoad.get_road()[i].get_y()  << ' ' << this->mRoad.get_road()[i].get_ord() << std::endl;

        }
        outputFile << this->mcompleteRoad.get_length() << std::endl; //write mcomplete road  length
        //write mcomplete road one by one
        for(int i = 0 ; i < this->mcompleteRoad.get_length() ; i ++)
        {
            outputFile << this->mcompleteRoad.get_road()[i].get_x() << ' ' << this->mcompleteRoad.get_road()[i].get_y()  << std::endl;

        }
        outputFile << this->mBlock.get_length() << std::endl; //write block length
        //write block one by one
        for(int i = 0 ; i < this->mBlock.get_length() ; i ++)
        {
            outputFile << this->mBlock.get_block_vec()[i].get_x() << ' ' << this->mBlock.get_block_vec()[i].get_y()   << std::endl;

        }

        outputFile << this->mPtrSnake->get_direction() << std::endl;//write direction

        //write snake position one by one
        for(int i = 0 ; i < this->mPtrSnake->getLength() ; i ++)
        {
            outputFile << this->mPtrSnake->getSnake()[i].getX() << ' ' << this->mPtrSnake->getSnake()[i].getY()  << std::endl;

        }

    }
    else return false;

    outputFile.close();
    return true;

}

//read game process
bool Game::readGameFile()
{
    std::ifstream inputFile(this->mSavedFilePath);


    //read
    int temp;
    if(inputFile.is_open()){
        inputFile >> temp;
        this->mDelay = temp;//read speed
        inputFile >> temp;
        this->mDifficulty = temp;//read difficulty
        inputFile >> temp;
        this->mPoints = temp; //read points
        inputFile >> temp;
        this->mLife = temp; //read life
        inputFile >> temp;
        this->mRedFoodNum = temp; //read red num

        int food_x , food_y;
        inputFile >> food_x >> food_y ; //read food position
        SnakeBody food(food_x , food_y);
        this->mFood = food;//initialise the food

        inputFile >> temp;//read mroad length
        this->mRoad = ScoreRoad(); //initialize mroad
        int score_one_x , score_one_y , score_one_ord;
        for(int i = 0 ; i < temp ; i ++)
        {
            inputFile >> score_one_x >> score_one_y >> score_one_ord;
            this->mRoad.get_road().push_back(ScoreOne(score_one_x , score_one_y , score_one_ord));

        }

        inputFile >> temp;//read mcompleteRoad length
        this->mcompleteRoad = ScoreRoad(); //initialize mcomplete road
        for(int i = 0 ; i < temp ; i ++)
        {
            inputFile >> score_one_x >> score_one_y;
            this->mcompleteRoad.get_road().push_back(ScoreOne(score_one_x , score_one_y , i));

        }

        inputFile >> temp;//read block length
        this->mBlock = Block(); //initialize mblock
        int block_one_x , block_one_y ;
        for(int i = 0 ; i < temp ; i ++)
        {
            inputFile >> block_one_x >> block_one_y;
            this->mBlock.get_block_vec().push_back(BlockOne(block_one_x , block_one_y));

        }

        inputFile >> temp;

        //read snake position one by one
        this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength , inputFile));
        //sense food
        this->mPtrSnake->senseFood(mFood);
        //set direction
        this->mPtrSnake->set_direction(temp);
        //sense mscore
        this->mPtrSnake->senseSroad(this->mRoad);

        }


    else return false;

    inputFile.close();
    return true;
}

bool Game::GameFile_is_empty()
{
    std::ifstream inputFile(this->mSavedFilePath);

    bool result = false;
    if(inputFile.get() == EOF)result = true;


    inputFile.close();

    return result;

}

bool Game::whether_to_read_file()
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Load", "New Game"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "whether to load or not");

    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, menuItems[1].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}








