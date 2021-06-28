#ifndef AI_H
#define AI_H

#include "AI_Position.h"

#include <vector>
#include <string>

using namespace std;

class AI
{

public:

    AI();
    virtual ~AI();
    int main(int map_width, int map_height, vector<vector<char> > map, Player p);
    vector<char> directions;

private:

    int  map_width, map_height, position_count, tmp_position_count;

    vector<AI_Position > position; //situation container
    vector<AI_Position > tmp_position; //for delayed decision making
    vector<bool> forged_blueprint;
    vector<vector<int> > steps; //stores one path

    int get_option_count(AI_Position &current_pos);
    int approach_count(vector<bool> approach_options);
    int oldestroute(int x, int y);

    bool solvable();
    bool consider_option(AI_Position &current_pos,int i, int j, char from_direction, bool recursion);
    bool beenthere(int x, int y);

    char get_single_approach_from_direction(AI_Position &position,int selected_option);
    char towards_direction(char from_direction);
    char get_option_item(AI_Position &current_pos, int option);

    void explore(AI_Position current_pos, int decision);
    void create_blueprint(AI_Position &current_pos); //discover sector options
    void simple_decision(AI_Position &current_pos, int decision, bool single_approach_trap); //place player directly to location and interact
    void raw_solutions();
    AI_Position best_solution();
    void display_position_info();
    void print_steps(AI_Position position);

    void create_option(AI_Position &current_pos,int i,int j,int aproach_index, int &option_mark);
    void display_map(AI_Position &current_pos);//,  int &option_mark);
    void path_build(AI_Position &pos, int to_x, int to_y);
    void path_find(
        AI_Position &pos, int y, int x, int to_x, int to_y,
        vector<vector<bool> > visited, int &min_dist, int dist,
        vector<vector<int> >steps, vector<vector<vector<int> > > &paths
    );
    void optimize();
    bool isValid(AI_Position &pos, vector<vector<bool> > visited, int y, int x, int to_x, int to_y);
};

#endif // AI_H
