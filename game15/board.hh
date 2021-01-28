#ifndef BOARD_HH
#define BOARD_HH
#include <vector>
#include <string>

typedef unsigned int grid_tile_t; // Grid tile type
using Grid = std::vector<std::vector<grid_tile_t>>;
const int SIZE = 4; // 4x4 board

const std::string DIR_DOWN    = "s";
const std::string DIR_UP      = "w";
const std::string DIR_LEFT    = "a";
const std::string DIR_RIGHT   = "d";

enum Move_return { INVALID_COMMAND=1, INVALID_TILE_NUM=2, MOVE_NOT_POSSIBLE=3, MOVE_OK=100 };

// Struct for grid tile position. Notice: upper-left is (0,0)
struct tile_pos {
  int x = -1;
  int y = -1;
};




int isGridValid(std::vector<grid_tile_t>& v);


class Board {
  public:

    // Constructor, empty game board. Use other methods to fill the way you want.
    Board();

    // Destructor
    ~Board();

    // Getter for this.grid_ --- NOT USED?
    Grid getGrid();

    // Creates SIZE*SIZE vector<...> for puzzle ints ("tiles")
    void initGrid();

    // Sets game board. Param is 1d vector of ints
    void setGrid(std::vector<grid_tile_t>& nums);

    // Shuffles and sets this.grid_ from numbers vector by using seed as a seed value
    void setGrid(std::vector<grid_tile_t>& nums, int seed);

    // No order given, shuffle 1-16 (ordered) vector with seed and set grid_ value to it
    void setGrid(int seed);

    // set this.grid_ as Grid
    void setGrid(Grid grid);

    // Changes the Grid status (order of tiles).
    // Returns false if someting went wrong (tile order does not change)
    // param validity should be checked before calling this meth.
    int move(const std::string dir, const unsigned int num);

    // Is game solvable or not?
    bool isSolvable();

    // Is game Board in solved order?
    bool isSolved();

    // Prints the game grid
    void print();

    // returns the ordinal position of from params row & col
    unsigned short getTileOrdinalPosition(const unsigned int row, const unsigned int col);

  private:


    // Returns pointer to the grid_ vector value that is in index [x,y]
    // nullptr if such position does not exists
    grid_tile_t* getGridValue(const unsigned int row, const unsigned int col);

    // receives row, col and value. Sets val to given position
    void setGridValue(const unsigned int row, const unsigned int col, unsigned int val);

    // Swaps values of two gird positions
    void swapGridTiles(const tile_pos a, const tile_pos b);

    // gets tile number (for example 15) and returns its position (tile_pos struct)
    tile_pos getTilePosition(const grid_tile_t tile);

    // Game grid for the 15 puzzle. 2d vector
    Grid grid_;

    // Checks that this.grid_ contains all nums. 1-16. If not, returns the smallest...
    int checkGridValidity();


};

#endif // BOARD_HH
