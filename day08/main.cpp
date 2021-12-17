#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");

  std::vector<std::pair<std::array<std::string,10>, std::array<std::string,4>>> displays;
  std::array<std::string,10> uniques;
  std::array<std::string,4> digits;
  while (input.peek() != EOF) {
    for (int i=0; i<10; ++i) {
      input >> uniques[i];
    }
    input >> digits[0];
    for (int i=0; i<4; ++i) {
      input >> digits[i]; 
    }
    displays.push_back({uniques, digits});
  }

  const auto part1 = std::accumulate(displays.begin(), displays.end(), 0, 
    [](auto acc, auto&& display) {
      auto [u,d] = display; 
      return acc + std::accumulate(d.begin(), d.end(), 0, 
        [](auto acc, auto digit){
          return acc + (digit.size() == 2 || digit.size() == 3 || digit.size() == 4 || digit.size() == 7);
        });
    }); 

  std::cout << "Day 08 part 1: " << part1 << "\n"; 

  const auto part2 = std::accumulate(displays.begin(), displays.end(), 0, 
    [](auto acc, auto&& display) {
      auto [u,d] = display; 
      auto digit1 = *std::find_if(u.begin(), u.end(), [](auto digit){return digit.size() == 2;});
      auto digit4 = *std::find_if(u.begin(), u.end(), [](auto digit){return digit.size() == 4;});
      return acc + std::accumulate(d.begin(), d.end(), 0, 
        [digit1, digit4](auto acc, auto digit){
          int val = 0;
          if (digit.size() == 2) {
            val = 1;
          } else if (digit.size() == 3) {
            val = 7;
          } else if (digit.size() == 4) {
            val = 4;
          } else if (digit.size() == 7) {
            val = 8;
          } else {
            auto count1 = std::count_if(digit.begin(), digit.end(), [digit1](auto c){return std::any_of(digit1.begin(), digit1.end(), [c](auto c1) {return c == c1;});});
            auto count4 = std::count_if(digit.begin(), digit.end(), [digit4](auto c){return std::any_of(digit4.begin(), digit4.end(), [c](auto c4) {return c == c4;});});
            if (digit.size() == 5) {
              if (count1 == 2) { 
                val = 3;
              } else if (count4 == 2) {
                val = 2;
              } else if (count4 == 3) {
                val = 5;
              }
            } else if (digit.size() == 6) {
              if (count1 == 1) {
                val = 6;
              } else if (count4 == 3) {
                val = 0;
              } else if (count4 == 4) {
                val = 9;
              }
            }
          }
          return acc*10 + val;
        });
    }); 

  std::cout << "Day 08 part 2: " << part2 << "\n";
}
