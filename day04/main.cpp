#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <valarray>

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");
  std::string numbers_string;
  input >> numbers_string;
  std::vector<int> numbers;
  for (size_t pos; (pos = numbers_string.find(",")) != std::string::npos;) {
      numbers.push_back(std::stoi(numbers_string.substr(0, pos)));
      numbers_string.erase(0, pos+1);
  }

  std::vector<std::valarray<int>> cards;
  constexpr int side = 5;
  constexpr int numbers_per_card = side*side;
  std::valarray<int> card(numbers_per_card);
  for (int i=0, n=0; input >> n; ++i) {
    card[i%numbers_per_card] = n; 
    if (i%numbers_per_card == numbers_per_card-1) {
      cards.push_back(card);
    }
  }

  std::vector<std::vector<int>> finished;
  std::vector<int> scores;
  for (auto number: numbers) {
    cards.erase(std::remove_if(cards.begin(), cards.end(), [&](auto& card){
      std::replace(std::begin(card), std::end(card), number, -1);
      for (int row_col=0; row_col<side; ++row_col) {
        const auto row_score = std::valarray(card[std::slice(row_col*side, side, 1)]).sum();
        const auto col_score = std::valarray(card[std::slice(row_col, side, side)]).sum();
        if (row_score == -side || col_score == -side) {
          scores.push_back(number * std::accumulate(std::begin(card), std::end(card), 0, [](auto acc, auto cell){return acc + (cell != -1 ? cell : 0);})); 
          return true;
        }
      }
      return false;
    }), cards.end());
  }
  
  const int part1 = scores.front();
  std::cout << "Day 04 part 1: " << part1 << "\n"; 

  const int part2 = scores.back();
  std::cout << "Day 04 part 2: " << part2 << "\n"; 
}
