#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

struct token {
  char c = -1;
  int n = -1; 
};

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");


  std::vector<std::vector<token>> fishes;
  std::string line;
  while (input >> line) {
    std::vector<token> fish(line.size()); 
    std::transform(line.begin(), line.end(), fish.begin(), [](auto c){return (c>='0'&&c<='9') ? token{-1,c-'0'} : token{c,-1};});
    fishes.push_back(std::move(fish));
  }

  auto sumfish = [](const auto& acc, const auto& fish) {
    auto newfish = std::vector<token>{{'['}};
    newfish.reserve(acc.size() + fish.size() + 3);
    newfish.insert(newfish.end(), acc.begin(), acc.end());
    newfish.push_back({','});
    newfish.insert(newfish.end(), fish.begin(), fish.end());
    newfish.push_back({']'});

    bool done = false;
    while (!done) {
      size_t itoken = 0;
      int depth = 0;
      done = true;
      while (itoken < newfish.size()) {
        if (newfish[itoken].c == '[') {
          ++depth;
          ++itoken;
          if (depth > 4) {
            size_t leftpos = itoken-1;
            int l = newfish[itoken++].n;
            itoken++;
            int r = newfish[itoken++].n;
            size_t rightpos = itoken+1; 
            std::vector<token> tmpfish;
            tmpfish.reserve(newfish.size()-4);
            tmpfish.insert(tmpfish.end(), newfish.begin(), newfish.begin()+leftpos); 
            tmpfish.push_back({-1,0});
            tmpfish.insert(tmpfish.end(), newfish.begin()+rightpos, newfish.end()); 
            newfish = std::move(tmpfish);
            itoken = leftpos-1;
            while (itoken > 0) {
              if (newfish[itoken].n >= 0) {
                newfish[itoken].n += l; 
                break;
              } 
              --itoken;
            }
            itoken = leftpos+2;
            while (itoken < newfish.size()) {
              if (newfish[itoken].n >= 0) {
                newfish[itoken].n += r;
                break;
              }
              ++itoken;
            }
            done = false;
            break;
          }
        } else if (newfish[itoken].c == ']') {
          --depth;
          ++itoken;
        } else {
          ++itoken;
        }
      }
      if (!done) {continue;}
      done = true;
      itoken = 0;
      while (itoken < newfish.size()) {
        if (newfish[itoken].n > 9) {
          int l = newfish[itoken].n/2;
          int r = (newfish[itoken].n+1)/2;
          std::vector<token> tmpfish;
          tmpfish.reserve(newfish.size()+4);
          tmpfish.insert(tmpfish.end(), newfish.begin(), newfish.begin()+itoken);
          tmpfish.insert(tmpfish.end(), {{'['}, {-1,l}, {','}, {-1,r}, {']'}});
          tmpfish.insert(tmpfish.end(), newfish.begin()+itoken+1, newfish.end());
          newfish = std::move(tmpfish);
          done = false;
          break;
        } else {
          ++itoken;
        }
      }
    }
    return newfish;
  };
  auto resultfish = std::accumulate(fishes.begin()+1, fishes.end(), fishes[0], sumfish);

  auto magnitude = [](auto fish) {
    while (fish.size() > 1) {
      size_t itoken = 0;
      while (itoken < fish.size()) {
        if (fish[itoken].c == ',') {
          if (fish[itoken-1].n >= 0 && fish[itoken+1].n >=0) {
            int n = fish[itoken-1].n*3 + fish[itoken+1].n*2;
            std::vector<token> tmpfish;
            tmpfish.reserve(fish.size()-4); 
            tmpfish.insert(tmpfish.end(), fish.begin(), fish.begin()+itoken-2);
            tmpfish.push_back({-1,n});
            tmpfish.insert(tmpfish.end(), fish.begin()+itoken+3, fish.end());
            fish = std::move(tmpfish);
            itoken = 0;
            continue;
          }
        }
        itoken++;
      }
    } 
    return fish.front().n;
  };
  
  std::cout << "Day 18 part 1: " << magnitude(resultfish) << "\n";

  int max = 0;
  for (size_t a = 0; a < fishes.size(); ++a) {
    for (size_t b = 0; b < fishes.size(); ++b) {
      if (a==b) {continue;}
      max = std::max(max,magnitude(sumfish(fishes[a], fishes[b])));
    }
  }
  
  std::cout << "Day 18 part 2: " << max << "\n";
}

