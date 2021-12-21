#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <utility>
#include <vector>

struct point {
  int x, y, z;

  bool operator==(const auto& o) const {
    return x == o.x && y == o.y && z == o.z;
  }

  point operator+(const auto& o) const {
    return point{x+o.x, y+o.y, z+o.z};
  }

  point operator-(const auto& o) const {
    return point{x-o.x, y-o.y, z-o.z};
  }

  int dist(const auto& o) const {
    return std::abs(x-o.x) + std::abs(y-o.y) + std::abs(z-o.z);
  }

  point orientation(int i) const {
    switch (i) {
      //x along x with rotations around x
      case 0:
        return point{x, y, z};
      case 1:
        return point{x, -z, y};
      case 2:
        return point{x, -y, -z};
      case 3:
        return point{x, z, -y};
      //-x along x with rotations
      case 4:
        return point{-x, -y, z};
      case 5:
        return point{-x, -z, -y};
      case 6:
        return point{-x, y, -z};
      case 7:
        return point{-x, z, y};
      //y along x with rotations
      case 8:
        return point{y, -x, z};
      case 9:
        return point{y, -z, -x};
      case 10:
        return point{y, x, -z};
      case 11:
        return point{y, z, x};
      //-y along x with rotations
      case 12:
        return point{-y, x, z};
      case 13:
        return point{-y, -z, x};
      case 14:
        return point{-y, -x, -z};
      case 15:
        return point{-y, z, -x};
      //z along x with rotations
      case 16:
        return point{z, y, -x};
      case 17:
        return point{z, x, y};
      case 18:
        return point{z, -y, x};
      case 19:
        return point{z, -x, -y};
      //-z along x with rotations
      case 20:
        return point{-z, y, x};
      case 21:
        return point{-z, -x, y};
      case 22:
        return point{-z, -y, -x};
      case 23:
        return point{-z, x, -y};
      default:
        std::cerr << "Illegal orientation\n";
        std::exit(EXIT_FAILURE);
    }
  }
};

std::ostream& operator<<(std::ostream& os, const point& p) {
  os << p.x << "," << p.y << "," << p.z;
  return os; 
}

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");

  std::vector<std::vector<point>> scanners;
  std::string tmp;
  int x, y, z;
  while (std::getline(input, tmp)) {
    std::vector<point> scanner;
    while (input >> x) {
      input.ignore();   
      input >> y;
      input.ignore();
      input >> z;
      input.ignore();
      scanner.push_back({x, y, z}); 
    }
    scanners.push_back(std::move(scanner)); 
    input.clear();
    input >> tmp;
  }

  //std::vector<std::vector<int>> scanner_distances;
  //for (const auto& scanner: scanners) {
  //  std::vector<int> distances;
  //  distances.reserve((scanner.size()*scanner.size()-1)/2);
  //  for (size_t i=0; i<scanner.size();++i) {
  //    for (size_t j=i+1; j<scanner.size();++j) {
  //      distances.push_back(scanner[i].dist(scanner[j])); 
  //    }
  //  } 
  //  scanner_distances.push_back(std::move(distances));
  //}
  
  std::vector<std::pair<point,size_t>> positions;
  positions.reserve(scanners.size());
  positions.push_back(std::make_pair(point{0,0,0},0)); 
  std::vector<point> s1;
  while (scanners.size() > 1) {
    for (size_t jscanner = 0; jscanner < scanners.size(); ++jscanner) {
      auto& scanner0 = scanners[jscanner]; 
      //auto& distances0 = scanner_distances[jscanner];
      for (size_t iscanner=jscanner+1; iscanner<scanners.size(); ++iscanner) {
        //auto& distances1 = scanner_distances[iscanner];
        //int count_distances = 0;
        //for (auto dist: distances1) {
        //  count_distances += std::find(distances0.begin(), distances0.end(), dist) != distances0.end(); 
        //  if (count_distances > 65) {break;}
        //}
        //if (count_distances < 66) {continue;}
        //std::cout << "first check: " << jscanner << " " << iscanner << "\n";
        s1.reserve(scanners[iscanner].size());
        for (int iorientation=0; iorientation<24; ++iorientation) {
          std::transform(scanners[iscanner].begin(), scanners[iscanner].end(), std::back_inserter(s1), [iorientation](const auto& s){return s.orientation(iorientation);});

          bool found = false;
          for (size_t a=0; a<scanner0.size()-11 && !found; ++a) {
            for (size_t b=0; b<s1.size()-11; ++b) {
              auto offset = scanner0[a] - s1[b];
              int count = 1;
              for (size_t c=0; c<s1.size() && count+s1.size()-c>11; ++c) {
                if (b==c) {continue;}
                auto target = s1[c] + offset;
                count += std::find(scanner0.begin(), scanner0.end(), target) != scanner0.end();
                if (count > 11) {break;}
              }
              if (count > 11) {
                found = true;
                //std::cout << "found: " << jscanner << " " << iscanner << "\n";
                for (size_t c=0; c<s1.size(); ++c) {
                  if (b==c) {continue;}
                  auto target = s1[c] + offset;
                  if (std::find(scanner0.begin(), scanner0.end(), target) == scanner0.end()) {
                     scanner0.push_back(target);
                  }
                }
                for (auto& pos : positions) {
                  if (pos.second == iscanner) {
                    pos.first = pos.first.orientation(iorientation)+offset;
                    pos.second = jscanner;
                  } else if (pos.second > iscanner) {
                    pos.second--;
                  }
                }
                positions.push_back(std::make_pair(offset, jscanner));
                scanners.erase(scanners.begin()+iscanner);
                //for (size_t i=0; i<scanner0.size(); ++i) {
                //  for (size_t j=0; j<s1.size(); ++j) {
                //    distances0.push_back(scanner0[i].dist(s1[j]));
                //  }
                //}
                //distances0.insert(distances0.end(), distances1.begin(), distances1.end());
                //scanner_distances.erase(scanner_distances.begin()+iscanner);
                iscanner--;
                break;
              }
            }
          }
          s1.clear();
          if (found) {break;}
        }
      }
    }
  }

  const int part1 = scanners.front().size(); 
  std::cout << "Day 19 part 1: " << part1 << "\n";

  int max = 0;
  for (size_t a=0; a<positions.size(); ++a) {
    for (size_t b=a+1; b<positions.size(); ++b) {
      auto dist = positions[a].first.dist(positions[b].first); 
      max = std::max(dist, max);
    }
  }
  std::cout << "Day 19 part 2: " << max << "\n";
}

