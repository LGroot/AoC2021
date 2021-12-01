#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <algorithm>

int main() {
  std::ifstream input("input");
  std::string line;
  std::vector<int> readings;
  while (std::getline(input, line)) {
    readings.push_back(std::stoi(line));
  }


  std::vector<int> differences(readings.size());
  std::adjacent_difference(readings.begin(), readings.end(), differences.begin()); 
  int increasing_measurements = std::count_if(differences.begin()+1, differences.end(), [](int i){return i > 0;});
  std::cout << "Day 01 part 1: " << increasing_measurements << "\n"; 


  std::vector<int> windowed_readings(readings.size());
  std::transform(readings.begin()+1, readings.end(), readings.begin(), windowed_readings.begin(), std::plus<>{});
  std::transform(readings.begin()+2, readings.end(), windowed_readings.begin(), windowed_readings.begin(), std::plus<>{});
  windowed_readings.erase(windowed_readings.end()-2, windowed_readings.end());
  std::vector<int> windowed_differences(windowed_readings.size());
  std::adjacent_difference(windowed_readings.begin(), windowed_readings.end(), windowed_differences.begin()); 
  int increasing_windowed_measurements = std::count_if(windowed_differences.begin()+1, windowed_differences.end(), [](int i){return i > 0;});
  std::cout << "Day 01 part 2: " << increasing_windowed_measurements << "\n";
}
