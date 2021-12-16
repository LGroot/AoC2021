#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <queue>

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");
  
  std::vector<std::vector<int>> grid;
  std::string line;
  while (input >> line) {
    std::vector<int> row(line.size());
    std::transform(line.begin(), line.end(), row.begin(), [](auto c){return c-'0';});
    grid.push_back(std::move(row));
  }
  
  auto search_path = [](auto grid) {
    const int maxcol = grid[0].size()-1;
    const int maxrow = grid.size()-1;
    struct path {
      int risk, x, y;
    };
    auto cmp = [](auto& l, auto& r){return l.risk > r.risk;};
    std::priority_queue<path, std::vector<path>, decltype(cmp)> paths(cmp);
    paths.push({0, 0, 0});
    while (true) {
      auto p = paths.top();
      paths.pop();
      if (p.x == maxcol && p.y == maxrow) {return p.risk;}
      if (p.x > 0 && grid[p.y][p.x-1]) {paths.push({p.risk+grid[p.y][p.x-1], p.x-1, p.y}); grid[p.y][p.x-1] = 0;}
      if (p.y > 0 && grid[p.y-1][p.x]) {paths.push({p.risk+grid[p.y-1][p.x], p.x, p.y-1}); grid[p.y-1][p.x] = 0;}
      if (p.x < maxcol && grid[p.y][p.x+1]) {paths.push({p.risk+grid[p.y][p.x+1], p.x+1, p.y}); grid[p.y][p.x+1] = 0;}
      if (p.y < maxrow && grid[p.y+1][p.x]) {paths.push({p.risk+grid[p.y+1][p.x], p.x, p.y+1}); grid[p.y+1][p.x] = 0;}
    }
  };

  const auto part1 = search_path(grid);
  std::cout << "Day 15 part 1: " << part1 << "\n"; 

  constexpr int scale = 5;
  std::vector<std::vector<int>> grid2(grid.size()*scale);
  for (int i=0; i<scale; ++i) {
    for (size_t irow=0; irow<grid.size(); ++irow) {
      auto& row = grid2[i*grid.size()+irow];
      auto& orig_row = grid[irow];
      row.reserve(scale*orig_row.size());
      for (int j=0; j<scale; ++j) {
        for (auto risk: orig_row) {
          row.push_back((risk+i+j-1)%9 + 1);
        }
      }
    }
  } 

  const auto part2 = search_path(grid2);
  std::cout << "Day 15 part 2: " << part2 << "\n"; 
}
