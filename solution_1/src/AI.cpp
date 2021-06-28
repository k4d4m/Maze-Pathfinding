#include "AI.h"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <conio.h>
#include <ctime>
#include <windows.h>
#include <vector>

using namespace std;

AI::AI()
{
    //this->position.resize(1);
}

AI::~AI()
{
    //dtor
}


int AI::main(int map_width, int map_height, vector<vector<char> > map, Player p)
{

    bool available_treasure=false;
    int hero_counter=0;
    for(int i=0; i<map_height; i++)
    {
        for(int j=0; j<map_width; j++)
        {
            if(map[i][j]=='h')hero_counter++;
            if(map[i][j]=='k')available_treasure=true;
        }
    }
    if(!available_treasure)
    {
        cout<<"no treasure"<<endl;
        return -1;
    }
    if(hero_counter!=1)
    {
        cout<<"invalid hero number"<<endl;
        return -2;
    }


    cout<<"AI: generating path..."<<endl;

    this->map_width=map_width;
    this->map_height=map_height;

    //Init position
    position_count=1;
    position.resize(position_count);

    tmp_position_count=0;
    tmp_position.resize(tmp_position_count);

    forged_blueprint.resize(position_count);

    AI_Position initial_pos;
    initial_pos.map=map;
    initial_pos.p=p;

    initial_pos.blueprint.resize(map_height);
    for (int i = 0; i < map_height; i++)
        initial_pos.blueprint[i].resize(map_width);

    initial_pos.step_history.resize(1);
    for (int i = 0; i < (int)initial_pos.step_history.size(); i++)
        initial_pos.step_history[i].resize(2);
    initial_pos.step_history[initial_pos.step_history.size()-1][0]=initial_pos.p.get_y();
    initial_pos.step_history[initial_pos.step_history.size()-1][1]=initial_pos.p.get_x();

    initial_pos.decision_count=0;
    initial_pos.traps_activated=0;

    create_blueprint(initial_pos);
    position[position_count-1]=initial_pos;

    //Exploration core
    if(get_option_count(position[0])>0)
    {

        int from=0;
        bool done=false;
        int tmp_flag = tmp_position_count;
        while(!done)
        {
            int to=position_count;

            for(int i=from; i<to; i++)
            {
                if(!forged_blueprint[i])
                    create_blueprint(position[i]);

                if(get_option_count(position[i])>0)
                {
                    for(int j=0; j<get_option_count(position[i]); j++)
                    {
                        bool history_repeats_itself=false;
                        for(int k=0; k<i; k++)if(position[k].equals(position[i]))history_repeats_itself=true;

                        if(position[i].p.get_hp()>0 && !position[i].p.get_treasure() && !history_repeats_itself)
                            explore(position[i],j+1);
                    }
                }
            }
            if(to<position_count) //there are new positions to explore
            {
                done=false;
                from=to;
            }
            else if(tmp_flag<tmp_position_count)  //new tmp_positions
            {
                done=false;
                from=to;

                int c=position_count+tmp_position_count;
                forged_blueprint.resize(c);
                for(int i=0; i< c; i++)
                    forged_blueprint[i]=false;


                for(int i=tmp_flag; i<tmp_position_count; i++)
                {
                    position_count++;
                    position.resize(position_count);
                    AI_Position new_pos = tmp_position[i];
                    position[position_count-1] = new_pos;

                    forged_blueprint[position_count-1]=true;

                }
                tmp_flag = tmp_position_count;
            }
            else done=true;
        }
    }

    //cout<<"AI: map explored!"<<endl;
    if(solvable())
    {
        //cout<<"AI: Solution found!"<<endl<<endl;
        cout<<"AI: best solution: "<<best_solution().step_history.size()<<" steps"<<endl;
        print_steps(best_solution());
        //raw_solutions();

        steps.resize(best_solution().step_history.size());
        for(int i=0; i<(int)best_solution().step_history.size(); i++)
            steps[i].resize(2);
        steps=best_solution().step_history;

        //creating directions from best solution
        directions.resize(steps.size());
        for(int i=0; i<(int)steps.size()-1; i++)
        {
            if(steps[i][0]<steps[i+1][0]) directions[i]='s';
            if(steps[i][0]>steps[i+1][0]) directions[i]='n';
            if(steps[i][1]<steps[i+1][1]) directions[i]='e';
            if(steps[i][1]>steps[i+1][1]) directions[i]='w';
        }
        //system("pause");
        return 1;
    }
    else
    {
        cout<<"AI: no solution found"<<endl<<endl;
        system("pause");
        //display_position_info();
        return 0;
    }

}

void AI::explore(AI_Position current_pos, int decision)
{

    if(get_option_count(current_pos)>0 && decision>0)
    {

        position_count++;
        position.resize(position_count);
        AI_Position new_pos = current_pos;
        position[position_count-1] = new_pos;

        if(get_option_item(position[position_count-1],decision)!='c')
            simple_decision(position[position_count-1],decision,false); //simple decision
        else if(get_option_item(position[position_count-1],decision)=='c' && approach_count(position[position_count-1].approach_option[decision-1])==1)
            simple_decision(position[position_count-1],decision,true); //trap with obvious approach
        else if(get_option_item(position[position_count-1],decision)=='c' && approach_count(position[position_count-1].approach_option[decision-1])>1)
        {
            //trap with multiple approach options
            //solution: separating each approach into single approach situations
            for(int i=0; i<4; i++)
            {
                if(position[position_count-1].approach_option[decision-1][i]) // approachable
                {
                    tmp_position_count++;
                    tmp_position.resize(tmp_position_count);
                    AI_Position new_pos = position[position_count-1];
                    tmp_position[tmp_position_count-1] = new_pos;

                    for(int j=0; j<4; j++) //trim: remove all approach options except currently observed
                        if(i!=j)
                            tmp_position[tmp_position_count-1].approach_option[decision-1][j]=false;

                    for(int j=0; j<map_height; j++) //making sure the only option will be the trap
                    {
                        for(int k=0; k<map_width; k++)
                        {
                            if(tmp_position[tmp_position_count-1].blueprint[j][k] == decision)
                                tmp_position[tmp_position_count-1].blueprint[j][k]=1;
                            else if(tmp_position[tmp_position_count-1].blueprint[j][k]>0)
                                tmp_position[tmp_position_count-1].blueprint[j][k]=0;
                        }
                    }
                }
            }
            position[position_count-1].p.set_hp(0); //nop
        } // end of multi approach
        create_blueprint( position[position_count-1]);
    }

}


void AI::simple_decision(AI_Position &current_pos, int decision, bool single_approach_trap)
{
    bool destination_set=false;
    char from='?';
    int to_x=-666, to_y=-666;
    //int original_destination_x=-666, original_destination_y=-666;

    current_pos.decision_count++;

    if(!single_approach_trap)
    {
        for(int i=0; i<map_height; i++)
        {
            for(int j=0; j<map_width; j++)
            {
                if(current_pos.blueprint[i][j]==decision)
                {
                    to_y=i;
                    to_x=j;
                    destination_set=true;
                }
            }
        }
    }
    else if(single_approach_trap) //getting obvious approach position
    {
        for(int i=0; i<map_height; i++)
        {
            for(int j=0; j<map_width; j++)
            {
                if(current_pos.blueprint[i][j]==decision)
                {
                    to_y=i;
                    to_x=j;
                    //original_destination_x=to_x;
                    //original_destination_y=to_y;

                    switch(get_single_approach_from_direction(current_pos,decision))
                    {
                    case 'n':
                    {
                        from='n';
                        to_y--;
                        break;
                    }
                    case 's':
                    {
                        from='s';
                        to_y++;
                        break;
                    }
                    case 'e':
                    {
                        from='e';
                        to_x++;
                        break;
                    }
                    case 'w':
                    {
                        from='w';
                        to_x--;
                        break;
                    }

                    }
                    destination_set=true;
                }
            }
        }
    }

    if(destination_set)
    {

        path_build(current_pos,to_x,to_y);

        char to=current_pos.map[to_y][to_x];

        if(to!='c') //shouldn't be able to step on trap directly
        {
            bool tmp1=false,tmp2=false; //todo
            current_pos.p.interact(tmp1,tmp2,to);
            current_pos.map[current_pos.p.get_y()][current_pos.p.get_x()]=' ';
            current_pos.map[to_y][to_x]='h';
            current_pos.p.set_x(to_x);
            current_pos.p.set_y(to_y);

            if(single_approach_trap)
            {
                //move through trap, and store step history
                if(current_pos.p.move(towards_direction(from),current_pos.map,map_width,map_height,true,tmp1,tmp2))
                {
                    current_pos.traps_activated++;

                    //trap location //well the player never actually stands on it so it does not count
                    /*current_pos.step_history.resize(current_pos.step_history.size()+1);
                    for (int i = 0; i < (int)current_pos.step_history.size(); i++)
                        current_pos.step_history[i].resize(2);
                    current_pos.step_history[current_pos.step_history.size()-1][0]=original_destination_y;
                    current_pos.step_history[current_pos.step_history.size()-1][1]=original_destination_x;*/


                    //trap arrival location
                    current_pos.step_history.resize(current_pos.step_history.size()+1);
                    for (int i = 0; i < (int)current_pos.step_history.size(); i++)
                        current_pos.step_history[i].resize(2);
                    current_pos.step_history[current_pos.step_history.size()-1][0]=current_pos.p.get_y();
                    current_pos.step_history[current_pos.step_history.size()-1][1]=current_pos.p.get_x();

                    //p.move contains interact and p.x/y sets

                }
                else current_pos.p.set_hp(0);
            }
        }
        else current_pos.p.set_hp(0);
    }
    else current_pos.p.set_hp(0);
}

void AI::path_build(AI_Position &pos, int to_x, int to_y)
{

    //path find init
    int min_dist = INT_MAX;
    int y=pos.p.get_y();
    int x=pos.p.get_x();
    steps.resize(0);
    vector<vector<vector<int> > >paths;
    paths.resize(0);
    vector<vector<bool> > visited;
    visited.resize(map_height);
    for (int i = 0; i < map_height; i++)
    {
        visited[i].resize(map_width);
        for(int j=0; j<map_width; j++) visited[i][j]=false;
    }

    //path finding recursion
    path_find(pos, y, x, to_x, to_y, visited, min_dist, 0, steps, paths);

    //paths found
    if(min_dist!=INT_MAX)
    {
        //selecting shortest path
        int shortest_index;
        for(int i=0; i<(int)paths.size(); i++)
        {
            if((int)paths[i].size()==min_dist)
            {
                shortest_index=i;
                break;
            }
        }

        //add shortest path to position step history
        int step_count_so_far=pos.step_history.size();
        pos.step_history.resize(step_count_so_far + paths[shortest_index].size());
        for (int i = step_count_so_far; i < (int)pos.step_history.size(); i++)
            pos.step_history[i].resize(2);

        for(int i=0; i<(int)paths[shortest_index].size()-1; i++)
        {
            pos.step_history[i+step_count_so_far][0]=paths[shortest_index][i+1][0];
            pos.step_history[i+step_count_so_far][1]=paths[shortest_index][i+1][1];
        }
        pos.step_history[pos.step_history.size()-1][0]=to_y;
        pos.step_history[pos.step_history.size()-1][1]=to_x;

    }
    else
    {
        //cout << "can't find path"<<endl;
        return;
    }
}


void AI::path_find(AI_Position &pos, int y, int x, int to_x, int to_y, vector<vector<bool> > visited, int &min_dist, int dist, vector<vector<int> >steps, vector<vector<vector<int> > > &paths)
{

    if (y==to_y && x==to_x)
    {
        min_dist = min(dist, min_dist);

        paths.resize(paths.size()+1);
        paths[paths.size()-1]=steps;

        return;
    }

    steps.resize(steps.size()+1);
    for(int k=0; k<(int)steps.size(); k++)
        steps[k].resize(2);

    steps[steps.size()-1][0]=y;
    steps[steps.size()-1][1]=x;

    visited[y][x]=true;

    //south
    if (isValid(pos, visited, y+1, x, to_x, to_y))
        path_find(pos, y+1, x, to_x, to_y,  visited, min_dist, dist+1, steps, paths);

    //east
    if (isValid(pos, visited, y, x+1, to_x, to_y))
        path_find(pos, y, x+1, to_x, to_y,  visited, min_dist, dist+1, steps, paths);

    //north
    if (isValid(pos, visited, y-1, x, to_x, to_y))
        path_find(pos, y-1, x, to_x, to_y,  visited, min_dist, dist+1, steps, paths);

    //west
    if (isValid(pos, visited, y, x-1, to_x, to_y))
        path_find(pos, y, x-1, to_x, to_y,  visited, min_dist, dist+1, steps, paths);

    visited[y][x] = false;
}


bool AI::isValid(AI_Position &pos, vector<vector<bool> > visited, int y, int x, int to_x, int to_y)
{
    if (x<map_width && y<map_height && x>=0 && y>=0)
    {
        if ((!visited[y][x] && pos.blueprint[y][x]!=-1) &&
                (pos.map[y][x]==' ' || (x==to_x && y==to_y)))
            return true;
    }
    return false;

}


void AI::create_blueprint(AI_Position &current_pos)
{

    /*Blueprint keys:
    0=?
    -1=obstacle
    -2=explorable location
    -3=explored location surroundings
    positive numbers=decision options*/


    //blueprint init
    for(int i=0; i<map_height; i++)
    {
        for(int j=0; j<map_width; j++)
        {
            if(current_pos.map[i][j]=='h' && current_pos.p.get_x()==j && current_pos.p.get_y()==i) current_pos.blueprint[i][j]=-2;
            else if(current_pos.map[i][j]=='x' || current_pos.map[i][j]=='j') current_pos.blueprint[i][j]=-1;
            else current_pos.blueprint[i][j]=0;
        }
    }


    //"L" trap filter
    for(int i=0; i<map_height; i++)
    {
        for(int j=0; j<map_width; j++)
        {
            bool surrounding_trap_x_axis=false, surrounding_trap_y_axis=false;

            if(current_pos.map[i][j]=='c')
            {
                if(i>0)
                    if(current_pos.map[i-1][j]=='c')
                        surrounding_trap_y_axis=true;
                if(i<map_height-1)
                    if(current_pos.map[i+1][j]=='c')
                        surrounding_trap_x_axis=true;
                if(j>0)
                    if(current_pos.map[i][j-1]=='c')
                        surrounding_trap_y_axis=true;
                if(j<map_width-1)
                    if(current_pos.map[i][j+1]=='c')
                        surrounding_trap_y_axis=true;

                if(surrounding_trap_x_axis && surrounding_trap_y_axis>1)
                    current_pos.blueprint[i][j]=-1;
            }
        }
    }

    bool done = false;
    int option_mark=0;
    while(!done)
    {
        done=true;
        for(int i=0; i<map_height; i++)
        {
            for(int j=0; j<map_width; j++)
            {
                if(current_pos.blueprint[i][j]==-2) //location to explore
                {
                    done=false;

                    if(i>0) //check northern surrounding
                    {
                        if(current_pos.blueprint[i-1][j]!=-3 && current_pos.blueprint[i-1][j]!=-1)
                        {
                            if(current_pos.map[i-1][j]==' ') current_pos.blueprint[i-1][j]=-2;
                            else if(consider_option(current_pos,i-1,j,'s',false)) create_option(current_pos,i-1,j,1,option_mark);
                        }
                    }
                    if(i<map_height-1) //check southern surrounding
                    {
                        if(current_pos.blueprint[i+1][j]!=-3 && current_pos.blueprint[i+1][j]!=-1)
                        {
                            if(current_pos.map[i+1][j]==' ') current_pos.blueprint[i+1][j]=-2;
                            else if(consider_option(current_pos,i+1,j,'n',false)) create_option(current_pos,i+1,j,0,option_mark);
                        }
                    }
                    if(j>0) //check western surrounding
                    {
                        if(current_pos.blueprint[i][j-1]!=-3 && current_pos.blueprint[i][j-1]!=-1)
                        {
                            if(current_pos.map[i][j-1]==' ' ) current_pos.blueprint[i][j-1]=-2;
                            else if(consider_option(current_pos,i,j-1,'e',false)) create_option(current_pos,i,j-1,3,option_mark);
                        }
                    }
                    if(j<map_width-1) //check eastern surrounding
                    {
                        if(current_pos.blueprint[i][j+1]!=-3 && current_pos.blueprint[i][j+1]!=-1)
                        {
                            if(current_pos.map[i][j+1]==' ') current_pos.blueprint[i][j+1]=-2;
                            else if(consider_option(current_pos,i,j+1,'w',false)) create_option(current_pos,i,j+1,2,option_mark);
                        }
                    }
                    current_pos.blueprint[i][j]=-3;

                }
            }
        }
    }
}


bool AI::consider_option(AI_Position &current_pos,int i, int j, char from_direction, bool recursion)
{

    if(current_pos.p.get_hp()<1 || current_pos.p.get_treasure()) return false; //don't bother

    if(
        (
            (current_pos.map[i][j]==' ' && recursion) ||
            current_pos.map[i][j]=='k' ||
            current_pos.map[i][j]=='i' ||
            current_pos.map[i][j]=='a'
        ) ||
        (
            current_pos.map[i][j]=='s' &&
            current_pos.p.get_hp()>1 &&
            current_pos.p.get_weapon()
        )
    ) return true;

    if(current_pos.map[i][j]=='c' && !recursion)  //checking trap surroundings
    {
        switch(from_direction)
        {
        case 'n':
        {
            if(map_height-1>i)
                if((current_pos.map[i-1][j]==' ' || current_pos.map[i-1][j]=='h')
                        && current_pos.map[i+1][j]!='c' && current_pos.map[i-1][j]!='c')
                    return consider_option(current_pos,i+1,j,'n', true);
            return false;
        }
        case 's':
        {
            if(i>0)
                if((current_pos.map[i+1][j]==' ' || current_pos.map[i+1][j]=='h')
                        && current_pos.map[i-1][j]!='c' && current_pos.map[i+1][j]!='c')
                    return consider_option(current_pos,i-1,j,'s', true);
            return false;
        }
        case 'w':
        {
            if(map_width-1>j)
                if((current_pos.map[i][j-1]==' ' || current_pos.map[i][j-1]=='h')
                        && current_pos.map[i][j+1]!='c' && current_pos.map[i][j-1]!='c')
                    return consider_option(current_pos,i,j+1,'w', true);
            return false;
        }
        case 'e':
        {
            if(j>0)
                if((current_pos.map[i][j+1]==' ' || current_pos.map[i][j+1]=='h')
                        && current_pos.map[i][j-1]!='c' && current_pos.map[i][j+1]!='c')
                    return consider_option(current_pos,i,j-1,'e', true);
            return false;
        }
        }
    }
    return false;
}

char AI::get_option_item(AI_Position &current_pos, int option)
{
    for(int i=0; i<map_height; i++)
    {
        for(int j=0; j<map_width; j++)
        {
            if(current_pos.blueprint[i][j]==option)
                return current_pos.map[i][j];
        }
    }
    cout<<"get_option_item fail"<<endl;
    return '?';
}


void AI::create_option(AI_Position &current_pos,int i,int j,int aproach_index, int &option_mark)
{

    if(current_pos.blueprint[i][j]<=0) //not an option yet
    {
        option_mark++;
        current_pos.blueprint[i][j]=option_mark; //make it an option

        current_pos.approach_option.resize(option_mark);
        for ( int k = 0; k < option_mark; k++)
            current_pos.approach_option[k].resize(4);

        for(int k=0; k<4; k++)
            current_pos.approach_option[option_mark-1][k]=false;
    }

    if(current_pos.map[i][j]=='c')
        current_pos.approach_option[option_mark-1][aproach_index]=true;
}


int AI::get_option_count(AI_Position &current_pos)
{
    int option_count=0;
    for(int i=0; i<map_height; i++)
    {
        for(int j=0; j<map_width; j++)
        {
            if(current_pos.blueprint[i][j]>0)option_count++;
        }
    }
    return option_count;
}


int AI::approach_count(vector<bool> approach_options)
{
    int counter=0;
    for(int i=0; i<4; i++)if(approach_options[i])counter++;
    return counter;
}


char AI::towards_direction(char from_direction)
{
    switch (from_direction)
    {
    case 'n':
        return 's';
    case 's':
        return 'n';
    case 'e':
        return 'w';
    case 'w':
        return 'e';
    }
    cout<<"error in towards_direction";
    return '?';
}

char AI::get_single_approach_from_direction(AI_Position &position,int selected_option)
{
    if(approach_count(position.approach_option[selected_option-1])==1)
    {
        for(int i=0; i<4; i++)
        {
            if( position.approach_option[selected_option-1][i] == true )
            {
                switch (i)
                {
                case 0:
                    return 'n';
                case 1:
                    return 's';
                case 2:
                    return 'w';
                case 3:
                    return 'e';
                }
            }
        }
    }
    cout<<"not single approach!";
    return '?';
}


bool AI::solvable()
{
    for(int i=0; i<position_count; i++)
        if(position[i].p.get_treasure() && position[i].p.get_hp()>0)
            return true;
    return false;
}

void AI::raw_solutions()
{
    int solution_counter=0;
    for(int i=0; i<position_count; i++)
    {
        if(position[i].p.get_treasure() && position[i].p.get_hp()>0)
        {
            solution_counter++;
            cout<<"solution "<<solution_counter<<": position["<<i<<"] "<<"steps="<<position[i].step_history.size();
            cout<<endl;
            print_steps(position[i]);
        }
    }
}

AI_Position AI::best_solution()
{
    //init
    AI_Position best;
    for(int i=0; i<position_count; i++)
    {
        if(position[i].p.get_treasure() && position[i].p.get_hp()>0)
        {
            best=position[i];
            break;
        }
    }

    //best
    for(int i=0; i<position_count; i++)
    {
        if(position[i].p.get_treasure() && position[i].p.get_hp()>0 && position[i].step_history.size()<=best.step_history.size())
            best=position[i];
    }
    return best;
}

bool is_legit(AI_Position &pos)
{
   ///TODO
   return true;
}

void AI::display_map(AI_Position &current_pos) //AI perspective
{
    system("cls");
    for(int i=0; i<map_height; i++)
    {
        for(int j=0; j<map_width; j++)
        {
            if(current_pos.map[i][j]=='h') cout<<'O';
            else
            {
                if(current_pos.blueprint[i][j]==-2) cout<<'k';
                if(current_pos.blueprint[i][j]==0) cout<<'?';
                if(current_pos.blueprint[i][j]==-1) cout<<'x';
                if(current_pos.blueprint[i][j]==-3) cout<<'*';
                if(current_pos.blueprint[i][j]>0) cout<<current_pos.blueprint[i][j];
            }
        }
        cout<<endl;
    }
    cout<<"position_count="<<position_count<<endl;
}

void AI::display_position_info()
{
    for(int i=0; i<position_count; i++)
    {
        cout<<"position["<<i<<"] x="<<position[i].p.get_x()<<" y="<<position[i].p.get_y();
        cout<<" hp="<<position[i].p.get_hp()<<" opt_count="<<get_option_count(position[i]);
        if(position[i].p.get_treasure()) cout<<" treasure="<<position[i].p.get_treasure();
        if(position[i].p.get_weapon()) cout<<" weapon="<<position[i].p.get_weapon();
        cout<<" steps="<<position[i].step_history.size();
        cout<<endl;
        print_steps(position[i]);
    }
    cout<<endl;
    system("pause");
}

void AI::print_steps(AI_Position position)
{
    for(int i=0; i<(int)position.step_history.size(); i++)
        cout<<'('<<position.step_history[i][0]<<','<<position.step_history[i][1]<<") ";
    cout<<endl<<endl;
    system("pause");
}

