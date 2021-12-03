#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <algorithm>
#include <functional>
#include <filesystem>

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");
  std::vector<std::pair<std::string,int>> moves;
  std::string dir;
  int steps;
  while (input >> dir >> steps) {
    moves.push_back(std::make_pair(dir, steps));
  }

  const auto [x, y] = std::accumulate(moves.begin(), moves.end(), std::make_pair(0, 0), 
    [](auto && prev, const auto move){
      auto [x,y] = prev;
      const auto& [d,s] = move;
      x += (d == "forward") ? s : 0;
      y += (d == "up") ? -s : ((d == "down") ? s : 0);
      return std::make_pair(x,y);
    });
  const int part1 = x*y;
  std::cout << "Day 02 part 1: " << part1 << "\n"; 


  const auto [x2, y2, aim2] = std::accumulate(moves.begin(), moves.end(), std::make_tuple(0, 0, 0),
    [](auto && prev, const auto & move){
      auto [x,y,aim] = prev;
      const auto& [d,s] = move;
      x += (d == "forward") ? s : 0;
      y += (d == "forward") ? s*aim : 0;
      aim += (d == "up") ? -s : ((d == "down") ? s : 0);
      return std::make_tuple(x,y,aim);
    });
  const int part2 = x2*y2;
  std::cout << "Day 02 part 2: " << part2 << "\n"; 
}
