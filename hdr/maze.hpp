#ifndef MAZE_H
#define MAZE_H

#define VERTICAL_WALL '|'
#define HORIZONTAL_WALL '_'
#define EMPTY_CELL ' '
#define ROW_DIVIDER '\n'

class Maze {
    public:
        
        /**
          * This function constructs a new maze with the given number of rows and columns. Initially
          * there are no walls in the maze.
          * @param num_rows The number of rows to initialize this maze with.
          * @param num_cols The number of columns to initialize this maze with.
          * @throws invalid_argument If the given number of rows or columns does not represent a valid
          *       maze.
          * @throws bad_alloc If the memory needed by the maze could not be allocated.
          */
        Maze(int num_rows, int num_cols);

        /**
          * This function constructs a new maze from the given ASCII representation.
          * @param ascii_maze the ASCII representation of the maze to be constructed.
          * @throws invalid_argument If the given ASCII does not represent a valid maze.
          * @throws bad_alloc If the memory needed by the maze could not be allocated.
          */
        Maze(char* ascii_maze);
        
        /**
          * This function generates the ASCII representation of the maze.
          * @return A string representing the ASCII maze. 
          */
        char* getASCIIMaze();

        /**
          * These functions add a wall (North, East, South, or West) to the specified cell within the
          * maze.
          * @param row The row of the cell to be modified. 
          * @param col The column of the cell to be modified.
          * @return A boolean indicating whether a wall was added.
          */
        bool addNorthernWall(int row, int col);
        bool addEasternWall(int row, int col);
        bool addSouthernWall(int row, int col);
        bool addWesternWall(int row, int col);

        /**
          * These functions remove a wall (North, East, South, or West) from the specified cell within
          * the maze.
          * @param row The row of the cell to be modified. 
          * @param col The column of the cell to be modified.
          * @return A boolean indicating whether a wall was removed.
          */
        bool removeNorthernWall(int row, int col);
        bool removeEasternWall(int row, int col);
        bool removeSouthernWall(int row, int col);
        bool removeWesternWall(int row, int col);

        /**
          * These functions check whether the specified cell has a wall in the specified direction
          * (North, East, South, or West).
          * the maze.
          * @param row The row of the cell to be modified. 
          * @param col The column of the cell to be modified.
          * @return A boolean indicating whether the specified cell has the specified wall..
          */
        bool hasNorthernWall(int row, int col);
        bool hasEasternWall(int row, int col);
        bool hasSouthernWall(int row, int col);
        bool hasWesternWall(int row, int col);

    private:
        /* An inner class representing a single cell within the Maze. */
        class MazeCell {
            public:
                bool north; /* Does this cell have a northern wall? */
                bool east; /* Does this cell have an eastern wall? */
                bool south; /* Does this cell have a southern wall? */
                bool west; /* Does this cell have a western wall? */    
                };

        int num_rows; /* The number of rows in the Maze. */
        int num_cols; /* The number of columns in the Maze. */
        
        MazeCell*** maze; /* This field represents a matrix of pointers to MazeCell objects. */
    
        /** 
          * This function allocates the maze field of the Maze class.
          */
        bool allocateMaze();
        /**
          * This function sets the walls of the given cell within the maze to the given values.
          * @param: row The row of the cell to be modified.
          * @param: col The column of the cell to be modified.
          * @param: north Should this cell have a wall to the north?
          * @param: east Should this cell have a wall to the east?
          * @param: south Should this cell have a wall to the south?
          * @param: west Should this cell have a wall to the west?
          * @return A boolean indicating whether changes were made.
          */
        bool setCellWalls(int row, int col, bool north, bool east, bool south, bool west);
        /**
          * This function retrieves a pointer to the MazeCell at the given row and column.
          * @param row The row of the desired cell.
          * @param col The column of the desired cell.
          * @return A pointer to the desired MazeCell.
          */
        MazeCell* getMazeCell(int row, int col);
};

#endif /* MAZE_H */
