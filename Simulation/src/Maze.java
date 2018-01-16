/**
 * This class represents a micro mouse maze.
 * @author Jeffrey Martin
 * Created on January 16th, 2018
 */
public class Maze {
    /**
     * This class represents a single cell in the Maze. It can have walls at any of the four cardinal directions
     * (North, East, South, West).
     */
    private class MazeCell {
        /* Booleans indicating whether the maze cell has walls at each of the cardinal directions */
        private boolean north;
        private boolean east;
        private boolean south;
        private boolean west;

        /**
         * Default Constructor: Initializes the MazeCell to not have any adjacent walls.
         */
        private MazeCell() {
            this.north = false;
            this.east = false;
            this.south = false;
            this.west = false;
        }
    }

    /* The default dimension (number of rows & number of columns) in the maze */
    public static final int DEFAULT_DIMENSION = 16;

    /* Characters to be used when generating an ASCII representation of the maze */
    public static final char HORIZONTAL_WALL = '_';
    public static final char VERTICAL_WALL = '|';
    public static final char EMPTY_WALL = ' ';
    public static final char NEWLINE = '\n';

    private int row_count;
    private int column_count;
    private MazeCell[][] maze;
    private boolean finalized;

    /**
     * Default Constructor: Initializes a new maze with {@value #DEFAULT_DIMENSION} rows and {@value #DEFAULT_DIMENSION}
     * columns. The initialized maze will have walls along all borders, and no where else.
     */
    public Maze(){
        this.row_count = DEFAULT_DIMENSION;
        this.column_count = DEFAULT_DIMENSION;
        this.finalized = false;
        initializeMaze();
    }

    /**
     * Custom Constructor: Initializes a new maze with the given number of rows and columns. The initialized maze will
     * have walls along all borders, and no where else.
     * @param row_count The number of rows in the maze.
     * @param column_count The number of columns in the maze.
     * @throws IllegalArgumentException If the given number of rows or columns is invalid (less than 1).
     */
    public Maze(int row_count, int column_count) throws IllegalArgumentException{
        if(row_count < 1) throw new IllegalArgumentException("The number of rows can not be less than 1.");
        if(column_count < 1) throw new IllegalArgumentException("The number of columns can not be less than 1.");
        this.row_count = row_count;
        this.column_count = column_count;
        this.finalized = false;
        initializeMaze();
    }

    /**
     * Custom Constructor: Initializes a new maze with the given ASCII representation of the maze.
     * @param maze The ASCII representation of the maze to be created.
     * @throws IllegalArgumentException If the given ASCII maze does not represent a valid maze.
     */
    public Maze(String maze) throws IllegalArgumentException{
        if(maze == null) throw new IllegalArgumentException("The given ASCII maze is null.");
        /* Trim leading or trailing whitespace from the maze */
        maze = maze.trim();
        if(maze.equals("")) throw new IllegalArgumentException("The given ASCII maze is empty.");
        /* Split the maze into its rows */
        String[] rows = maze.split("\n");
        /* Trim any leading or trailing whitespace */
        for(int i = 0; i < rows.length; i++){
            rows[i] = rows[i].trim();
            if(i == 0) rows[i] = " " + rows[i]; /* Account for space in top border row */
        }
        /* Determine number of rows and columns */
        this.row_count = rows.length - 1;
        this.column_count = (rows[0].length() + 1) / 2;
        /* Ensure that all of the rows have the same number of columns */
        for(int i = 1; i <= row_count; i++){
            if(rows[i].length()/2 != column_count) throw new IllegalArgumentException(String.format(
                    "Row %d of the given ASCII maze has a different number of columns than the others.", i));
        }
        finalized = false;
        /* Initialize the maze */
        initializeMaze();
        /* Parse the northern border to ensure its valid */
        for(int j = 0; j < column_count; j++){
            if(!(rows[0].charAt((j*2)+1) == HORIZONTAL_WALL)) throw new IllegalArgumentException(String.format(
                    "The given ASCII maze has an invalid northern border at row %d, column %d.",0,j));
        }
        /* Parse each row ensuring borders are valid and adding walls as needed */
        for(int i = 0; i < this.row_count; i++){
            if(!(rows[i+1].charAt(0) == VERTICAL_WALL)) throw new IllegalArgumentException(String.format(
                    "The given ASCII maze has an invalid western border at row %d, column %d.",i,0));
            for(int j = 0; j < this.column_count; j++){
                if(j == this.column_count - 1 && !(rows[i+1].charAt((j+1)*2) == VERTICAL_WALL))
                    throw new IllegalArgumentException(String.format("The given ASCII maze has an invalid eastern" +
                            "border at row %d, column %d.", i, j));
                if(i == this.row_count - 1 && !(rows[i+1].charAt((j*2)+1) == HORIZONTAL_WALL))
                    throw new IllegalArgumentException(String.format("The given ASCII maze has an invalid southern" +
                            "border at row %d, column %d.",i,j));
                if(rows[i+1].charAt((j+1)*2) == VERTICAL_WALL) addEasternWallToCell(i,j);
                if(rows[i+1].charAt((j*2)+1) == HORIZONTAL_WALL) addSouthernWallToCell(i,j);
            }
        }
    }

    /**
     * This function initializes the maze such that there are walls along all maze borders.
     */
    private void initializeMaze(){
        this.maze = new MazeCell[this.row_count][this.column_count];
        for(int i = 0; i < this.row_count; i++){
            for(int j = 0; j < this.column_count; j++){
                MazeCell cell = new MazeCell();
                /* Add walls to the cell if it is on the border of the maze */
                if(i == 0){cell.north = true;}
                if(j == this.column_count- 1){cell.east = true;}
                if(i == this.row_count - 1){cell.south = true;}
                if(j == 0){cell.west = true;}
                /* Add cell to the maze */
                this.maze[i][j] = cell;
            }
        }
    }

    /**
     * This function generates and returns an ASCII representation of the maze.
     * @return A string representing the maze in ASCII characters.
     */
    public String toString(){
        StringBuilder builder = new StringBuilder();
        /* Add the northern border */
        for(int j = 0; j < this.column_count; j++){
            builder.append(this.EMPTY_WALL);
            if(this.maze[0][j].north){builder.append(this.HORIZONTAL_WALL);}
            else{builder.append(this.EMPTY_WALL);}
        }
        builder.append(NEWLINE);
        /* Add each row in the maze */
        for(int i = 0; i < this.row_count; i++){
            if(this.maze[i][0].west)builder.append(this.VERTICAL_WALL);
            else builder.append(this.EMPTY_WALL);
            for(int j = 0; j < this.column_count; j++){
                if(this.maze[i][j].south){builder.append(this.HORIZONTAL_WALL);}
                else{builder.append(this.EMPTY_WALL);}
                if(this.maze[i][j].east){builder.append(this.VERTICAL_WALL);}
                else{builder.append(this.EMPTY_WALL);}
            }
            if(i != this.row_count - 1){builder.append(NEWLINE);}
        }
        return builder.toString();
    }

    /**
     * This function sets the walls of the cell at the given row and column to the given values. It will update adjacent
     * cells as necessary.
     * @param row The row of the cell to be modified.
     * @param column The column of the cell to be modified.
     * @param north A boolean indicating whether this cell should have a northern wall.
     * @param east A boolean indicating whether this cell should have a eastern wall.
     * @param south A boolean indicating whether this cell should have a southern wall.
     * @param west A boolean indicating whether this cell should have a western wall.
     * @return A boolean indicating whether the changes could be made without violating the external maze border. If the
     * return value is false, no changes were made to the maze.
     * @throws IllegalArgumentException if the given row or column does not exist within the bounds of the maze.
     */
    public boolean setCellWalls(int row, int column, boolean north, boolean east, boolean south, boolean west)
            throws IllegalArgumentException{
        if(row < 0 || row >= this.row_count)
            throw new IllegalArgumentException("The given row does not exist within the bounds of the maze");
        if(column < 0 || column >= this.column_count)
            throw new IllegalArgumentException("The given column does not exist within the bounds of the maze");
        /* Check that the maze has not been finalized */
        if(finalized) return false;
        /* Check that all the changes can be made without removing the border. */
        if(row == 0 && !north) return false; /* Would remove the northern border. */
        if(column == this.column_count -1 && !east) return false; /* Would remove the eastern border. */
        if(row == this.row_count - 1 && !south) return false; /* Would remove the southern border. */
        if(column == 0 && !west) return false; /* Would remove the western border. */
        /* Make changes to this cell (and any adjacent cells) */
        this.maze[row][column].north = north;
        if(row != 0)this.maze[row-1][column].south = north;
        this.maze[row][column].east = east;
        if(column != column_count - 1) this.maze[row][column+1].west = east;
        this.maze[row][column].south = south;
        if(row != row_count - 1) this.maze[row+1][column].north = south;
        this.maze[row][column].west = west;
        if(column != 0)this.maze[row][column-1].east = west;
        return true;
    }

    /**
     * This function adds a northern wall to the given cell.
     * @param row The row of the cell to be modified.
     * @param column The column of the cell to be modified.
     * @return A boolean indicating whether the changes could be made without violating the external maze border. If the
     * return value is false, no changes were made to the maze.
     * @throws IllegalArgumentException if the given row or column does not exist within the bounds of the maze.
     */
    public boolean addNorthernWallToCell(int row, int column) throws IllegalArgumentException{
        return setCellWalls(row, column, true, this.maze[row][column].east,
                this.maze[row][column].south, this.maze[row][column].west);
    }

    /**
     * This function adds a eastern wall to the given cell.
     * @param row The row of the cell to be modified.
     * @param column The column of the cell to be modified.
     * @return A boolean indicating whether the changes could be made without violating the external maze border. If the
     * return value is false, no changes were made to the maze.
     * @throws IllegalArgumentException if the given row or column does not exist within the bounds of the maze.
     */
    public boolean addEasternWallToCell(int row, int column) throws IllegalArgumentException{
        return setCellWalls(row, column, this.maze[row][column].north, true,
                this.maze[row][column].south, this.maze[row][column].west);
    }

    /**
     * This function adds a southern wall to the given cell.
     * @param row The row of the cell to be modified.
     * @param column The column of the cell to be modified.
     * @return A boolean indicating whether the changes could be made without violating the external maze border. If the
     * return value is false, no changes were made to the maze.
     * @throws IllegalArgumentException if the given row or column does not exist within the bounds of the maze.
     */
    public boolean addSouthernWallToCell(int row, int column) throws IllegalArgumentException{
        return setCellWalls(row, column, this.maze[row][column].north, this.maze[row][column].east,
                true, this.maze[row][column].west);
    }

    /**
     * This function adds a western wall to the given cell.
     * @param row The row of the cell to be modified.
     * @param column The column of the cell to be modified.
     * @return A boolean indicating whether the changes could be made without violating the external maze border. If the
     * return value is false, no changes were made to the maze.
     * @throws IllegalArgumentException if the given row or column does not exist within the bounds of the maze.
     */
    public boolean addWesternWallToCell(int row, int column) throws IllegalArgumentException{
        return setCellWalls(row, column, this.maze[row][column].north, this.maze[row][column].east,
                this.maze[row][column].south, true);
    }

    /**
     * This function removes the northern wall from the given cell.
     * @param row The row of the cell to be modified.
     * @param column The column of the cell to be modified.
     * @return A boolean indicating whether the changes could be made without violating the external maze border. If the
     * return value is false, no changes were made to the maze.
     * @throws IllegalArgumentException if the given row or column does not exist within the bounds of the maze.
     */
    public boolean removeNorthernWallFromCell(int row, int column) throws IllegalArgumentException{
        return setCellWalls(row, column, false, this.maze[row][column].east,
                this.maze[row][column].south, this.maze[row][column].west);
    }

    /**
     * This function removes the eastern wall from the given cell.
     * @param row The row of the cell to be modified.
     * @param column The column of the cell to be modified.
     * @return A boolean indicating whether the changes could be made without violating the external maze border. If the
     * return value is false, no changes were made to the maze.
     * @throws IllegalArgumentException if the given row or column does not exist within the bounds of the maze.
     */
    public boolean removeEasternWallFromCell(int row, int column) throws IllegalArgumentException{
        return setCellWalls(row, column, this.maze[row][column].north, false,
                this.maze[row][column].south, this.maze[row][column].west);
    }

    /**
     * This function removes southern wall from the given cell.
     * @param row The row of the cell to be modified.
     * @param column The column of the cell to be modified.
     * @return A boolean indicating whether the changes could be made without violating the external maze border. If the
     * return value is false, no changes were made to the maze.
     * @throws IllegalArgumentException if the given row or column does not exist within the bounds of the maze.
     */
    public boolean removeSouthernWallFromCell(int row, int column) throws IllegalArgumentException{
        return setCellWalls(row, column, this.maze[row][column].north, this.maze[row][column].east,
                false, this.maze[row][column].west);
    }

    /**
     * This function removes the western wall from the given cell.
     * @param row The row of the cell to be modified.
     * @param column The column of the cell to be modified.
     * @return A boolean indicating whether the changes could be made without violating the external maze border. If the
     * return value is false, no changes were made to the maze.
     * @throws IllegalArgumentException if the given row or column does not exist within the bounds of the maze.
     */
    public boolean removeWesternWallFromCell(int row, int column) throws IllegalArgumentException{
        return setCellWalls(row, column, this.maze[row][column].north, this.maze[row][column].east,
                this.maze[row][column].south, false);
    }

    /**
     * This function finalizes the maze, preventing further additions or removal of walls.
     * @return A boolean indicating whether the maze was finalized before this function was called.
     */
    public boolean finalizeMaze(){
        boolean old = this.finalized;
        this.finalized = true;
        return old;
    }
}
