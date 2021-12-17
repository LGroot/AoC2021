#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");

  std::string line;
  std::getline(input, line);

  const auto pos_xbegin = line.find("x=");
  const auto pos_xsep = line.find("..", pos_xbegin);
  const auto pos_xend = line.find(",", pos_xsep);
  const auto pos_ybegin = line.find("y=", pos_xend);
  const auto pos_ysep = line.find("..", pos_ybegin);

  const auto x1 = std::stoi(line.substr(pos_xbegin+2, pos_xsep-pos_xbegin-2));
  const auto x2 = std::stoi(line.substr(pos_xsep+2, pos_xend-pos_xsep-2));
  const auto y1 = std::stoi(line.substr(pos_ybegin+2, pos_ysep-pos_ybegin-2));
  const auto y2 = std::stoi(line.substr(pos_ysep+2));
   
  std::map<int,std::vector<int>> steps;
  int total_maxy = 0;
  // Assumes y1 and y2 are negative...
  const int ystart = y1;
  const int yend = -y1;
  for (int y=ystart; y<yend; ++y) {
    int posy = 0, maxy = 0, step = 0, vy = y;
    while (posy >= y1) {
      posy += vy;
      maxy = std::max(posy, maxy);
      --vy;
      ++step;
      if (posy >= y1 && posy <= y2) {
        total_maxy = std::max(maxy, total_maxy);
        steps[step].push_back(y);
      }
    }
  }

  std::cout << "Day 17 part 1: " << total_maxy << "\n"; 

  const auto max_steps = std::max_element(steps.begin(), steps.end(), [](const auto& l, const auto& r){return l.first < r.first;})->first;

  std::vector<int> yvals;
  yvals.reserve(yend-ystart);
  std::vector<std::pair<int,int>> coordinates;
  // Assumes x1 and x2 are positive...
  const int xstart = 1;
  const int xend = x2+1;
  for (int x=xstart; x<xend; ++x) {
    yvals.clear();
    int posx = 0, vx = x;
    for (int step=1; posx <= x2 && (vx > 0 || posx >= x1) && step <= max_steps; ++step) {
      posx += vx;
      if (vx) {--vx;} 
      if (posx >= x1 && posx <= x2) {
        for (auto y: steps[step]) {
          if (std::find(yvals.begin(), yvals.end(), y) == yvals.end()) {
            yvals.push_back(y);
            coordinates.push_back(std::make_pair(x, y));
          }
        } 
      }
    }
  }

  const auto part2 = coordinates.size();
  std::cout << "Day 17 part 2: " << part2 << "\n"; 
}
