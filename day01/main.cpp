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

  int increasing_measurements = std::inner_product(readings.begin()+1, readings.end(), readings.begin(), 0, std::plus<>(), [](int i, int j){return i > j;});
  std::cout << "Day 01 part 1: " << increasing_measurements << "\n"; 

  std::vector<int> windowed_readings(readings.size());
  std::transform(readings.begin()+1, readings.end(), readings.begin(), windowed_readings.begin(), std::plus<>{});
  std::transform(readings.begin()+2, readings.end(), windowed_readings.begin(), windowed_readings.begin(), std::plus<>{});
  int increasing_windowed_measurements = std::inner_product(windowed_readings.begin()+1, windowed_readings.end()-2, windowed_readings.begin(), 0, std::plus<>(), [](int i, int j){return i > j;});

  //Ugly oneliner instead of generating the summed sliding window first
  //int increasing_windowed_measurements = std::inner_product(readings.begin(), readings.end()-3, readings.begin()+1, 0, std::plus<>(), [](const int& i, const int& j){return i+(&i)[1]+(&i)[2] < j+(&j)[1]+(&j)[2];});

  std::cout << "Day 01 part 2: " << increasing_windowed_measurements << "\n";
}
