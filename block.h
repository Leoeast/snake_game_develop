#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include <vector>

using namespace std;

class BlockOne
{
public:
    BlockOne(int c , int r);
    int get_x();
    int get_y();

private:
    int x;
    int y;
};

class Block
{
public:
    Block();
    int get_length();
    std::vector<BlockOne>& get_block_vec();
    bool isPartOfBlock(int x , int y);

private:
    std::vector<BlockOne> Block_vec;
};

#endif // BLOCK_H_INCLUDED
