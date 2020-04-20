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
  
  auto level = expert; // easy, medium, hard, expert
  
  auto grid = Grid(level);
  auto start = std::chrono::high_resolution_clock::now();
  grid.solveBrutForce();
  auto stop = std::chrono::high_resolution_clock::now();
  auto solveTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  
  std::cout << "Solution Brut Force: " << std::endl;
  grid.check();
  grid.print();
  std::cout << "Solve time Brut Force: " << (float)solveTime.count()/1e6 << " [seconds]" << std::endl << std::endl;
  
  grid = Grid(level);
  start = std::chrono::high_resolution_clock::now();
  grid.solveHumanStyle();
  stop = std::chrono::high_resolution_clock::now();
  solveTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  
  std::cout << "Solution Human Style: " << std::endl;
  grid.check();
  grid.print();
  std::cout << "Solve time Human Style: " << (float)solveTime.count()/1e6 << " [seconds]" << std::endl << std::endl;
  
  return 0;
}
