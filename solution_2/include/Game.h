#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "AI.h"
#include <string>


class Game
{

public:

    Game();
    virtual ~Game();

private:

    string path;
    vector<vector<char> > map;
    Player p;
    AI ai;
    void main();
    void map_init(bool re);
    void map_load();
    void map_display();
    void control();
    void AI_Playthrough();

};

#endif // GAME_H
