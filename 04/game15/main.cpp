/* 
 * Desc:
 *  This program generates a 15 puzzle. The board is SIZE x SIZE,
 *  (original was 4x4) and every round the player chooses a direction and
 *  a number tile, making the given tile to be moved to the given direction.
 *  Game will end when the numbers 1-15 (1-16) are in the ascending order,
 *  starting from top left and proceeding row by row.
 *  Empty space is represented by the number 16 in the code (constant EMPTY)
 *  and by dot when printing the game board.
 * */
#include "board.hh"
#include "helpers.hh"
#include <iostream>
#include <string>
#include <algorithm>

const int USER_GRID_INPUT_OK = 100;

/*
  Checks if given grid input is valid and can be used.
  If is:    returns USER_GRID_INPUT_OK
  If not:   returns the smallest missing int
*/
int isGridValid(std::vector<grid_tile_t>& grid) {

  grid_tile_t number = 1;

  while(number <= 16) {
    // use algoritm find() to see if number is in vector. If not, return that num
    // find() returns iterator to end if 0 occurences.
    if( (std::find(grid.begin(), grid.end(), number)) == grid.end()) {
      return number;  // this num. is missing!
    }
    number++;
  }

  // finally return int that stands for valid input
  return USER_GRID_INPUT_OK;

}




int main() {

  Board board;      // init game object

  std::string cmd = "";   // store user's input/command here
  int seed_ = 0;          // init seed value



  while(true) {
    std::cout << "Random initialization (y/n): ";
    getline(std::cin, cmd);


    if(cmd == "y" || cmd == "Y") {

      std::string seed;
      std::cout << "Give a seed value or an empty line: ";
      getline(std::cin, seed);

      // Use seed given by user, or if it was null use system clock timestamp instead
      if(seed.empty()) {
        seed_ = time(NULL) ;
      } else {
        seed_ = stoi(seed);
      }

      board.setGrid( seed_ );

      // Command was ok. get out of this loop
      break;
    }

    else if( cmd == "n" || cmd == "N" ) {

      // Init board
      std::vector<unsigned int> nums;
      std::string line = "";  // store user's input here

      // Ask the order
      std::cout << "Enter the numbers 1-16 in a desired order (16 means empty):" << std::endl;
      getline(std::cin, line);

      // === Take user input from the string ===
      size_t pos = 0;
      std::string token = "";
      const std::string del = " ";

      // remove leading whitespaces. Continue when .at(0) is not delim.
      while(line.compare(0, 1, del)==0) {
        line.erase(line.begin());
      }

      // Iterate string with .find(). When found a match, add it to the vector and substring it out
      while ((pos = line.find(del)) != std::string::npos) {
        token = line.substr(0, pos);
        line.erase(0, pos + del.length());
        nums.push_back( stoi(token) );
      }
      // Take rest of the string (the last one)
      if( pos == std::string::npos && !line.empty() ) {
        nums.push_back( stoi(line) );
      }
      // == *** ==



      int missing = isGridValid(nums); // see if all nums. are given
      if( missing != USER_GRID_INPUT_OK ) {
        std::cout << "Number " << missing << " is missing" << std::endl;
        return EXIT_FAILURE;
      }

      // Finally, set grid as user wanted it
      board.setGrid(nums);

      // Command was ok. get out of this loop
      break;
    }

    // We did not exit the loop. Inform user.
    std::cout << "Unknown choice: " << cmd << std::endl;


  } // while




  // After board init. tell if game is solvable or not
  if(board.isSolvable()) {
    std::cout << "Game is solvable: Go ahead!" << std::endl;
    board.print();
  } else {
    std::cout << "Game is not solvable. What a pity.";
    return EXIT_SUCCESS;
  }



  // === Start the actual game ===

  while(! board.isSolved()) {   

    std::string user_input = "";
    std::string tile = "";
    int tile_ = 0;

    // Ask & read inputs to variables
    std::cout << "Dir (command, number): ";
    getline(std::cin, user_input);

    // Parse command format: "C[C]_N[N]"
    cmd   = user_input.substr(0, user_input.find(" "));

    // Check: If it was quit command, stop immediately
    if(cmd == "q" || cmd == "Q") { return EXIT_SUCCESS; }

    // ...continue reading
    tile  = user_input.substr(user_input.find(" ")+1);
    tile_ = stoi( tile ); // skip the space bar

    // is command valid if not, output reason. If valid, change game status

    int return_code = board.move(cmd, tile_);

    if(return_code == INVALID_COMMAND) {
      std::cout << "Unknown command: " << cmd << std::endl;
    }
    else if(return_code == INVALID_TILE_NUM) {
      std::cout << "Invalid number: " << tile_ << std::endl;
    }
    else if(return_code == MOVE_NOT_POSSIBLE) {
      std::cout << "Impossible direction: " << cmd << std::endl;
    } // else: move was ok. nothing special.

    // Print game status after command. Then go back to start...
    board.print();

  }

  // Game solved!
  std::cout << "You won!";
  return EXIT_SUCCESS;
}
