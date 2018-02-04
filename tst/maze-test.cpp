#include "maze.hpp"
#include <stdio.h>

int main(){
    int maze_size = 10;
    Maze* maze = new Maze(maze_size, maze_size);
    bool test1 = true;
    for(int i = 0; i < maze_size; i++){
        for(int j = 0; j < maze_size; j++){
            if(maze->hasNorthernWall(i, j) || maze->hasEasternWall(i, j) || maze->hasSouthernWall(i, j) || maze->hasWesternWall(i, j)) test1 = false;
            if(!(maze->addNorthernWall(i, j) && maze->hasNorthernWall(i, j))) test1 = false;
            if(!(maze->addEasternWall(i, j) && maze->hasEasternWall(i, j))) test1 = false;
            if(!(maze->addSouthernWall(i, j) && maze->hasSouthernWall(i, j))) test1 = false;
            if(!(maze->addWesternWall(i, j) && maze->hasWesternWall(i, j))) test1 = false;
            if(!maze->removeNorthernWall(i, j) || maze->hasNorthernWall(i, j)) test1 = false;
            if(!maze->removeEasternWall(i, j) || maze->hasEasternWall(i, j)) test1 = false;
            if(!maze->removeSouthernWall(i, j) || maze->hasSouthernWall(i, j)) test1 = false;
            if(!maze->removeWesternWall(i, j) || maze->hasWesternWall(i, j)) test1 = false;
        }
    }
    printf("Test 1: Adding, removing, and checking walls works properly: %s\n", test1 ? "true" : "false");

    for(int i = 0; i < maze_size; i++){
        for(int j = 0; j < maze_size; j++){
            maze->addNorthernWall(i, j);
            maze->addEasternWall(i, j);
            maze->addSouthernWall(i, j);
            maze->addWesternWall(i, j);
        }
    }
    printf("Test2: Printing Ascii Maze with walls everywhere:\n%s", maze->getASCIIMaze());

    Maze* maze2 = new Maze(maze->getASCIIMaze());
    printf("Test3: Printing second maze, constructed from the previous ASCII representation:\n%s", maze2->getASCIIMaze());
}  
