#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <set>

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");
  
  std::set<std::pair<int,int>> grid;
  std::string line;
  while (std::getline(input, line) && line != "") {
    auto pos = line.find(",");
    auto x = std::stoi(line.substr(0, pos));
    auto y = std::stoi(line.substr(pos+1));
    grid.emplace(std::make_pair(x,y));
  }

  std::vector<std::pair<char,int>> folds;
  while (std::getline(input, line)) {
    auto pos = line.find("=");
    auto axis = line[pos-1];
    auto val = std::stoi(line.substr(pos+1));
    folds.push_back(std::make_pair(axis,val));
  }

  int part1 = 0;
  int minx = 0;
  int miny = 0;
  for (auto&& fold: folds) {
    auto [axis, val] = fold; 
    if (axis == 'x') {
      if (val < minx || !minx) {minx = val;}
    } else {
      if (val < miny || !miny) {miny = val;}
    }
    std::set<std::pair<int,int>> newgrid; 
    std::transform(grid.begin(), grid.end(), std::inserter(newgrid, newgrid.begin()), 
      [axis,val](auto&& point){
        auto [x,y] = point;
        if (axis == 'x' && x > val) {
          return std::make_pair(2*val-x, y);
        } else if (axis == 'y' && y > val) {
          return std::make_pair(x, 2*val-y);
        } else {
          return std::make_pair(x, y);
        }
      });
    grid.swap(newgrid);
    if (part1 == 0) {part1 = grid.size();}
  } 
  std::cout << "Day 13 part 1: " << part1 << "\n"; 

  std::cout << "Day 13 part 2:\n";
  for (int y=0; y<miny; ++y) {
    for (int x=0; x<minx; ++x) {
      if (grid.contains(std::make_pair(x,y))) {
        std::cout << "#";
      } else {
        std::cout << " ";
      }
    }
    std::cout << "\n";
  }
}
