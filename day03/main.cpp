#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <algorithm>
#include <functional>
#include <filesystem>

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");
  std::string n;
  std::vector<std::string> numbers;
  while (input >> n) {
    numbers.push_back(n);
  }

  const auto numlen = numbers.front().size();
  auto bitcount = std::accumulate(numbers.begin(), numbers.end(), std::vector<int>(numlen, 0), 
    [](auto&& v, const auto & s){
      std::transform(v.begin(), v.end(), s.begin(), v.begin(), [](auto i, auto c) {return i+(c=='1');});
      return v;
    });
  const int half = numbers.size()/2;
  const auto gamma = std::accumulate(bitcount.begin(), bitcount.end(), 0, 
    [half](auto acc, auto count){
      return (acc<<1) + (count>half);
    });
  const auto epsilon = (~gamma)&((1<<numlen)-1);
  const auto part1 = gamma * epsilon;

  std::cout << "Day 03 part 1: " << part1 << "\n"; 

  auto common = [](auto gas, std::array<char,2> bits) {
    for (int i=0; gas.size()>1; ++i) {
      unsigned int count = std::count_if(gas.begin(), gas.end(), [i](auto&& number){ return number[i] == '1'; });
      char filter = count*2 >= gas.size() ? bits[0] : bits[1];
      gas.erase(std::remove_if(gas.begin(), gas.end(), [i,filter](auto&& number){ return number[i] != filter; }), gas.end());
    }
    return std::stoi(gas.front(), 0, 2);
  };
  const auto O2 = common(numbers, {'1', '0'});
  const auto CO2 = common(numbers, {'0', '1'});
  const auto part2 = O2 * CO2;

  std::cout << "Day 03 part 2: " << part2 << "\n"; 
}
