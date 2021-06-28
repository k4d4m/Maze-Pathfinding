#ifndef AI_POSITION_H
#define AI_POSITION_H

#include "Player.h"
#include <vector>
#include <string>

using namespace std;

class AI_Position
{

public:
    AI_Position();
    Player p;
    int traps_activated, decision_count;
    vector<vector<char> > map; //[H][W]
    vector<vector<int> > blueprint; //[H][W]
    vector<vector<bool> > approach_option; //[decision-1][N,S,W,E] - FROM direction!
    vector<vector<int > > step_history; //[step no][Y,X]
    bool equals(AI_Position);

    //int id, ancestor_id, decision_count;
    //vector<int > decisions;
    //int aproach_count;

};

#endif // AI_POSITION_H
