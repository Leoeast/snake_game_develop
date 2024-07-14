#include <vector>
#include "block.h"

using namespace std;
//blockone
BlockOne::BlockOne(int c , int r)
{
    x = c;
    y = r;
}
int BlockOne::get_x()
{
    return x;
}

int BlockOne::get_y()
{
    return y;
}



//block
Block::Block()
{

}

bool Block::isPartOfBlock(int x , int y)
{
    for(int i = 0  ; i < get_length() ; i ++)
    {
        if(x == Block_vec[i].get_x() && y == Block_vec[i].get_y())return true;
    }
    return false;
}

int Block::get_length()
{
    return Block_vec.size();
}
std::vector<BlockOne>& Block::get_block_vec()
{
    return Block_vec;
}
