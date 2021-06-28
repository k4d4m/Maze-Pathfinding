#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <conio.h>
#include <ctime>
#include <windows.h>

#include "Game.h"
//#include "Player.h"
//#include "AI.h"

using namespace std;

Game::Game()
{
    Player p;
    //AI ai;
    main();
}

Game::~Game()
{
    //dtor
}


void Game::main()
{
    cout<<"Welcome to the game! Grab the treasure and get out alive!"<<endl<<"Press ENTER to load a level, ESC to exit."<<endl<<endl;
    bool exit=false, map_loaded=false;
    while(!exit)
    {
        if(kbhit()) //keypressed
        {
            char c = getch();
            switch(c)
            {
            case 13: //enter
            {
                p.init();
                gameover=false;
                win=false;
                map_init(false);
                map_loaded=true;
                control();
                break;
            }
            case 'r': //reloading
            {
                if(map_loaded)
                {
                    p.init();
                    gameover=false;
                    win=false;
                    map_init(true);
                    control();
                }
                break;
            }
            case 27:
            {
                exit=true; //esc
                break;
            }
            }
        }
    }
}


void Game::map_init(bool re)
{
    bool opened=false;
    while(!opened)
    {
        if(!re)
        {
            system("cls");
            cout << "Map file path: " ;
            cin >> path;
            //path="C:/map.txt";
        }


        ifstream mapfile(path.c_str());
        if (mapfile.is_open())
        {

            opened=true;
            string line;
            map_width=0;
            int i=0;
            while(mapfile.good())
            {
                getline(mapfile,line);
                if((int)line.length()>map_width) map_width=line.length();
                i++;
            }
            map_height=i;

            //considering line break at the end of file
            if(line.length()==0) map_height-=1;

            mapfile.clear();
            mapfile.seekg(0, ios::beg);
            mapfile.close();

            //set map size
            map.resize(map_height);
            for ( int i = 0; i < map_height; i++)
                map[i].resize(map_width);

            //cout<<"height: "<<map_height<<endl<<"width: "<<map_width<<endl<<endl;

        }
        else cout<<"Unable to open file"<<endl<<endl;

    }
    map_load();
}


void Game::map_load()
{
    ifstream mapfile(path.c_str());
    if (mapfile.is_open())
    {
        string line;
        for( int i=0; i<map_height; i++)
        {
            getline(mapfile,line);
            for( int j=0; j<map_width; j++)
            {
                map[i][j]=line[j];

                //init player location
                if(map[i][j]=='h')
                {
                    p.set_y(i);
                    p.set_x(j);
                }

            }
        }
        mapfile.clear();
        mapfile.seekg(0, ios::beg);
        mapfile.close();
    }
    else cout << "Unable to open file";
}

//color util
WORD GetConsoleTextAttribute (HANDLE hCon)
{
    CONSOLE_SCREEN_BUFFER_INFO con_info;
    GetConsoleScreenBufferInfo(hCon, &con_info);
    return con_info.wAttributes;
}


void Game::map_display()
{
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    const int saved_colors = GetConsoleTextAttribute(hConsole);

    for(int i=0; i<map_height; i++)
    {
        for(int j=0; j<map_width; j++)
        {
            //coloring
            switch(map[i][j])
            {
            case'k':
            {
                SetConsoleTextAttribute(hConsole, 6 | FOREGROUND_INTENSITY);
                break;
            }
            case'i':
            {
                SetConsoleTextAttribute(hConsole, 9 | FOREGROUND_INTENSITY);
                break;
            }
            case'c':
            {
                SetConsoleTextAttribute(hConsole, 15 | FOREGROUND_INTENSITY);
                break;
            }
            case'a':
            {
                SetConsoleTextAttribute(hConsole, 13 | FOREGROUND_INTENSITY);
                break;
            }
            case'h':
            {
                SetConsoleTextAttribute(hConsole, 2 | FOREGROUND_INTENSITY);
                break;
            }
            case'x':
            {
                SetConsoleTextAttribute(hConsole, 8 | FOREGROUND_INTENSITY);
                break;
            }
            case'j':
            {
                SetConsoleTextAttribute(hConsole, 11 | FOREGROUND_INTENSITY);
                break;
            }
            case's':
            {
                SetConsoleTextAttribute(hConsole, 12 | FOREGROUND_INTENSITY);
                break;
            }
            }
            cout<<map[i][j];
            SetConsoleTextAttribute(hConsole, saved_colors);
        }

        //info bar
        if(i==0) cout<<"\tHP="<<p.get_hp()<<endl;
        else if (i==1) cout<<"\tWeapon="<<p.get_weapon()<<endl;
        else if(i==2 && p.get_treasure()) cout<<"\tTreasure="<<p.get_treasure()<<endl;
        else cout<<endl;
    }

    cout<<endl<<"Symbols: k-treasure, i-poti, c-trap, a-weapon, h-hero, x-wall, j-exit, s-monster"<<endl;
    if(!gameover)cout<<"Keys: Movement-WASD, AI-Enter, Exit-Esc"<<endl<<endl;
}


void Game::control()
{
    map_display();
    while(!gameover)
    {
        if(kbhit()) //keypressed
        {
            char c = getch();
            switch(c)
            {
            case 'a':
            {
                if(p.move('w', map, map_width, map_height, false, win, gameover)) map_display();
                break;
            }
            case 'd':
            {
                if(p.move('e', map, map_width, map_height, false, win, gameover)) map_display();
                break;
            }
            case 'w':
            {
                if(p.move('n', map, map_width, map_height, false, win, gameover)) map_display();
                break;
            }
            case 's':
            {
                if(p.move('s', map, map_width, map_height, false, win, gameover)) map_display();
                break;
            }
            case 13: //enter
            {
                AI_Playthrough();
                break;
            }
            case 27: //esc
            {
                gameover=true;
                map_display();
                break;
            }

            }
        }
    }
    if(win)cout<<"Congratulations! You won!"<<endl;
    else cout<<"You lost."<<endl;
    cout<<"Press R to restart this level, ENTER to load another level, or ESC to exit."<<endl<<endl;

}



void Game::AI_Playthrough()
{
    if(ai.main(map_width, map_height, map, p)==1)
    {
        for(int i=0; i<ai.directions.size(); i++)
        {
            p.move(ai.directions[i], map, map_width, map_height, false, win, gameover);
            map_display();
            Sleep(100);
        }
        ai.directions.resize(0);
    }
}


