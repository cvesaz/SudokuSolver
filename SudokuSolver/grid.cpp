//
//  grid.cpp
//  SudokuSolver
//
//  Created by Christian Vessaz on 26.08.19.
//  Copyright © 2019 Christian Vessaz. All rights reserved.
//

#include "grid.hpp"

// Constructor
Grid::Grid(const FILLED_CELLS& input) {
  std::cout << "Initialization: " << std::endl;
  // Initialize empty grid
  DIGITS emptyCell = {1,2,3,4,5,6,7,8,9};
  for (INDEX i = 0; i<NN; ++i) {
    data[i] = emptyCell;
  }
  // Update input
  for (const auto& cell : input) {
    auto index = cell.first;
    auto value = cell.second;
    data[index] = {value};
  }
};

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
  check();
  std::cout << std::endl;
};

// Clean grid
bool Grid::clean() {
  std::cout << "Clean: " << std::endl;
  bool success(true);
  FILLED_CELLS cells;
  for (INDEX i = 0; i<NN; ++i) {
    if (data[i].size()==1) cells.insert({i,*data[i].begin()});
  }
  for (const auto& cell : cells) {
    success = std::min(success, clean(cell.first, cell.second));
  }
  return success;
};

// Check if Grid is valid
bool Grid::check() {
  std::cout << "Check: ";
  bool success(true);
  for (INDEX i = 0; i<NN; ++i) {
    if (data[i].size()!=1) continue;
    for (const auto& value : data[i]) {
      success = check(i,value,getLineIndices(i));
      if (success==false) break;
      success = check(i,value,getColumnIndices(i));
      if (success==false) break;
      success = check(i,value,getSquareIndices(i));
      if (success==false) break;
    }
    if (success==false) break;
  }
  std::cout << (success ? "true" : "false") << std::endl;
  assert(success);
  return success;
};

// Count remaing cell to solve
int Grid::countRemaining() {
  int count(0);
  for (INDEX i = 0; i<NN; ++i) {
    if (data[i].size()==1) ++count;
  }
  return NN-count;
};

// Solve unique value
void Grid::unique() {
  std::cout << "CheckUnique: " << std::endl;
  bool found(true);
  while (found) {
    found = false;
    for (INDEX i = 0; i<NN; ++i) {
      if (data[i].size()==1) continue;
      for (const auto& value : data[i]) {
        unique(i,value,getLineIndices(i));
        if (data[i].size()==1) {
          //std::cout << "Found unique in line: " << i << " / " << *data[i].begin() << std::endl;
          break;
        }
        unique(i,value,getColumnIndices(i));
        if (data[i].size()==1) {
          //std::cout << "Found unique in column: " << i << " / " << *data[i].begin() << std::endl;
          break;
        }
        unique(i,value,getSquareIndices(i));
        if (data[i].size()==1) {
          //std::cout << "Found unique in square: " << i << " / " << *data[i].begin() << std::endl;
          break;
        }
      }
      if (data[i].size()==1) {
        found = true;
      }
    }
  }
};

// Solve linked cells
void Grid::linkedCells() {
  std::cout << "CheckLinkedCells: " << std::endl;
  bool found(true);
  while (found) {
    found = false;
    auto nr = countRemaining();
    for (INDEX i = 0; i<NN; ++i) {
      if (data[i].size()==1) continue;
      linkedCells(i,getLineIndices(i));
      linkedCells(i,getColumnIndices(i));
      linkedCells(i,getSquareIndices(i));
    }
    if (nr-countRemaining()>0) found = true;
  }
};

// Helper to get the line indices
INDICES Grid::getLineIndices(const INDEX& index) {
  INDICES line = {0,1,2,3,4,5,6,7,8};
  int shift = index / 9;
  for (auto& ind : line) {
    ind += 9*shift;
  }
  return line;
};

// Helper to get the column indices
INDICES Grid::getColumnIndices(const INDEX& index) {
  INDICES column = {0,9,18,27,36,45,54,63,72};
  auto shift = index % 9;
  for (auto& ind : column) {
    ind += shift;
  }
  return column;
};

// Helper to get the square indices
INDICES Grid::getSquareIndices(const INDEX& index) {
  INDICES square = {0,1,2,9,10,11,18,19,20};
  auto shift = 3*((index % 9)/3) + 27*((index / 9)/3);
  for (auto& ind : square) {
    ind += shift;
  }
  return square;
};

// Clean value from indices in neighboring cell of current line, column and square
bool Grid::clean(const INDEX& index, const DIGIT& value) {
  bool success(true);
  auto lineIndices = getLineIndices(index);
  auto columnIndices = getColumnIndices(index);
  auto squareIndices = getSquareIndices(index);
  SET_INDICES indices;
  indices.insert(lineIndices.begin(), lineIndices.end());
  indices.insert(columnIndices.begin(), columnIndices.end());
  indices.insert(squareIndices.begin(), squareIndices.end());
  for (const auto& ind : indices) {
    if (ind==index) continue;
    auto it = data[ind].find(value);
    if (it != data[ind].end()) {
      if (data[ind].size()==1) {
        success = false;
      }
      else {
        data[ind].erase(it);
        if (data[ind].size()==1) {
          const DIGIT v = *data[ind].begin();
          clean(ind, v);
          //std::cout << "Last digit in cell: " << ind << " / " << v << std::endl;
        }
      }
    }
  }
  return success;
};

// Check if value is present in neighboring indices
bool Grid::check(const INDEX& index, const DIGIT& value, const INDICES& indices) {
  bool success(true);
  assert(data[index].size()==1);
  for (const auto& ind : indices) {
    if (ind==index || data[ind].size()>1) continue;
    if (*data[ind].begin()==value) {
      success = false;
      break;
    }
  }
  return success;
};

// Solve unique value in neighboring indices and clean
void Grid::unique(const INDEX& index, const DIGIT& value, const INDICES& indices) {
  assert(data[index].size()>1);
  for (const auto& ind : indices) {
    if (ind==index || data[ind].size()==1) continue;
    auto it = data[ind].find(value);
    if (it != data[ind].end()) {
      return;
    }
  }
  data[index] = {value};
  auto success = clean(index, value);
  assert(success);
};

// TODO Solve linked cells in neighboring indices and clean
void Grid::linkedCells(const INDEX& index, const INDICES& indices) {
  INDICES cells;
  cells.reserve(N);
  for (const auto& ind : indices) {
    if (ind!=index && data[ind].size()>1) cells.push_back(ind);
  }
  auto nr = cells.size();
  
  if (nr==1) {
    assert(false);
    return;
  }
  if (nr==2) {
    assert(data[cells[0]]==data[cells[1]]);
    assert(data[cells[0]].size()==2);
    return;
  }
  
  for (INDEX nl=1; nl<nr-1; ++nl) {
    // TODO
  }
};
