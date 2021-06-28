#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

#include "Player.h"
#include "AI.h"

using namespace std;

class Game
{

public:

    Game();
    virtual ~Game();

private:
    string path;
    int map_width, map_height;
    vector<vector<char> > map;
    bool gameover, win;
    Player p;
    AI ai;
    void map_init(bool re);
    void map_load();
    void map_display();
    void main();
    void control();
    void AI_Playthrough();

};

#endif // GAME_H
