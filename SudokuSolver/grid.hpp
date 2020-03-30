//
//  grid.hpp
//  SudokuSolver
//
//  Created by Christian Vessaz on 26.08.19.
//  Copyright © 2019 Christian Vessaz. All rights reserved.
//

#ifndef grid_hpp
#define grid_hpp


#include <stdio.h>
#include <iostream>

#include <vector>
#include <set>
#include <map>

typedef int INDEX;
typedef std::vector<INDEX> INDICES;
typedef std::set<INDEX> SET_INDICES;
typedef int DIGIT;
typedef std::set<DIGIT> DIGITS;
typedef std::map<INDEX,DIGIT> FILLED_CELLS;

class Grid {
  
public:
  // Grid size
  enum {N = 9, NN = 81};
  
public:
  // Constructor
  Grid(const FILLED_CELLS& input);
  // Print grid to terminal
  void print();
  // Clean grid
  bool clean();
  // Check if Grid is valid
  bool check();
  // Count remaing cell to solve
  int  countRemaining();
  // Solve unique value per lines, columns, squares
  void unique();
  // Solve linked cells per lines, columns, squares
  void linkedCells();
  
private:
  // Helper to get the line indices
  INDICES getLineIndices(const INDEX& index);
  // Helper to get the column indices
  INDICES getColumnIndices(const INDEX& index);
  // Helper to get the square indices
  INDICES getSquareIndices(const INDEX& index);
  // Clean value from indices in neighboring cell of current line, column and square
  bool clean(const INDEX& index, const DIGIT& value);
  // Check if value is present in neighboring indices
  bool check(const INDEX& index, const DIGIT& value, const INDICES& indices);
  // Solve unique value in neighboring indices
  void unique(const INDEX& index, const DIGIT& value, const INDICES& indices);
  // Solve linked cells in neighboring indices
  void linkedCells(const INDEX& index, const INDICES& indices);
  
public:
  // Data of cells: lift of all remaining possible digits
  DIGITS data[NN];
};

// Grid example easy
static FILLED_CELLS easy = {
  {1, 6}, {2, 8}, {4, 2}, {5, 7}, {6, 9}, {8, 1},
  {12, 9}, {13, 1}, {17, 8},
  {18, 1}, {20, 7}, {23, 3}, {25, 4},
  {27, 4}, {28, 7}, {30, 2}, {32, 6},
  {37, 5}, {38, 1}, {39, 3}, {40, 4}, {41, 9}, {42, 7},
  {46, 2}, {58, 8}, {49, 7}, {51, 3}, {52, 5},
  {55, 1}, {56, 9}, {58, 6},
  {64, 3}, {66, 5}, {71, 6},
  {72, 6}, {74, 5}, {78, 4}, {80, 7}
};

// Grid example medium
static FILLED_CELLS medium = {
  {6, 4}, {7, 7},
  {9, 5}, {12, 4}, {17, 2},
  {19, 6}, {25, 3}, {26, 8},
  {29, 7}, {32, 5}, {34, 1},
  {38, 5}, {40, 4},
  {47, 3}, {49, 9}, {50, 1}, {51, 2}, {53, 4},
  {54, 2}, {55, 5}, {57, 3}, {59, 4}, {62, 1},
  {64, 4}, {65, 9}, {69, 7},
  {72, 8}, {73, 7}, {76, 2}, {80, 5}
};

// Grid example hard
static FILLED_CELLS hard = {
  {1, 4}, {2, 9}, {8, 5},
  {12, 8}, {13, 3}, {14, 1},
  {26, 8},
  {31, 9}, {33, 8}, {34, 1},
  {41, 7}, {42, 6},
  {47, 1}, {51, 3},
  {54, 7}, {55, 8}, {57, 4}, {58, 1}, {62, 9},
  {63, 2}, {64, 9}, {65, 4}, {66, 7},
  {72, 6}, {75, 9}
};

// Grid example expert
static FILLED_CELLS expert = {
  {2, 6}, {4, 4}, {7, 3},
  {10, 3}, {15, 6}, {17, 9},
  {21, 1}, {22, 3}, {24, 8},
  {28, 2}, {34, 7},
  {39, 2}, {40, 9}, {41, 4}, {44, 5},
  {46, 1}, {51, 9},
  {57, 3}, {58, 2}, {59, 8},
  {64, 6},
  {72, 4}, {79, 5}
};

#endif /* grid_hpp */
