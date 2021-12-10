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

  std::string line;
  std::vector<std::string> lines;
  while (input >> line) {
    lines.push_back(line); 
  }
  

  std::map<char,char> pairs{{')','('}, {']','['}, {'}','{'}, {'>','<'}};
  std::map<char,int> part1_points{{')',3}, {']',57}, {'}',1197}, {'>',25137}};
  std::map<char,int> part2_points{{'(',1}, {'[',2}, {'{',3}, {'<',4}};
  std::vector<char> tokens;
  std::vector<long> scores;
  int part1 = 0;
  for (const auto& line: lines) {
    for (auto c: line) {
      if (c == '(' || c == '[' || c == '{' || c == '<') {
        tokens.push_back(c); 
      } else {
        if (tokens.empty() || tokens.back() != pairs[c]) {
          part1 += part1_points[c];
          tokens.clear();
          break;
        }
        tokens.pop_back();
      }
    } 
    if (!tokens.empty()) {
      long score = 0;
      while (!tokens.empty()) {
        score *= 5;
        score += part2_points[tokens.back()];
        tokens.pop_back(); 
      }
      scores.push_back(score);
    }
  }

  std::cout << "Day 10 part 1: " << part1 << "\n"; 

  std::sort(scores.begin(), scores.end());
  const long part2 = scores[scores.size()/2];
  std::cout << "Day 10 part 2: " << part2 << "\n";
}
