#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");

  struct point {
    int x, y;
    bool operator< (const point& o) const {
      if (x < o.x) {return true;}
      if (x > o.x) {return false;}
      return y < o.y;
    }
  };

  struct line {
    point p1, p2;
  };
 
  std::vector<line> lines; 
  std::string p1, p2, ignore;
  while (input >> p1 >> ignore >> p2) {
    size_t pos1 = p1.find(",");
    size_t pos2 = p2.find(",");
    lines.push_back({{std::stoi(p1.substr(0,pos1)), std::stoi(p1.substr(pos1+1))}, {std::stoi(p2.substr(0,pos2)), std::stoi(p2.substr(pos2+1))}}); 
  }

  std::map<point,int> grid;
  for (auto line: lines) {
    if (line.p1.x != line.p2.x && line.p1.y != line.p2.y) {continue;}
    grid[line.p1]++;
    const int dx = line.p1.x < line.p2.x ? 1 : -1;
    while (line.p1.x != line.p2.x) {
      line.p1.x += dx;
      grid[line.p1]++;
    }
    const int dy = line.p1.y < line.p2.y ? 1 : -1;
    while (line.p1.y != line.p2.y) {
      line.p1.y += dy;
      grid[line.p1]++;
    }
  }
  const int part1 = std::accumulate(grid.begin(), grid.end(), 0, [](auto acc, auto&& keyval){auto [k, v] = keyval; return acc+(v>1);});

  std::cout << "Day 05 part 1: " << part1 << "\n"; 

  for (auto line: lines) {
    if (line.p1.x == line.p2.x || line.p1.y == line.p2.y) {continue;}
    grid[line.p1]++;
    const int dx = line.p1.x < line.p2.x ? 1 : -1;
    const int dy = line.p1.y < line.p2.y ? 1 : -1;
    while (line.p1.x != line.p2.x) {
      line.p1.x += dx;
      line.p1.y += dy;
      grid[line.p1]++;
    }
  }
  const int part2 = std::accumulate(grid.begin(), grid.end(), 0, [](auto acc, auto&& keyval){auto [k, v] = keyval; return acc+(v>1);});

  std::cout << "Day 05 part 2: " << part2 << "\n";
}
