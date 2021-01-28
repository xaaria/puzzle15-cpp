/*
  File containing some helpers
*/

#include <iostream>
#include <vector>

void printVector(std::vector<unsigned int>& vec) {

  for(auto el: vec) {
    std::cout << el << ", ";
  }
  std::cout << "\n";


}
