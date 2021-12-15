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
  
  std::string sequence;
  input >> sequence;

  std::map<std::string,std::string> rules;
  std::string pair, ignore, element;
  while (input >> pair >> ignore >> element) {
    rules[pair] = element;
  }

  std::map<std::string,long> pairs;
  for (size_t i=0; i<sequence.size()-1; ++i) {
    auto pair = sequence.substr(i,2);
    pairs[pair]++;
  }
  
  constexpr int steps[] = {10, 30};
  for (int ipart = 0; ipart<2; ++ipart) {
    for (int i=0; i<steps[ipart]; ++i) {
      std::map<std::string,long> newpairs;
      for (auto& keyval: pairs) {
        auto& next = rules[keyval.first];
        newpairs[keyval.first[0]+next] += keyval.second;
        newpairs[next+keyval.first[1]] += keyval.second;
      }
      pairs.swap(newpairs);
    }

    std::map<char,long> counts{{sequence.back(),1}};
    for (auto& keyval: pairs) {
      counts[keyval.first[0]] += keyval.second; 
    }
    const auto [min, max] = std::minmax_element(counts.begin(), counts.end(), [](const auto l, const auto r) { return l.second < r.second; });
    const long part = max->second-min->second;

    std::cout << "Day 14 part " << ipart+1 << ": " << part << "\n"; 
  }
}
