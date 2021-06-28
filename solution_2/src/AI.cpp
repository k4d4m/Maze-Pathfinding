#include "AI.h"
#include <cstdlib>
#include <iostream>


AI::AI()
{
    //ctor
}

AI::~AI()
{
    //dtor
}


int AI::main(Player p, vector<vector<char> > map)
{

    bool available_treasure=false;
    int hero_counter=0;
    for(int i=0; i<(int)map.size(); i++)
    {
        for(int j=0; j<(int)map[i].size(); j++)
        {
            if(map[i][j]=='h')hero_counter++;
            if(map[i][j]=='k')available_treasure=true;
        }
    }
    if(!available_treasure)
    {
        cout<<"AI: no more loot here"<<endl;
        return -1;
    }
    if(hero_counter!=1)
    {
        cout<<"AI: I feel dizzy"<<endl;
        return -2;
    }

    cout<<"AI: generating path..."<<endl;

    AI_Position initial_pos(p,map);
    position.resize(1);
    position[0]=initial_pos;


    int depth=1;
    for(int i=0; i<depth; i++)
    {
        if(!doable())
        {
            for (int j = 0; j < (int)position[0].visited.size(); j++)
                for(int k=0; k<(int) position[0].visited[j].size(); k++)
                    position[0].visited[j][k]=0;

            ///
            path_find(position[0], position, i);
            ///

        }
    }

    cout<<"AI: map explored!"<<endl;

    if(doable())
    {
        cout<<"AI: best solution: "<<best_solution().directions.size()<<" steps"<<endl;
        //print_steps(best_solution());
        return 1;
    }
    else
    {
        cout<<"AI: no solution found (depth="<<depth<<")"<<endl;
        // for(int i=0; i<position.size(); i++)
        // print_steps(position[i]);
        // system("pause");
        return 0;
    }
}


void AI::path_find(AI_Position pos, vector<AI_Position> &position, int &depth)
{

    if (pos.p.get_treasure() && pos.p.get_hp()>0)
    {
        position.push_back(pos);
        return;
    }
    else if(pos.p.get_hp()<1)
        return;

    pos.visited[pos.p.get_y()][pos.p.get_x()]++;

    //south (y++)
    if (is_valid(pos, pos.p.get_y()+1, pos.p.get_x(), depth))
    {
        AI_Position tmp_pos=pos;
        if(pos.p.move('s', pos.map))
        {
            pos.directions.push_back('s');
            path_find(pos, position, depth); //recursion
            pos=tmp_pos;
        }

    }
    //east (x++)
    if (is_valid(pos, pos.p.get_y(), pos.p.get_x()+1, depth))
    {
        AI_Position tmp_pos=pos;
        if(pos.p.move('e', pos.map))
        {
            pos.directions.push_back('e');
            path_find(pos, position, depth); //recursion
            pos=tmp_pos;
        }
    }
    //north (y--)
    if (is_valid(pos, pos.p.get_y()-1, pos.p.get_x(), depth))
    {
        AI_Position tmp_pos=pos;
        if(pos.p.move('n', pos.map))
        {
            pos.directions.push_back('n');
            path_find(pos, position, depth); //recursion
            pos=tmp_pos;
        }
    }
    //west (x--)
    if (is_valid(pos, pos.p.get_y(), pos.p.get_x()-1, depth))
    {
        AI_Position tmp_pos=pos;
        if(pos.p.move('w', pos.map))
        {
            pos.directions.push_back('w');
            path_find(pos, position, depth); //recursion
            pos=tmp_pos;
        }
    }
    //backtrack
    pos.visited[pos.p.get_y()][pos.p.get_x()]--;
}


bool AI::is_valid(AI_Position &pos, int y, int x, int &depth)
{
    if (
        x>=0 &&
        y>=0 &&
        x<(int)pos.map[0].size()-1 &&
        y<(int)pos.map.size()-1 &&
        pos.visited[y][x]<1 //TODO depth
    )
        return true;
    return false;
}


bool AI::doable()
{
    for(int i=0; i<(int)position.size(); i++)
        if(position[i].p.get_treasure() && position[i].p.get_hp()>0)
            return true;
    return false;
}


AI_Position AI::best_solution()
{
    //init
    AI_Position best;
    for(int i=0; i<(int)position.size(); i++)
    {
        if(position[i].p.get_treasure() && position[i].p.get_hp()>0)
        {
            best=position[i];
            break;
        }
    }
    //best
    for(int i=0; i<(int)position.size(); i++)
    {
        if(position[i].p.get_treasure() && position[i].p.get_hp()>0 && position[i].directions.size()<best.directions.size())
            best=position[i];
    }
    return best;
}


void AI::print_steps(AI_Position pos)
{
    for(int i=0; i<(int)pos.directions.size(); i++)
        cout<<pos.directions[i]<<", ";
    cout<<endl<<endl;
}

