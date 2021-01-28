/*  Puzzle 15
 *
 * Class: Board
 *
 * */

#include "board.hh"
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>

const int     EMPTY = 16;
const char    EMPTY_CHAR = '.';
const unsigned int PRINT_WIDTH = 5;


// Consturcor
Board::Board() {
  this->initGrid();
}


// Destructor
Board::~Board() { }


// Init Game board 4x4
void Board::initGrid() {
  this->grid_ = {
    std::vector<grid_tile_t>(4),
    std::vector<grid_tile_t>(4),
    std::vector<grid_tile_t>(4),
    std::vector<grid_tile_t>(4)
  };
}


/*
  Sets game status/tile order as defined in param nums. No randomization.

  For example vector: (1,2,3,4 ... 16) will be:
    [01][02][03][04]
    [05][06][07][08]
    [09][10][12][12]
    [13][14][15][16]
*/
void Board::setGrid(std::vector<grid_tile_t>& nums) {

  for(std::vector<grid_tile_t>::size_type idx=0; idx<nums.size(); ++idx) {
    // Convert i to row/y and col/x values. For example 14 -> [x=1, y=3]
    grid_tile_t row = idx / SIZE;   // for example idx=4 is on row 1
    grid_tile_t col = idx % SIZE;   // .. idx 14 is on col 2
    this->setGridValue(row, col, nums.at(idx));
  }

}


/*
  Sets game grid status, but first randomize it
*/
void Board::setGrid(std::vector<grid_tile_t>& nums, int seed) {

    std::default_random_engine randomEng(seed);
    std::uniform_int_distribution<int> distr(0, nums.size() - 1);

    for(unsigned int i = 0; i < nums.size(); ++i){
        unsigned int random_index = distr(randomEng);
        unsigned int temp = nums.at(i);
        nums.at(i) = nums.at(random_index);
        nums.at(random_index) = temp;
    }

    // Then set the actual grid
    this->setGrid(nums);
}



/*
  When setGrid is called with only seed value.
  Calls method setGrid(&vector, seed) that does the actual shffling.
  Default order 1-16 is given as param.
*/
void Board::setGrid(int seed) {
  std::vector<grid_tile_t> defaultOrder{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
  this->setGrid(defaultOrder, seed);
}


/*
  Takes type Grid as param and sets game grid
*/
void Board::setGrid(Grid grid) {
  this->grid_ = grid;
}


/*
  Getter for this.grid_
*/
Grid Board::getGrid() {
  return this->grid_;
}


/*
  set grid tile's value (num).
  Params row, col and desired value.
  Params assumed to be valid.
*/
void Board::setGridValue(const unsigned int row, const unsigned int col, grid_tile_t val) {
  this->grid_.at(row).at(col) = val;
}


/*
  Receives row and col ints (y & x) and returns pointer to that int
  Params row, col and desired value.
  Params assumed to be valid.
*/
grid_tile_t* Board::getGridValue(const unsigned int row, const unsigned int col) {
  return &(this->grid_.at(row).at(col));
}


/*
  Returns boolean true if grid status is in state that is solvable,
  false otherwise

  (*) Make local copy of the board
  (1) Move EMPTY to bottom row (to row index SIZE-1)
  (2) Calculate inversion for each non-empty tile ([1- SIZE^2-1]).
      Inversion is the number of tiles with smaller number, that are after tile T.
      For example 5,1,3,2,4 -> inv(5) = 4. Order is LtR from top to bottom.
  (3) Sum all invs., if sum odd, puzzle is not solvable
  (4) Return result

  -- TODO: Get a copy of the original grid_ ?
*/
bool Board::isSolvable() {

  bool debug = false;

  // We will make local copy of the board & its grid_
  Board b;
  b.setGrid( this->grid_ );
  //b.print();

  tile_pos empty_pos = b.getTilePosition(EMPTY);

  // (1) - Move
  while(empty_pos.y != SIZE-1) {

    // Get tile NUM below EMPTY tile
    grid_tile_t* tile_below_num = b.getGridValue(empty_pos.y+1, empty_pos.x);
    // Move NUM down 1 step
    b.move(DIR_UP, *tile_below_num);

    // get new empty pos
    empty_pos = b.getTilePosition(EMPTY);

  }

  // (2) - Inversion
  unsigned short inv_total = 0; // total sum

  // from [1. SIZE^2-1 (!!!)]
  for(unsigned int tile_num=1; tile_num<SIZE*SIZE; tile_num++) {

    tile_pos tile_pos_      = b.getTilePosition(tile_num);
    // Get tiles ordinal position (1. - 16.)
    const unsigned int tiles_ord    = b.getTileOrdinalPosition(tile_pos_.y, tile_pos_.x);
    unsigned int tile_inv   = 0;

    if(debug) {
      std::cout << "tiles(" << tile_num << ") ordinal pos. is: " << tiles_ord << "/16 \n"; }

    // Loop through game board starting from ()
    // and caluclate tile's inversion
    for(unsigned int row=0; row<SIZE; row++) {
      for(unsigned int col=0; col<SIZE; col++) {

        // Get <y,x>'s ordinal position. Skip if LTE than cur. tile.
        unsigned int lk_tile_ord  = b.getTileOrdinalPosition(row, col);


        if( lk_tile_ord <= tiles_ord ) {
          if(debug) {
            std::cout << "-- skip this tile!\n"; }
          continue;
        }


        // Get lookup tile's value
        grid_tile_t* lk_tile_val = b.getGridValue(row, col);

        // Compare tiles values
        // If smaller, we have an inversion. Raise inv.sum. by 1.
        if( (*lk_tile_val) < tile_num) {
          if(debug) {
            std::cout << "--> " << *lk_tile_val << " < " << tile_num << "*  "; }
          tile_inv++;
        }
      }
    }

    // Before changing tile...
    inv_total += tile_inv;
    if(debug) {
      std::cout << "Tile's inv(" << tile_num << ") was " << tile_inv << "---\n" << std::endl; }

  }


  // All tiles checked
  if(debug) {
    std::cout << inv_total << " = tot. inv\n"; }

  // odd (false) or even (true)
  return (inv_total % 2 == 0);
}


/*
  Returns boolean true if grid is in solved state,
  the grid.at(0) contains vector that contains values 1,2,3,4
  .at(1) .. 5,6,7,8 ...

  return false immediately even if one tile is not positioned correctly.

  Code assumes that grid pos (x=0, y=0) is at top-left.
*/
bool Board::isSolved() {

  grid_tile_t val;

  for(int row=0; row<SIZE; ++row) {
    for(int col=0; col<SIZE; ++col) {
       val = *this->getGridValue(row, col);
       if(val != static_cast<unsigned int>((row*SIZE) + col+1) ) {
          return false;
       }
    }
  }
  return true;

}



/*

  IF move command is valid, changes game staus.
  If not, returns int error code:
    1 = invalid dir
    2 = invalid num
    3 = other unkwnow error

    100 = move ok

  (validity check could be its own method... but this will do it for now)

*/
int Board::move(const std::string dir, const grid_tile_t num) {

  // First check. Is command ok?
  if(dir != "a" && dir != "w" && dir != "d" && dir != "s")  { return INVALID_COMMAND; }
  // Second check. Is tile number ok?
  if(num < 1 || num > (SIZE*SIZE)-1) { return INVALID_TILE_NUM; }

  // Get positions of of the wanted tiles.
  tile_pos empty_pos    = this->getTilePosition(EMPTY);   // retruns x/y
  tile_pos num_pos      = this->getTilePosition(num);     // retruns x/y

  // Third check. Is move allowed? Move is valid if
  //  a -> tile is on right side of the empty [.][n] (aka move tile to left),
  //  and so on...

  if(
    (dir == DIR_LEFT  && num_pos.x == empty_pos.x+1) ||
    (dir == DIR_RIGHT && num_pos.x == empty_pos.x-1) ||
    (dir == DIR_UP    && num_pos.y == empty_pos.y+1) ||
    (dir == DIR_DOWN  && num_pos.y == empty_pos.y-1)
  ) {
    // Is valid. Do the value swap
    this->swapGridTiles( empty_pos, num_pos );
    return MOVE_OK;
  }
  return MOVE_NOT_POSSIBLE;

}


/*
  Receives two position structs (tile_pos). Swaps their places (values)
  - even if it would be illegal in real 15-puzzle (no special checks are made).
  No swap is made if x/y values are invalid (outside the puzzle bounds [0, size-1]).
*/
void Board::swapGridTiles(const tile_pos a, const tile_pos b) {

  if(
    a.x < 0 || a.x > SIZE-1 || b.x < 0 || b.x > SIZE-1 ||
    a.y < 0 || a.y > SIZE-1 || b.y < 0 || b.y > SIZE-1
  ) { return; }

  // Do the value swap
  grid_tile_t a_val_temp = *(this->getGridValue(a.y, a.x));

  this->setGridValue(a.y, a.x, *(this->getGridValue(b.y, b.x) ));   // a <-- b
  this->setGridValue(b.y, b.x, a_val_temp);                         // b <-- a (temp)

}





/*
  Method that gets tile position in the grid. Returned value is struct tile_pos
  with variables x and y. (where y row and x column).
  If not found, struct with def. values will be returned
*/
tile_pos Board::getTilePosition(const grid_tile_t tile) {

  struct tile_pos pos;

  // Find the tile and return its position as tile_pos struct
  for(unsigned int row = 0; row < SIZE; ++row) {  // row/y
    for(unsigned int col = 0; col < SIZE; ++col) {  // col/x

      // If tile was found
      if(this->grid_.at(row).at(col) == tile) {
        pos.x = col;
        pos.y = row;
        return pos;
      }
    }
  }
  return pos;

}



/*
  Returs the ordinal position of the (row, col).
  returned value is between [1-SIZE^2]
*/
unsigned short Board::getTileOrdinalPosition(const unsigned int row, const unsigned int col) {
  return (row*SIZE) + (col+1);
}


/*
  Prints the game board
*/
void Board::print() {

  for(unsigned int y = 0; y < SIZE; ++y) {
    std::cout << std::string(PRINT_WIDTH * SIZE + 1, '-') << std::endl;

    for(unsigned int x = 0; x < SIZE; ++x) {
        std::cout << "|" << std::setw(PRINT_WIDTH - 1);
        if(grid_.at(y).at(x) != EMPTY) {
            std::cout << grid_.at(y).at(x);
        }
        else {
            std::cout << EMPTY_CHAR;
        }
    }
    std::cout << "|" << std::endl;
  }
  std::cout << std::string(PRINT_WIDTH * SIZE + 1, '-') << std::endl;
}



