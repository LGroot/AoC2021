#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>


int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");

  std::vector<std::string> grid;
  std::string line;
  while (input >> line) {
    grid.push_back(std::move(line));
  }

  const int rows = grid.size();
  const int cols = grid[0].size();
  
  int steps = 0;
  int changed = 1;
  auto newgrid = grid; 
  while (changed) {
    changed = 0;
    for (int y=0; y<rows; ++y) {
      for (int x=0; x<cols; ++x) {
        if (grid[y][x] == '>') {
          int nextx = (x+1)%cols;
          if (grid[y][nextx] == '.') {
            ++changed;
            newgrid[y][x] = '.';
            newgrid[y][nextx] = '>';
          }
        } 
      }
    }
    grid = newgrid;
    for (int y=0; y<rows; ++y) {
      for (int x=0; x<cols; ++x) {
        if (grid[y][x] == 'v') {
          int nexty = (y+1)%rows;
          if (grid[nexty][x] == '.') {
            ++changed;
            newgrid[y][x] = '.';
            newgrid[nexty][x] = 'v';
          }
        } 
      }
    }
    ++steps;
    grid = newgrid;
  }

  std::cout << "Day 25 part 1: " << steps << "\n"; 
}

