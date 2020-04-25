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
typedef std::vector<DIGIT> DIGITS;
typedef std::set<DIGIT> SET_DIGITS;
typedef std::map<INDEX,DIGIT> FILLED_CELLS;

class Grid {
  
private:
  // Grid size
  enum {N = 9, NN = 81};
  // Data of cells: lift of all remaining possible digits
  SET_DIGITS data[NN];
  // Remaing cell indices
  SET_INDICES remainingCells;
  // Solved cell indices;
  SET_INDICES solvedCells;
  // Remaing line indices
  INDICES remainingLines[N];
  // Remaing column indices
  INDICES remainingColumns[N];
  // Remaing squares indices
  INDICES remainingSquares[N];
  // Is valid
  bool isValid;
  
public:
  // Constructor
  Grid(const FILLED_CELLS& input);
  // Copy constructor
  Grid(const Grid& _grid);
  // Assignement operator
  Grid& operator=(const Grid& _grid);
  
private:
  // Helper to get the line indices
  INDICES getLineIndicesFromCell(const INDEX& cell);
  INDEX   getLineIndexFromCell(const INDEX& cell);
  INDICES getLineIndicesFromLine(const INDEX& shift);
  // Helper to get the column indices
  INDICES getColumnIndicesFromCell(const INDEX& cell);
  INDEX   getColumnIndexFromCell(const INDEX& cell);
  INDICES getColumnIndicesFromColumn(const INDEX& shift);
  // Helper to get the square indices
  INDICES getSquareIndicesFromCell(const INDEX& cell);
  INDEX   getSquareIndexFromCell(const INDEX& cell);
  INDICES getSquareIndicesFromSquare(const INDEX& shift);
  // Helper to get factorial of n
  int factorial(const int& n);
  // Helper to get indices permutations
  std::vector<INDICES> getIndicesPermSizeKinN(const int& k, const int& n);
  // Clean value from indices in neighboring cell of current line, column and square
  void clean(const INDEX& index, const DIGIT& value);
  // Clean value from indices
  int clean(const INDICES& indices, const DIGIT& value);
  // Clean values from indices
  int clean(const INDICES& indices, const SET_DIGITS& values);
  // Check if value is present in neighboring indices
  bool check(const INDEX& index, const DIGIT& value, const INDICES& indices);
  // Check if value is present in neighboring indices
  bool check(const INDEX& index, const DIGIT& value);
  // Set solved cell
  void setSolvedCell(const INDEX& index, const DIGIT& value);
  // Solve unique value in neighboring indices
  bool unique(const INDEX& index, const DIGIT& value, const INDICES& indices);
  // Solve linked squares with neighboring indices
  bool linkedSquares(const INDICES& squareIndices, const INDICES& indices);
  // Solve linked cells in neighboring indices
  bool linkedCells(const INDICES& remainingIndices);
  
public:
  // Print grid to terminal
  void print();
  // Clean grid
  void clean();
  // Check if Grid is valid
  bool check();
  // Count remaing cell to solve
  int  countRemaining();
  // Solve last value remaining in cell
  bool last();
  // Solve unique value per lines, columns, squares
  bool unique();
  // Solve linked squares
  bool linkedSquares();
  // Solve linked cells per lines, columns, squares
  bool linkedCells();
  // Solve Human Style
  void solveHumanStyle();
  // Solve Brut Force
  void solveBrutForce();
};

// Grid example easy
static FILLED_CELLS easy = {
  {1, 4}, {5, 7}, {6, 1},
  {9, 5}, {10, 3}, {13, 9}, {16, 7},
  {20, 7}, {22, 6}, {24, 9}, {25, 4},
  {27, 4}, {29, 6}, {31, 8}, {33, 7}, {34, 5}, {35, 1},
  {37, 1}, {42, 6}, {43, 9},
  {46, 5}, {47, 3}, {49, 1}, {53, 2},
  {54, 9}, {55, 6}, {58, 3}, {61, 1},
  {63, 3}, {64, 7}, {67, 5}, {68, 1},
  {72, 1}, {75, 2}, {77, 9}, {78, 3}, {79, 6}, {80, 7}
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
