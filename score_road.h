#ifndef SCORE_ROAD_H_INCLUDED
#define SCORE_ROAD_H_INCLUDED

#include <vector>

using namespace std;

class ScoreOne
{
public:
    ScoreOne(int c , int r , int ord);
    int get_x() const;
    int get_y() const;
    int get_ord() const;



private:
    int x;
    int y;
    int order;
    char symbol;
};

class ScoreRoad
{
public:
    ScoreRoad();
    ScoreRoad(int x , int y);
    std::vector<ScoreOne>& get_road();
    bool isPartOfScore(int x , int y);
    int get_length();
    bool is_head(int x , int y);
    bool operator == (ScoreRoad& sroad);

private:
    std::vector<ScoreOne> Road_vec;

};

#endif // SCORE_ROAD_H_INCLUDED
