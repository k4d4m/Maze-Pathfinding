#include "Player.h"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <conio.h>
#include <vector>

using namespace std;

Player::Player()
{
    init();
}

Player::~Player()
{

}

void Player::init()
{
    hp=2;
    set_weapon(false);
    set_treasure(false);
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

bool Player::move(char direction, vector<vector<char> > &map, int map_width, int map_height, bool ai_control, bool &win, bool &gameover)
{
    char to;
    bool moved=false;

    switch(direction)
    {
    case 'w': //West (x--)
    {
        to = map[get_y()][get_x()-1];
        if(to!='x' && to!='c' && get_x()>0)
        {
            map[get_y()][get_x()]=' ';
            set_x(get_x()-1);
            map[get_y()][get_x()]='h';
            moved=true;
        }
        else if(to=='c' && get_x()>1 && map[get_y()][get_x()-2]!='x' && map[get_y()][get_x()-2]!='c')
        {
            to=map[get_y()][get_x()-2];
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
        to = map[get_y()][get_x()+1];
        if(to!='x' && to!='c' && get_x()<map_width-1)
        {
            map[get_y()][get_x()]=' ';
            set_x(get_x()+1);
            map[get_y()][get_x()]='h';
            moved=true;
        }
        else if(to=='c' && get_x()<map_width-2 && map[get_y()][get_x()+2]!='x' && map[get_y()][get_x()+2]!='c')
        {
            to=map[get_y()][get_x()+2];
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
        to = map[get_y()-1][get_x()];
        if(to!='x' && to!='c' && get_y()>0)
        {
            map[get_y()][get_x()]=' ';
            set_y(get_y()-1);
            map[get_y()][get_x()]='h';
            moved=true;
        }
        else if(to=='c' && get_y()>2 && map[get_y()-2][get_x()]!='x' && map[get_y()-2][get_x()]!='c')
        {
            to=map[get_y()-2][get_x()];
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
        to = map[get_y()+1][get_x()];
        if(to!='x' && to!='c' && get_y()<map_height-1)
        {
            map[get_y()][get_x()]=' ';
            set_y(get_y()+1);
            map[get_y()][get_x()]='h';
            moved=true;
        }
        else if(to=='c' && get_y()<map_height-2 && map[get_y()+2][get_x()]!='x' && map[get_y()+2][get_x()]!='c')
        {
            to=map[get_y()+2][get_x()];
            map[get_y()][get_x()]=' ';
            set_y(get_y()+2);
            map[get_y()][get_x()]='h';
            map[get_y()-1][get_x()]='x';
            moved=true;
        }
        break;
    }

    }

    interact(win, gameover, to);
    if(moved) return true; //&& !ai_control
    else return false;

}
void Player::interact(bool &win, bool &gameover, char to)
{
    switch(to)
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
        if(get_treasure())win=true;
        gameover=true;
        break;
    }
    case 's': //monster
    {
        if(get_weapon())set_hp(get_hp()-1);
        else set_hp(0);
        if (get_hp()<=0)gameover=true;
        break;
    }
    }
}

bool Player::equals(Player other)
{
    if(
        this->hp==other.get_hp() &&
        this->x==other.get_x() &&
        this->y==other.get_y() &&
        this->weapon==other.get_weapon() &&
        this->treasure==other.get_treasure()
    )return true;
    return false;
}
