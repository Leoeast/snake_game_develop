#ifndef GAME_H
#define GAME_H

//#include <ncurses.h>
#include "curses.h"
#include <string>
#include <vector>
#include <memory>

#include "snake.h"
#include "music.h"
#include "score_road.h"
#include "block.h"


class Game
{
public:
    Game();
    ~Game();

		void createInformationBoard();
    void renderInformationBoard() const;

    void createGameBoard();
    void renderGameBoard() const;

		void createInstructionBoard();
    void renderInstructionBoard() const;

		void loadLeadBoard();
    void updateLeadBoard();
    bool readLeaderBoard();
    bool updateLeaderBoard();
    bool writeLeaderBoard();
    void renderLeaderBoard() const;

		void renderBoards() const;

		void initializeGame();
    void runGame(int & escFor);
    void lifeRestart();

    void renderGameParameter() const;

        //food
		void createRamdonFood();
    void renderFood() const;

    //prop
    void eatPropResult();
    void renderProp() const;//render prop
    void createRandomProp();//create random prop
    bool isPartOfProp(int x , int y);
    void adjustProp();

    //score
    void createRandomScoreHead(int& x , int& y);
    void createRandomScore();
    void renderScore();
    bool isEmpty(int x , int y);
    bool inBound(int x , int y);
    bool Road_is_complete();
    //block
    void createBlock(int x , int y);
    void createBlock();
    void createRandomBlock(int n);
    void renderBlock();

    //snake
    void renderSnake() const;
    void controlSnake(int& escfor) const;

    //game
		void startGame();
    bool renderRestartMenu() const;
    void renderHelp();
    int renderEscMenu() const;
    void adjustDelay();
    void adjustLife();
    bool is_dead();

    //file
    bool writeGameFile();
    bool readGameFile();
    bool GameFile_is_empty();
    bool whether_to_read_file();


private:
    // We need to have two windows
    // One is for game introduction
    // One is for game mWindows
    int mScreenWidth;
    int mScreenHeight;
    int mGameBoardWidth;
    int mGameBoardHeight;
    const int mInformationHeight = 6;
    const int mInstructionWidth = 18;
    std::vector<WINDOW *> mWindows;
    // Snake information
    const int mInitialSnakeLength = 2;
    const char mSnakeSymbol = '@';
    std::unique_ptr<Snake> mPtrSnake;
    // Food information
    SnakeBody mFood;
    int food_num;
    const char mFoodSymbol = '#';
    //ScoreRoad
    ScoreRoad mRoad;
    ScoreRoad mcompleteRoad;
    const int road_length = 4;
    //block
    Block mBlock;
    const char mBlockSymbol = 'X';
    const int startBlockNum = 10;
    //Prop information
    SnakeBody mProp;
    int PropNum = 0;
    const char mPropSymbol1 = '$';
    const char mPropSymbol2 = '*';
    const char mPropSymbol3 = '%';
    const char mPropSymbol4 = '>';
    const char mPropSymbol5 = '<';


    //游戏参数
    int mPoints = 0;
    int mDifficulty = 0;
    int mBaseDelay = 100;
    int mDelay;
    int mLife = 1;
    int mRedFoodNum = 0;
    //文件
    const std::string mRecordBoardFilePath = "record.dat";
    const std::string mSavedFilePath = "gamefile.txt";
    //其他
    std::vector<int> mLeaderBoard;
    const int mNumLeaders = 3;
};

#endif
