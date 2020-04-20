//
//  grid.cpp
//  SudokuSolver
//
//  Created by Christian Vessaz on 26.08.19.
//  Copyright © 2019 Christian Vessaz. All rights reserved.
//

#include "grid.hpp"

// Constructor
// input: list of filled cells in the grid
Grid::Grid(const FILLED_CELLS& input) {
  std::cout << "Initialization: " << std::endl;
  // Initialize empty grid
  SET_DIGITS emptyCell = {1,2,3,4,5,6,7,8,9};
  for (INDEX i = 0; i<NN; ++i) {
    data[i] = emptyCell;
    remainingCells.insert(i);
  }
  // Update input
  for (const auto& cell : input) {
    auto index = cell.first;
    auto value = cell.second;
    data[index] = {value};
    auto it = remainingCells.find(index);
    if (it!=remainingCells.end()) {
      remainingCells.erase(it);
      solvedCells.insert(index);
    }
  }
  // Check data
  clean();
  isValid = check();
  if (isValid) print();
}

// Helper to get the line indices
// index: Current cell index
// return: List of all cell indices on the current line
INDICES Grid::getLineIndices(const INDEX& index) {
  INDICES line = {0,1,2,3,4,5,6,7,8};
  int shift = index / 9;
  for (auto& ind : line) {
    ind += 9*shift;
  }
  return line;
}

// Helper to get the column indices
// index: Current cell index
// return: List of all cell indices on the current column
INDICES Grid::getColumnIndices(const INDEX& index) {
  INDICES column = {0,9,18,27,36,45,54,63,72};
  auto shift = index % 9;
  for (auto& ind : column) {
    ind += shift;
  }
  return column;
}

// Helper to get the square indices
// index: Current cell index
// return: List of all cell indices on the current square
INDICES Grid::getSquareIndices(const INDEX& index) {
  INDICES square = {0,1,2,9,10,11,18,19,20};
  auto shift = 3*((index % 9)/3) + 27*((index / 9)/3);
  for (auto& ind : square) {
    ind += shift;
  }
  return square;
}

// Helper to get factorial of n
// n: input number
// return: n!
int factorial(const int& n) {
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

// Helper to get indices permutations
// k: size of echantillon
// n: size of population
// return: list of all k indices permutations out of n indices
std::vector<std::vector<int>> getIndicesPermSizeKinN(const int& k, const int& n) {
  std::vector<std::vector<int>> permList;
  if (k>n || k<1) return permList;
  int permCount(0);
  permList.resize(factorial(n)/factorial(n-k));
  std::vector<int> perm(k);
  for (int i = 0; i < k; ++i) perm[i] = i;
  permList[permCount] = perm;
  ++permCount;
  while (perm[0]<n-k) {
    for (int i = perm[k-1]; i < n-1; ++i) {
      perm[k-1] += 1;
      permList[permCount] = perm;
      ++permCount;
    }
    for (int i = 1; i <= k; ++i) {
      if (perm[k-i]<(n-i)) {
        perm[k-i] += 1;
        for (int j = k-i+1; j<k; ++j) {
          perm[j] = perm[j-1]+1;
        }
        permList[permCount] = perm;
        ++permCount;
        break;
      }
    }
  }
  return permList;
}

// Clean value from indices in neighboring cell of current line, column and square
// index: Current cell index
// value: Digit to be removed from cells data
void Grid::clean(const INDEX& index, const DIGIT& value) {
  auto lineIndices = getLineIndices(index);
  auto columnIndices = getColumnIndices(index);
  auto squareIndices = getSquareIndices(index);
  SET_INDICES indices;
  indices.insert(lineIndices.begin(), lineIndices.end());
  indices.insert(columnIndices.begin(), columnIndices.end());
  indices.insert(squareIndices.begin(), squareIndices.end());
  for (const auto& ind : indices) {
    if (ind==index || data[ind].size()==1) continue;
    auto it = data[ind].find(value);
    if (it != data[ind].end()) {
      data[ind].erase(it);
    }
  }
}

// Check if value is present in neighboring indices
// index: Current cell index
// value: Digit to check
// indices: Indices to be checked, including current cell
// return: Boolean success
bool Grid::check(const INDEX& index, const DIGIT& value, const INDICES& indices) {
  for (const auto& ind : indices) {
    if (ind==index || data[ind].size()>1) continue;
    if (*data[ind].begin()==value) {
      return false;
    }
  }
  return true;
}

// Check if value is present in neighboring indices
// index: Current cell index
// value: Digit to check
// return: Boolean success
bool Grid::check(const INDEX& index, const DIGIT& value) {
  if (!check(index,value,getLineIndices(index))) return false;
  if (!check(index,value,getColumnIndices(index))) return false;
  if (!check(index,value,getSquareIndices(index))) return false;
  return true;
}

// Set solved cell
// index: Current cell index
// value: Digit to assign
void Grid::setSolvedCell(const INDEX& index, const DIGIT& value) {
  data[index] = {value};
  auto it = remainingCells.find(index);
  if (it!=remainingCells.end()) {
    remainingCells.erase(it);
    solvedCells.insert(index);
  }
  else {
    assert(false);
  }
  clean(index, value);
}

// Solve unique value in neighboring indices
// index: Current cell index
// value: Digit to check if unique in neighboring indices, if so clean()
// indices: Indices of neighboring cell, including current cell
void Grid::unique(const INDEX& index, const DIGIT& value, const INDICES& indices) {
  assert(data[index].size()>1);
  for (const auto& ind : indices) {
    if (ind==index || data[ind].size()==1) continue;
    auto it = data[ind].find(value);
    if (it != data[ind].end()) {
      return;
    }
  }
  setSolvedCell(index, value);
}

// TODO Solve linked cells in neighboring indices
// index: Current cell index
// indices: Indices of neighboring cell, including current cell
void Grid::linkedCells(const INDEX& index, const INDICES& indices) {
  
}

// Print grid to terminal
void Grid::print() {
  for (INDEX i = 0; i<NN; ++i) {
    if (i%9 == 0 && i!=0) std::cout << std::endl;
    if (data[i].size()==1) {
      std::cout << *data[i].begin();
    }
    else {
      std::cout << " ";
    }
    std::cout << " ";
    
  }
  std::cout << std::endl << "Remaining cells: " << countRemaining() << std::endl;
}

// Clean grid
void Grid::clean() {
  for (const auto& cell : solvedCells) {
    clean(cell, *data[cell].begin());
  }
}

// Check if Grid is valid
// return: Boolean success
bool Grid::check() {
  std::cout << "Check: ";
  bool success(true);
  for (const auto& cell : solvedCells) {
    assert(data[cell].size()==1);
    auto value = *data[cell].begin();
    success = check(cell,value);
    if (success==false) break;
  }
  std::cout << (success ? "true" : "false") << std::endl;
  return success;
}

// Count remaing cell to solve
// return: Number of remaining cells
int Grid::countRemaining() {
  return (int)remainingCells.size();
}

// Solve last value remaining in cell
void Grid::last() {
  for (const auto& cell : remainingCells) {
    if (data[cell].size()==1) {
      setSolvedCell(cell, *data[cell].begin());
      return;
    }
  }
}

// Solve unique value per lines, columns, squares
void Grid::unique() {
  for (const auto& cell : remainingCells) {
    for (const auto& value : data[cell]) {
      unique(cell,value,getLineIndices(cell));
      if (data[cell].size()==1) return;
      unique(cell,value,getColumnIndices(cell));
      if (data[cell].size()==1) return;
      unique(cell,value,getSquareIndices(cell));
      if (data[cell].size()==1) return;
    }
  }
}

// Solve linked cells per lines, columns, squares
void Grid::linkedCells() {
  
}

// Solve Human Style
void Grid::solveHumanStyle() {
  if (!isValid) return;
  while (countRemaining()>0) {
    auto nr = countRemaining();
    last();
    if (nr-countRemaining()>0) continue;
    unique();
    if (nr-countRemaining()>0) continue;
    linkedCells();
    break;
  }
}

// Solve Brut Force
void Grid::solveBrutForce() {
  if (!isValid) return;
  for (INDEX cell = 0; cell<NN; ++cell) {
    if (data[cell].size()>1) {
      for (DIGIT value = 1; value<=9; ++value) {
        if (check(cell, value)) {
          auto cellData = data[cell];
          data[cell] = {value};
          solveBrutForce();
          if (remainingCells.size()==0) return;
          data[cell] = cellData;
        }
      }
      return;
    }
  }
  remainingCells.clear(); // FIXME force to stop after founding the first solution
}
