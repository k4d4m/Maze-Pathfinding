#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

using namespace std;


class Player
{

public:

    Player();
    virtual ~Player();

    bool move(char direction, vector<vector<char> > &map);
    void interact(char to);

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

    bool get_gg();
    void set_gg(bool gg);

private:

    int hp;
    int x,y;
    bool weapon, treasure, gg;

};

#endif // PLAYER_H
