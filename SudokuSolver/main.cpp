//
//  main.cpp
//  SudokuSolver
//
//  Created by Christian Vessaz on 26.08.19.
//  Copyright © 2019 Christian Vessaz. All rights reserved.
//

#include <iostream>
#include <chrono>
#include "grid.hpp"

int main(int argc, const char * argv[]) {
  
  auto grid = Grid(expert); // easy, medium, hard, expert
  
  auto start = std::chrono::high_resolution_clock::now();
  grid.solveBrutForce();
  auto stop = std::chrono::high_resolution_clock::now();
  auto solveTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  
  std::cout << "Solution: " << std::endl;
  grid.check();
  grid.print();
  std::cout << "Solve time: " << (float)solveTime.count()/1e6 << " [seconds]" << std::endl;
  
  return 0;
}
