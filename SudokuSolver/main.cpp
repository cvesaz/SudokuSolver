//
//  main.cpp
//  SudokuSolver
//
//  Created by Christian Vessaz on 26.08.19.
//  Copyright © 2019 Christian Vessaz. All rights reserved.
//

#include <iostream>
#include "grid.hpp"

int main(int argc, const char * argv[]) {
  
  auto grid = Grid(expert); // easy, medium, hard, expert
  grid.print();
  
  grid.clean();
  grid.print();
  
  grid.unique();
  grid.print();
  
  grid.linkedCells();
  grid.print();
  
  // TODO add recursive method when multiple option is required
  
  return 0;
}
