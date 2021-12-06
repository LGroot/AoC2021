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
  std::array<long,9> ages;
  ages.fill(0);

  std::string n;
  while (std::getline(input, n, ',')) {
    ages[std::stoi(n)]++;
  }
        
  auto breed = [&ages](auto days) {
    for (int i=0; i<days; ++i) {
      ages[7] += ages[0];
      std::rotate(ages.begin(), ages.begin()+1, ages.end());
    }
  };

  constexpr int days = 80;
  breed(days);
  const long part1 = std::accumulate(ages.begin(), ages.end(), 0L, std::plus<>());

  std::cout << "Day 06 part 1: " << part1 << "\n"; 

  constexpr int days2 = 256 - days;
  breed(days2);
  const long part2 = std::accumulate(ages.begin(), ages.end(), 0L, std::plus<>());

  std::cout << "Day 06 part 2: " << part2 << "\n";
}
