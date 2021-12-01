#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <algorithm>
#include <functional>

int main() {
  std::ifstream input("input");
  std::string line;
  std::vector<int> readings;
  while (std::getline(input, line)) {
    readings.push_back(std::stoi(line));
  }

  int increasing_measurements = std::inner_product(readings.begin()+1, readings.end(), readings.begin(), 0, std::plus<>(), std::greater<>());
  std::cout << "Day 01 part 1: " << increasing_measurements << "\n"; 

  // We only need to compare readings[x] with readings[x+3] because the shared readings cancel out
  int increasing_windowed_measurements = std::inner_product(readings.begin()+3, readings.end(), readings.begin(), 0, std::plus<>(), std::greater<>());

  std::cout << "Day 01 part 2: " << increasing_windowed_measurements << "\n";
}
