#include "Player.h"


Player::Player()
{
    hp=2;
    weapon=false;
    treasure=false;
    gg=false;
}

Player::~Player()
{

}

bool Player::move(char direction, vector<vector<char> > &map)
{
    char item;
    bool moved=false;

    switch(direction)
    {
    case 'w': //West (x--)
    {
        item = map[get_y()][get_x()-1];
        if(item!='x' && item!='c' && get_x()>0)
        {
            map[get_y()][get_x()]=' ';
            set_x(get_x()-1);
            map[get_y()][get_x()]='h';
            moved=true;
        }
        else if(item=='c' && get_x()>1 && map[get_y()][get_x()-2]!='x' && map[get_y()][get_x()-2]!='c')
        {
            item=map[get_y()][get_x()-2];
            map[get_y()][get_x()]=' ';
            set_x(get_x()-2);
            map[get_y()][get_x()]='h';
            map[get_y()][get_x()+1]='x';
            moved=true;
        }
        break;
    }
    case 'e': //East (x++)
    {
        item = map[get_y()][get_x()+1];
        if(item!='x' && item!='c' && get_x()<(int)map[get_y()].size()-1)
        {
            map[get_y()][get_x()]=' ';
            set_x(get_x()+1);
            map[get_y()][get_x()]='h';
            moved=true;
        }
        else if(item=='c' && get_x()<(int)map[get_y()].size()-2 && map[get_y()][get_x()+2]!='x' && map[get_y()][get_x()+2]!='c')
        {
            item=map[get_y()][get_x()+2];
            map[get_y()][get_x()]=' ';
            set_x(get_x()+2);
            map[get_y()][get_x()]='h';
            map[get_y()][get_x()-1]='x';
            moved=true;
        }
        break;
    }
    case 'n': //North (y--)
    {
        item = map[get_y()-1][get_x()];
        if(item!='x' && item!='c' && get_y()>0)
        {
            map[get_y()][get_x()]=' ';
            set_y(get_y()-1);
            map[get_y()][get_x()]='h';
            moved=true;
        }
        else if(item=='c' && get_y()>2 && map[get_y()-2][get_x()]!='x' && map[get_y()-2][get_x()]!='c')
        {
            item=map[get_y()-2][get_x()];
            map[get_y()][get_x()]=' ';
            set_y(get_y()-2);
            map[get_y()][get_x()]='h';
            map[get_y()+1][get_x()]='x';
            moved=true;
        }
        break;
    }
    case 's': //South (y++)
    {
        item = map[get_y()+1][get_x()];
        if(item!='x' && item!='c' && get_y()<(int)map.size()-1)
        {
            map[get_y()][get_x()]=' ';
            set_y(get_y()+1);
            map[get_y()][get_x()]='h';
            moved=true;
        }
        else if(item=='c' &&  get_y()<(int)map.size()-2 && map[get_y()+2][get_x()]!='x' && map[get_y()+2][get_x()]!='c')
        {
            item=map[get_y()+2][get_x()];
            map[get_y()][get_x()]=' ';
            set_y(get_y()+2);
            map[get_y()][get_x()]='h';
            map[get_y()-1][get_x()]='x';
            moved=true;
        }
        break;
    }
    }
interact(item);
    if(moved)
    {

        return true;
    }
    else return false;
}


void Player::interact(char item)
{
    switch(item)
    {
    case 'k': //treasure
    {
        set_treasure(true);
        break;
    }
    case 'i': //hp poti
    {
        if(get_hp()<2)set_hp(get_hp()+1);
        break;
    }
    case 'c': //trap
    {
        //implemented in movement
        break;
    }
    case 'a': //weapon
    {
        set_weapon(true);
        break;
    }
    case 'j': //exit
    {
        set_gg(true);
        break;
    }
    case 's': //monster
    {
        if(get_weapon())set_hp(get_hp()-1);
        else set_hp(0);
        break;
    }
    }
}

int Player::get_hp()
{
    return this->hp;
}

void Player::set_hp(int hp)
{
    this->hp=hp;
}

int Player::get_x()
{
    return this->x;
}

void Player::set_x(int x)
{
    this->x=x;
}

int Player::get_y()
{
    return this->y;
}

void Player::set_y(int y)
{
    this->y=y;
}

bool Player::get_weapon()
{
    return this->weapon;
}

void Player::set_weapon(bool weapon)
{
    this->weapon=weapon;
}

bool Player::get_treasure()
{
    return this->treasure;
}

void Player::set_treasure(bool treasure)
{
    this->treasure=treasure;
}

bool Player::get_gg()
{
    return gg;
}

void Player::set_gg(bool gg)
{
    this->gg=gg;
}

