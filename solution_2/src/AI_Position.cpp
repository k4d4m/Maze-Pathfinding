#include "AI_Position.h"


AI_Position::AI_Position()
{
    Player new_player;
    this->p=new_player;

    vector<vector<char> > new_map;
    this->map=new_map;

    vector<char> new_directions;
    this->directions=new_directions;

    vector<vector<int> > new_visited;
    this->visited=new_visited;

}


AI_Position::AI_Position(Player p, vector<vector<char> > map)
{
    this->p=p;
    this->map=map;

    vector<vector<int> > new_visited;
    new_visited.resize(map.size());
    for (int i = 0; i < (int)map.size(); i++)
    {
        new_visited[i].resize(map[i].size());
        for(int j=0; j<(int)new_visited[i].size(); j++)
            new_visited[i][j]=1;
    }
    this->visited=new_visited;

    vector<char> new_directions;
    new_directions.resize(0);
    this->directions=new_directions;
}

