//
//  grid.cpp
//  SudokuSolver
//
//  Created by Christian Vessaz on 26.08.19.
//  Copyright © 2019 Christian Vessaz. All rights reserved.
//

#include "grid.hpp"
#include <algorithm>

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

// Copy constructor
// _grid: input grid to copy
Grid::Grid(const Grid& _grid) {
  *this = _grid;
}

// Assignement operator
// _grid: input grid to copy
Grid& Grid::operator=(const Grid& _grid) {
  for (INDEX i = 0; i<NN; ++i) {
    data[i] = _grid.data[i];
  }
  remainingCells = _grid.remainingCells;
  solvedCells = _grid.solvedCells;
  isValid = _grid.isValid;
  return *this;
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
int Grid::factorial(const int& n) {
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

// Helper to get indices permutations
// k: size of echantillon
// n: size of population
// return: list of all k indices permutations out of n indices
std::vector<INDICES> Grid::getIndicesPermSizeKinN(const int& k, const int& n) {
  std::vector<INDICES> permList;
  if (k>n || k<1) return permList;
  int permCount(0);
  permList.resize(factorial(n)/(factorial(k)*factorial(n-k)));
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

// Clean values from indices
// indices: cell indices
// value: Digit to be removed from cells data
// return: Number of removed digits
int Grid::clean(const INDICES& indices, const DIGIT& value) {
  int count(0);
  for (const auto& ind : indices) {
    auto it = data[ind].find(value);
    if (it != data[ind].end()) {
      data[ind].erase(it);
      ++count;
    }
  }
  return count;
}

// Clean values from indices
// indices: cell indices
// values: Digits to be removed from cells data
// return: Number of removed digits
int Grid::clean(const INDICES& indices, const SET_DIGITS& values) {
  int count(0);
  for (const auto& ind : indices) {
    for (const auto& value : values) {
      auto it = data[ind].find(value);
      if (it != data[ind].end()) {
        data[ind].erase(it);
        ++count;
      }
    }
  }
  return count;
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
  isValid = std::min(isValid, check(index, value));
}

// Solve unique value in neighboring indices
// index: Current cell index
// value: Digit to check if unique in neighboring indices, if so clean()
// indices: Indices of neighboring cell, including current cell
// return: Found a unique value for cell at index
bool Grid::unique(const INDEX& index, const DIGIT& value, const INDICES& indices) {
  assert(data[index].size()>1);
  for (const auto& ind : indices) {
    if (ind==index || data[ind].size()==1) continue;
    auto it = data[ind].find(value);
    if (it != data[ind].end()) {
      return false;
    }
  }
  setSolvedCell(index, value);
  return true;
}

// Solve linked square with neighboring indices
// squareIndices: Indices of current square
// indices: Indices of neighboring line or column
// return: Found a linked cells that need to be cleaned
bool Grid::linkedSquares(const INDICES& squareIndices, const INDICES& indices) {
  INDICES remainingSquareIndices;
  remainingSquareIndices.reserve(N);
  INDICES remainingOverlapIndices;
  remainingOverlapIndices.reserve(N);
  INDICES remainingIndices;
  remainingIndices.reserve(N);
  for (const auto& i : squareIndices) {
    if (data[i].size()==1) continue;
    if (std::find(indices.begin(), indices.end(),i)!=indices.end()) {
      remainingOverlapIndices.push_back(i);
    }
    else {
      remainingSquareIndices.push_back(i);
    }
  }
  for (const auto& i : indices) {
    if (data[i].size()==1) continue;
    if (std::find(remainingOverlapIndices.begin(), remainingOverlapIndices.end(),i)==remainingOverlapIndices.end()) {
      remainingIndices.push_back(i);
    }
  }
  SET_DIGITS remainingSquareValues;
  for (const auto& i : remainingSquareIndices) {
    for (const auto& v : data[i]) {
      remainingSquareValues.insert(v);
    }
  }
  SET_DIGITS remainingOverlapValues;
  for (const auto& i : remainingOverlapIndices) {
    for (const auto& v : data[i]) {
      remainingOverlapValues.insert(v);
    }
  }
  SET_DIGITS remainingValues;
  for (const auto& i : remainingIndices) {
    for (const auto& v : data[i]) {
      remainingValues.insert(v);
    }
  }
  for (const auto& v : remainingOverlapValues) {
    if (remainingSquareValues.count(v) && !remainingValues.count(v)) {
      clean(remainingSquareIndices, v);
      return true;
    }
    if (!remainingSquareValues.count(v) && remainingValues.count(v)) {
      clean(remainingIndices, v);
      return true;
    }
  }
  return false;
}

// Solve linked cells in neighboring indices
// indices: Indices of neighboring cell, including current cell
// return: Found a linked cells that need to be cleaned
bool Grid::linkedCells(const INDICES& indices) {
  INDICES remainingIndices;
  remainingIndices.reserve(N);
  for (INDEX i = 0; i < N; ++i) {
    auto cell = indices[i];
    if (data[cell].size()>1) remainingIndices.push_back(cell);
  }
  auto nr = (int)remainingIndices.size();
  auto checkLinkedPerm = [&] (INDICES cellPerm, INDICES cellPermCompl) -> bool {
    SET_DIGITS valuesInCellPerm;
    for (const auto& i : cellPerm) {
      for (const auto& v : data[i]) {
        valuesInCellPerm.insert(v);
      }
    }
    if (cellPerm.size()==valuesInCellPerm.size()) {
      if (clean(cellPermCompl, valuesInCellPerm)>0) {
        return true;
      }
    }
    return false;
  };
  for (INDEX k = 2; k <= ((nr+1)/2); ++k) {
    auto perms = getIndicesPermSizeKinN(k, nr);
    for (auto perm : perms) {
      INDICES cellPerm;
      cellPerm.reserve(perm.size());
      for (const auto& i : perm) {
        cellPerm.push_back(remainingIndices[i]);
      }
      INDICES cellPermCompl;
      cellPermCompl.reserve(nr-cellPerm.size());
      for (const auto& ri : remainingIndices) {
        if (std::count(cellPerm.begin(), cellPerm.end(), ri)==0) cellPermCompl.push_back(ri);
      }
      if (checkLinkedPerm(cellPerm, cellPermCompl)) return true;
      if (checkLinkedPerm(cellPermCompl, cellPerm)) return true;
    }
  }
  return false;
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
// return: Found a last value for cell
bool Grid::last() {
  for (const auto cell : remainingCells) {
    if (data[cell].size()==1) {
      setSolvedCell(cell, *data[cell].begin());
      return true;
    }
  }
  return false;
}

// Solve unique value per lines, columns, squares
// return: Found a unique value for cell at index
bool Grid::unique() {
  for (const auto cell : remainingCells) {
    for (const auto value : data[cell]) {
      if (unique(cell,value,getLineIndices(cell))) return true;
      if (unique(cell,value,getColumnIndices(cell))) return true;
      if (unique(cell,value,getSquareIndices(cell))) return true;
    }
  }
  return false;
}

// Solve linked squares
// return: Found a linked square that need to be cleaned
// TODO optimize me using loop over lines, columns and cells instead of remainingCells
bool Grid::linkedSquares() {
  for (const auto cell : remainingCells) {
    if (linkedSquares(getSquareIndices(cell),getLineIndices(cell))) return true;
    if (linkedSquares(getSquareIndices(cell),getColumnIndices(cell))) return true;
  }
  return false;
}

// Solve linked cells per lines, columns, squares
// return: Found a linked cells that need to be cleaned
// TODO optimize me using loop over lines, columns and cells instead of remainingCells
bool Grid::linkedCells() {
  for (const auto cell : remainingCells) {
    if (linkedCells(getLineIndices(cell))) return true;
    if (linkedCells(getColumnIndices(cell))) return true;
    if (linkedCells(getSquareIndices(cell))) return true;
  }
  return false;
}

// Solve Human Style
void Grid::solveHumanStyle() {
  if (!isValid) return;
  // REMARK force to stop after founding the first solution
  while (countRemaining()>0) {
    // Solvers which set a single cell
    if (last()) continue;
    if (!isValid) return;
    if (unique()) continue;
    if (!isValid) return;
    // Solvers which clean some cells data
    if (linkedSquares()) continue;
    if (linkedCells()) continue;
    // Recursion
    auto initialGrid = Grid(*this);
    for (const auto& cell : initialGrid.remainingCells) {
      auto initialCellData = data[cell];
      for (const auto& value : initialCellData) {
        setSolvedCell(cell, value);
        solveHumanStyle();
        // REMARK force to stop after founding the first solution
        if (remainingCells.size()==0) return;
        *this = initialGrid;
      }
    }
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
          // REMARK force to stop after founding the first solution
          if (remainingCells.size()==0) return;
          data[cell] = cellData;
        }
      }
      return;
    }
  }
  // REMARK force to stop after founding the first solution
  remainingCells.clear();
}
