#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

using namespace std;

class Player
{
public:
    Player();
    virtual ~Player();

    void init();

    int get_hp();
    void set_hp(int hp);

    int get_x();
    void set_x(int x);

    int get_y();
    void set_y(int y);

    bool get_weapon();
    void set_weapon(bool weapon);

    bool get_treasure();
    void set_treasure(bool treasure);

    bool move(char direction, vector<vector<char> > &map, int map_width, int map_height, bool ai_control, bool &win, bool &gameover);
    void interact(bool &win, bool &gameover, char to);

    bool equals(Player other);

private:
    int hp;
    int x,y;
    bool weapon, treasure;

};

#endif // PLAYER_H
