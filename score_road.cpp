#include "score_road.h"
#include <cmath>

using namespace std;


//score one

ScoreOne::ScoreOne(int c , int r , int ord)
{
    x = c;
    y = r;
    order = ord;
}


int ScoreOne::get_x() const
{
    return x;
}

int ScoreOne::get_y() const
{
    return y;
}

int ScoreOne::get_ord() const
{
    return order;
}

//score road
ScoreRoad::ScoreRoad()
{

}

ScoreRoad::ScoreRoad(int x , int y)
{
    Road_vec.push_back(ScoreOne(x , y , 1));
}

std::vector<ScoreOne>& ScoreRoad::get_road()
{
    return Road_vec;
}

bool ScoreRoad::isPartOfScore(int x , int y)
{
    for(int i = 0 ; i < Road_vec.size() ; i ++)
    {
        if(x == Road_vec[i].get_x() && y == Road_vec[i].get_y())return true;
    }
    return false;
}

int ScoreRoad::get_length()
{
    return Road_vec.size();
}

bool ScoreRoad::is_head(int x , int y)
{
    if(x == Road_vec[0].get_x() && y == Road_vec[0].get_y())return true;
    else return false;
}

bool ScoreRoad::operator == (ScoreRoad& sroad)
{
    if(!get_length() == sroad.get_length())return false;
    for(int i = 0 ; i < get_length() ; i ++)
    {
        if(!(Road_vec[i].get_x() == sroad.get_road()[i].get_x() && Road_vec[i].get_y() == sroad.get_road()[i].get_y()))return false;
    }
    return true;
}


