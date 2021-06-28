#ifndef AI_H
#define AI_H

#include "AI_Position.h"


class AI
{

public:

    AI();
    virtual ~AI();
    int main(Player p, vector<vector<char> > map);
    AI_Position best_solution();

private:

    vector<AI_Position > position;
    void path_find(AI_Position pos, vector<AI_Position> &positions, int &depth);
    bool is_valid(AI_Position &pos, int y, int x, int &depth);
    bool doable();
    void print_steps(AI_Position position);

};

#endif // AI_H
