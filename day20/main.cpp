#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");

  std::string alg;
  input >> alg;
  std::vector<bool> algorithm;
  algorithm.reserve(alg.size());
  std::transform(alg.begin(), alg.end(), std::back_inserter(algorithm), [](const auto c) {return c=='#';});

  input.ignore();

  std::string line;
  std::vector<std::vector<bool>> image;
  while (input >> line) {
    std::vector<bool> row;
    row.reserve(line.size());
    std::transform(line.begin(), line.end(), std::back_inserter(row), [](const auto c) {return c=='#';});
    image.push_back(std::move(row));
  }

  auto part1 = 0;
  bool infinity = false;
  for (int istep=0; istep<50; ++istep) {
    for (auto& row: image) {
      row.insert(row.begin(), 2, infinity);
      row.insert(row.end(), 2, infinity);
    }
    size_t rowlen = image[0].size();
    std::vector<bool> extrarows(rowlen, infinity);
    image.insert(image.begin(), 2, extrarows);
    image.insert(image.end(), 2, extrarows);
//    image.push_back(std::move(extrarows));

    std::vector<std::vector<bool>> newimage; 
    newimage.reserve(image.size());
    for (size_t y=1; y<image.size()-1; ++y) {
      std::vector<bool> newrow;
      newrow.reserve(image[y].size());
      for (size_t x=1; x<image[y].size()-1; ++x) {
        size_t val = (image[y-1][x-1]<<8) + (image[y-1][x]<<7) + (image[y-1][x+1]<<6);
        val += (image[y][x-1]<<5) + (image[y][x]<<4) + (image[y][x+1]<<3);
        val += (image[y+1][x-1]<<2) + (image[y+1][x]<<1) + (image[y+1][x+1]);
        newrow.push_back(algorithm[val]);
      }
      newimage.push_back(std::move(newrow));
    }
    image = std::move(newimage);
    infinity = infinity ? algorithm.back() : algorithm.front();
    if (istep == 1) {
      part1 = std::accumulate(image.begin(), image.end(), 0, [](auto acc, const auto& row){
        return acc + std::count(row.begin(), row.end(), true);
      });
    }
  }

  auto part2 = std::accumulate(image.begin(), image.end(), 0, [](auto acc, const auto& row){
    return acc + std::count(row.begin(), row.end(), true);
  });


  std::cout << "Day 20 part 1: " << part1 << "\n";

  std::cout << "Day 20 part 2: " << part2 << "\n";
}

