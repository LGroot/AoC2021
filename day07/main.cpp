#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");

  std::vector<int> positions;
  std::string n;
  while (std::getline(input, n, ',')) {
    positions.push_back(std::stoi(n));
  }
  std::sort(positions.begin(), positions.end());

  const int median = positions[positions.size()/2];
  const auto part1 = std::accumulate(positions.begin(), positions.end(), 0, 
    [median](const auto acc, const auto pos){
      return acc + std::abs(pos-median);
    }
  );
        
  std::cout << "Day 07 part 1: " << part1 << "\n"; 

  const int average = std::accumulate(positions.begin(), positions.end(), 0, std::plus<>()) / positions.size();
  const auto part2 = std::accumulate(positions.begin(), positions.end(), 0, 
    [average](const auto acc, const auto pos){
      auto dist = std::abs(pos-average);
      return acc + dist*(dist+1)/2;
    }
  );

  std::cout << "Day 07 part 2: " << part2 << "\n";
}
