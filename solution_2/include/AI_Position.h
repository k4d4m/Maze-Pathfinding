#ifndef AI_POSITION_H
#define AI_POSITION_H

#include "Player.h"


class AI_Position
{

public:

    AI_Position();
    AI_Position(Player p, vector<vector<char> > map);
    Player p;
    vector<vector<char> > map;
    vector<char> directions;
    vector<vector<int> > visited;

};

#endif // AI_POSITION_H
