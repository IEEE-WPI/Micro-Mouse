#include "maze.hpp"

#include <stdexcept>
#include <stdio.h>
#include <string.h>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Begin Public Definitions of Maze ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

Maze::Maze(int num_rows, int num_cols){
    if(num_rows < 1){
        throw std::invalid_argument("The number of rows must be greater than one.");
    }
    if(num_cols < 1){
        throw std::invalid_argument("The number of cols must be greater than one.");
    }
    this->num_rows = num_rows;
    this->num_cols = num_cols;
    if(!allocateMaze()){
        throw std::bad_alloc();
    }
}

Maze::Maze(char* ascii_maze){
    int length = strlen(ascii_maze);
    int chars_in_last_row = 0;
    int chars_in_row = 0;
    int num_rows = 0;

    /* Obtain the number of rows and columns & verify the validity of the maze. */
    char msg[100];
    for(int i = 0; i < length; i++){
        chars_in_row++;
        char c = ascii_maze[i];
        if((c != EMPTY_CELL && c != ROW_DIVIDER && c != VERTICAL_WALL && c != HORIZONTAL_WALL) ||
                (c == VERTICAL_WALL && i % 2 == 1) || (c == HORIZONTAL_WALL && i % 2 == 0)){
            char fmt[100] = "Character '%c' at index %d is not valid.";
            sprintf(msg, fmt, c, i);
            throw std::invalid_argument(msg);
        }
        if(c == ROW_DIVIDER){
            if(num_rows++ != 0){
                if(chars_in_row != chars_in_last_row){
                    char fmt[100] = "Rows %d & %d have a different number of characters.";
                    sprintf(msg, fmt, num_rows, num_rows + 1);
                    throw std::invalid_argument(msg);
                }
            }
            chars_in_last_row = chars_in_row;
            chars_in_row = 0;
        }
    }
    this->num_rows = num_rows - 1;
    if(this->num_rows < 1){
        throw std::invalid_argument("There must be at least 1 row in the provided maze.");
    }
    this->num_cols = (chars_in_last_row - 2) / 2;
    if(this->num_cols < 1){
        throw std::invalid_argument("There must be at least 1 column in the provided maze.");
    }
    if((chars_in_last_row - 2) % 2 != 0){
        throw std::invalid_argument("There was an improper number of characters in each row.");
    }
    
    /* Parse the Maze into an object */
    if(!allocateMaze()){
        throw std::bad_alloc();
    }
    for(int i = -1; i < this->num_rows; i++){
        for(int j = 0; j < this->num_cols; j++){
            if(i == -1){
                if(ascii_maze[(j*2) + 1] == HORIZONTAL_WALL) addNorthernWall(0, j);
            }else{
                if(j == 0 && ascii_maze[(i + 1) * chars_in_last_row] == VERTICAL_WALL) addWesternWall(i, 0);
                if(ascii_maze[(i + 1) * chars_in_last_row + j * 2 + 2] == VERTICAL_WALL) addEasternWall(i, j);
                if(ascii_maze[(i + 1) * chars_in_last_row + j * 2 + 1] == HORIZONTAL_WALL) addSouthernWall(i, j);
            }
        }
    }
    printf("Num Rows: %d\tNum Cols: %d\n", this->num_rows, this->num_cols);
}
char* Maze::getASCIIMaze(){
    /* Allocate a buffer that is large enough to hold the ASCII Maze */
    int mem_needed = ((this->num_cols * 2) + 2) * (this->num_rows + 1) + 1;
    char* ascii_maze = (char*) calloc(mem_needed, sizeof(char));

    int index = 0; /* Index into buffer */
    
    /* Add the northern most border */
    for(int j = 0; j < this->num_cols; j++){
        ascii_maze[index++] = EMPTY_CELL;
        ascii_maze[index++] = getMazeCell(0, j)->north ? HORIZONTAL_WALL : EMPTY_CELL;
        if(j == this->num_rows - 1){
            ascii_maze[index++] = EMPTY_CELL;
            ascii_maze[index++] = ROW_DIVIDER;
        }
    }

    /* Add each row of the maze */
    for(int i = 0; i < this->num_rows; i++){
        ascii_maze[index++] = getMazeCell(i, 0)->west ? VERTICAL_WALL : EMPTY_CELL;
        for(int j = 0; j < this->num_cols; j++){
            ascii_maze[index++] = getMazeCell(i, j)->south ? HORIZONTAL_WALL : EMPTY_CELL;
            ascii_maze[index++] = getMazeCell(i, j)->east ? VERTICAL_WALL : EMPTY_CELL;
        }
        ascii_maze[index++] = ROW_DIVIDER;
    }
    ascii_maze[index] = '\0';
    return ascii_maze;
}

bool Maze::addNorthernWall(int row, int col){
    MazeCell* cell = getMazeCell(row, col);
    return setCellWalls(row, col, true, cell->east, cell->south, cell->west);
}

bool Maze::addEasternWall(int row, int col){
    MazeCell* cell = getMazeCell(row, col);
    return setCellWalls(row, col, cell->north, true, cell->south, cell->west);
}

bool Maze::addSouthernWall(int row, int col){
    MazeCell* cell = getMazeCell(row, col);
    return setCellWalls(row, col, cell->north, cell->east, true, cell->west);
}

bool Maze::addWesternWall(int row, int col){
    MazeCell* cell = getMazeCell(row, col);
    return setCellWalls(row, col, cell->north, cell->east, cell->south, true);
}

bool Maze::removeNorthernWall(int row, int col){
    MazeCell* cell = getMazeCell(row, col);
    return setCellWalls(row, col, false, cell->east, cell->south, cell->west);
}

bool Maze::removeEasternWall(int row, int col){
    MazeCell* cell = getMazeCell(row, col);
    return setCellWalls(row, col, cell->north, false, cell->south, cell->west);
}

bool Maze::removeSouthernWall(int row, int col){
    MazeCell* cell = getMazeCell(row, col);
    return setCellWalls(row, col, cell->north, cell->east, false, cell->west);
}

bool Maze::removeWesternWall(int row, int col){
    MazeCell* cell = getMazeCell(row, col);
    return setCellWalls(row, col, cell->north, cell->east, cell->south, false);
}

bool Maze::hasNorthernWall(int row, int col){
    return getMazeCell(row,col)->north;
}

bool Maze::hasEasternWall(int row, int col){
    return getMazeCell(row,col)->east;
}

bool Maze::hasSouthernWall(int row, int col){
    return getMazeCell(row,col)->south;
}

bool Maze::hasWesternWall(int row, int col){
    return getMazeCell(row,col)->west;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~ Begin Private Definitions of Maze                                                         */
/* ~~~ Specific function details are available in the header file.                               */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

bool Maze::allocateMaze(){
    this->maze = (MazeCell***) calloc(this->num_rows, sizeof(MazeCell**));
    if(this->maze == NULL) return false;
    for(int i = 0; i < this->num_rows; i++){
        this->maze[i] = (MazeCell**) calloc(this->num_cols, sizeof(MazeCell*));
        if(maze[i] == NULL) return false;
        for(int j = 0; j < this->num_cols; j++){
            this->maze[i][j] = new MazeCell();
            if(maze[i][j] == NULL) return false;
        }
    }
    return true;
}

bool Maze::setCellWalls(int row, int col, bool north, bool east, bool south, bool west){
    if(row >= this->num_rows || row < 0){
        throw std::invalid_argument("The given row does not exist within the bounds of the maze.");
    }
    if(col >= this->num_cols || col < 0){
        throw std::invalid_argument("The given column does not exist within the bounds of the maze.");
    }
    
    /* Check if any changes need to be made */
    bool ret = false;
    MazeCell* cell = getMazeCell(row, col);
    if(cell->north != north || cell->east != east || cell->south != south || cell->west != west){
        ret = true;
    }

    if(ret){
        /* Adjust the walls of these cells */
        cell->north = north;
        cell->east = east;
        cell->south = south;
        cell->west = west;

        /* Adjust the walls of adjacent cells */
        if(row > 0){
            getMazeCell(row - 1, col)->south = north;
        }
        if(col < (this->num_cols - 1)){
            getMazeCell(row, col + 1)->west = east;
        }
        if(row < (this->num_rows - 1)){
            getMazeCell(row + 1, col)->north = south;
        }
        if(col > 0){
            getMazeCell(row, col - 1)->east = west;
        }
    }
    return ret;
}

Maze::MazeCell* Maze::getMazeCell(int row, int col){
    return this->maze[row][col];
}
