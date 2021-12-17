#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");

  std::vector<std::vector<int>> grid;
  std::string line;
  while (std::getline(input, line)) {
    std::vector<int> row(line.size());
    std::transform(line.begin(), line.end(), row.begin(), [](auto c){return c-'0';});
    grid.push_back(row);
  }

  int part1 = 0;
  const size_t rows = grid.size();
  const size_t cols = grid[0].size();
  for (size_t y = 0; y < rows; ++y) {
    for (size_t x = 0; x < cols; ++x) {
      auto val = grid[y][x];
      if ((x==0 || val < grid[y][x-1]) &&
          (x==cols-1 || val < grid[y][x+1]) &&
          (y==0 || val < grid[y-1][x]) &&
          (y==rows-1 || val < grid[y+1][x])) {
          part1 += val+1;
      } 
    }
  }

  std::cout << "Day 09 part 1: " << part1 << "\n"; 

  int max1 = 0, max2 = 0, max3 = 0;
  for (size_t y = 0; y < rows; ++y) {
    for (size_t x = 0; x < cols; ++x) {
      if (grid[y][x] == 9) {continue;}
      int basinsize = 0;
      std::vector<std::pair<size_t,size_t>> tocheck{{y,x}};
      std::vector<std::pair<size_t,size_t>> newcheck;
      while (!tocheck.empty()) {
        for (auto&& check: tocheck) {
          auto [yy, xx] = check;
          if (grid[yy][xx] == 9) {continue;}
          grid[yy][xx] = 9;
          basinsize++; 
          if (xx > 0) {newcheck.push_back({yy,xx-1});}
          if (xx < cols-1) {newcheck.push_back({yy,xx+1});}
          if (yy > 0) {newcheck.push_back({yy-1,xx});}
          if (yy < rows-1) {newcheck.push_back({yy+1,xx});}
        }
        tocheck.swap(newcheck);
        newcheck.clear();
      }
      if (basinsize > max1) {
        max3 = max2; max2 = max1; max1 = basinsize;
      } else if (basinsize > max2) {
        max3 = max2; max2 = basinsize;
      } else if (basinsize > max3) {
        max3 = basinsize; 
      }
    }
  }

  const int part2 = max1 * max2 * max3;
  std::cout << "Day 09 part 2: " << part2 << "\n";
}
