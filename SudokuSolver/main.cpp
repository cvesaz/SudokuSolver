//
//  main.cpp
//  SudokuSolver
//
//  Created by Christian Vessaz on 26.08.19.
//  Copyright © 2019 Christian Vessaz. All rights reserved.
//

#if 1 // Test Grid class

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

#else // REMOVE ME Test combination

#include <iostream>
#include <vector>

using namespace std;

vector<int> people;
vector<int> combination;

void pretty_print(const vector<int>& v, const int o, const int k) {
  static int count = 0;
  if (k==0) ++count;
  cout << "[" << o << "/" << k << "] combination no " << count << ": [ ";
  for (int i = 0; i < v.size(); ++i) { cout << v[i] << " "; }
  cout << "] " << endl;
}

void go(int offset, int k) {
  pretty_print(combination, offset, k);
  if (k == 0) {
    return;
  }
  for (int i = offset; i <= people.size() - k; ++i) {
    combination.push_back(people[i]);
    go(i+1, k-1);
    combination.pop_back();
  }
}

int main() {
  int n = 8, k = 5;
  
  for (int i = 0; i < n; ++i) { people.push_back(i+1); }
  go(0, k);
  
  return 0;
}

#endif
