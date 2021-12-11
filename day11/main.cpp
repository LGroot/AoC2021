#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <map>

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");

  constexpr int gridsize = 10;
  std::array<std::array<int, gridsize>, gridsize> grid;
  std::string line;
  size_t irow = 0;
  while (input >> line) {
    std::transform(line.begin(), line.end(), grid[irow].begin(), [](auto& c){return c-'0';}); 
    ++irow;
  }

  constexpr int steps = 100;
  constexpr int totalsize = gridsize*gridsize; 
  int flashes = 0;
  int prevflashes = 0;
  int part1 = 0;
  int part2 = 0;
  for (int i=1; part1 == 0 || part2 == 0; ++i) {
    std::for_each(grid.begin(), grid.end(), [](auto& row){
      std::transform(row.begin(), row.end(), row.begin(), [](const auto cell){
        return cell+1;
      });
    });
    bool done = false;  
    while (!done) {
      done = true;  
      for (size_t y=0; y<gridsize; ++y) {
        for (size_t x=0; x<gridsize; ++x) {
          if (grid[y][x] > 9) {
            ++flashes;
            done = false;
            grid[y][x] = 0;
            if (y>0) {
              if (grid[y-1][x]) {grid[y-1][x]++;}
              if (x>0 && grid[y-1][x-1]) {grid[y-1][x-1]++;}
              if (x<gridsize-1 && grid[y-1][x+1]) {grid[y-1][x+1]++;}
            }
            if (y<gridsize-1) {
              if (grid[y+1][x]) {grid[y+1][x]++;}
              if (x>0 && grid[y+1][x-1]) {grid[y+1][x-1]++;}
              if (x<gridsize-1 && grid[y+1][x+1]) {grid[y+1][x+1]++;}
            }
            if (x>0 && grid[y][x-1]) {grid[y][x-1]++;}
            if (x<gridsize-1 && grid[y][x+1]) {grid[y][x+1]++;}
          }
        }
      }
    }
    if (i == steps) {part1 = flashes;}
    if (prevflashes+totalsize == flashes) {part2 = i;}
    prevflashes = flashes;
  }
  

  std::cout << "Day 11 part 1: " << part1 << "\n"; 

  std::cout << "Day 11 part 2: " << part2 << "\n";
}
