#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

struct cube {
  int x1, x2, y1, y2, z1, z2;

  friend std::ostream& operator<<(std::ostream& os, const cube& c);
  bool contains(const cube& c) const {
    return (c.x1 >= x1 && c.x2 <= x2 &&
            c.y1 >= y1 && c.y2 <= y2 && 
            c.z1 >= z1 && c.z2 <= z2);
  }
  
  bool some_overlap(const cube& c) const {
    return ((c.x1 >= x1 && c.x1 <= x2) || (c.x2 >= x1 && c.x2 <= x2)) &&
           ((c.y1 >= y1 && c.y1 <= y2) || (c.y2 >= y1 && c.y2 <= y2)) &&
           ((c.z1 >= z1 && c.z1 <= z2) || (c.z2 >= z1 && c.z2 <= z2));
  }

  bool no_overlap(const cube& c) const {
    return (c.x1 > x2 || c.x2 < x1 ||
            c.y1 > y2 || c.y2 < y1 ||
            c.z1 > z2 || c.z2 < z1);
  }

  long volume() const {
    return (x2-x1+1L)*(y2-y1+1L)*(z2-z1+1L);
  }

  std::vector<cube> splitX(int x) const {
    //if (x1 == x) {
    //  return {{x, x2, y1, y2, z1, z2}};
    //}
    //if (x2 == x) {
    //  return {{x1, x, y1, y2, z1, z2}};
    //}
    return {{x1, x, y1, y2, z1, z2}, {x+1, x2, y1, y2, z1, z2}};
  }
  std::vector<cube> splitY(int y) const {
    //if (y1 == y) {
    //  return {{x1, x2, y, y2, z1, z2}};
    //}
    //if (y2 == y) {
    //  return {{x1, x2, y1, y, z1, z2}};
    //}
    return {{x1, x2, y1, y, z1, z2}, {x1, x2, y+1, y2, z1, z2}};
  }
  std::vector<cube> splitZ(int z) const {
    //if (z1 == z) {
    //  return {{x1, x2, y1, y2, z+1, z2}};
    //}
    //if (z2 == z) {
    //  return {{x1, x2, y1, y2, z1, z}};
    //}
    return {{x1, x2, y1, y2, z1, z}, {x1, x2, y1, y2, z+1, z2}};
  }
};

void operator+=(std::vector<cube> &v1, const cube &c) {
  std::vector<cube> v2{c}; 
  for (const auto& c1: v1) {
//    std::cout << "Matching with: " << c1 << "\n";
    std::vector<cube> new_cubes;
    for (const auto& c2: v2) { 
//      std::cout << "Checking x: " << c2 << "\n";
      if (c1.contains(c2)) {continue;}
      if (c1.no_overlap(c2)) {new_cubes.push_back(c2); continue;}
      std::vector<cube> new_cubesx;
      if (c2.x1 >= c1.x1 && c2.x1 <= c1.x2 && c2.x2 > c1.x2) {
         new_cubesx = c2.splitX(c1.x2);
      } else if (c2.x2 >= c1.x1 && c2.x2 <= c1.x2 && c2.x1 < c1.x1) {
         new_cubesx = c2.splitX(c1.x1-1);
      } else if (c2.x1 < c1.x1 && c2.x2 > c1.x2) {
         auto ab = c2.splitX(c1.x1-1);
         auto bc = ab[1].splitX(c1.x2);
         new_cubesx.push_back(ab[0]);
         new_cubesx.push_back(bc[0]);
         new_cubesx.push_back(bc[1]);
      } else {
         new_cubesx.push_back(c2);
      }
      std::vector<cube> new_cubesy;
      for (auto& c3: new_cubesx) {
        if (c3.y1 >= c1.y1 && c3.y1 <= c1.y2 && c3.y2 > c1.y2) {
          auto a = c3.splitY(c1.y2);
          new_cubesy.insert(new_cubesy.end(), a.begin(), a.end());
        } else if (c3.y2 >= c3.y1 && c3.y2 <= c1.y2 && c3.y1 < c1.y1) {
          auto a = c3.splitY(c1.y1-1);
          new_cubesy.insert(new_cubesy.end(), a.begin(), a.end());
        } else if (c3.y1 < c1.y1 && c3.y2 > c1.y2) {
           auto ab = c3.splitY(c1.y1-1);
           auto bc = ab[1].splitY(c1.y2);
           new_cubesy.push_back(ab[0]);
           new_cubesy.push_back(bc[0]);
           new_cubesy.push_back(bc[1]);
        } else {
          new_cubesy.push_back(c3);
        }
      }
      std::vector<cube> new_cubesz;
      for (auto& c3: new_cubesy) {
        if (c3.z1 >= c1.z1 && c3.z1 <= c1.z2 && c3.z2 > c1.z2) {
          auto a = c3.splitZ(c1.z2);
          new_cubesz.insert(new_cubesz.end(), a.begin(), a.end());
        } else if (c3.z2 >= c3.z1 && c3.z2 <= c1.z2 && c3.z1 < c1.z1) {
          auto a = c3.splitZ(c1.z1-1);
          new_cubesz.insert(new_cubesz.end(), a.begin(), a.end());
        } else if (c3.z1 < c1.z1 && c3.z2 > c1.z2) {
           auto ab = c3.splitZ(c1.z1-1);
           auto bc = ab[1].splitZ(c1.z2);
           new_cubesz.push_back(ab[0]);
           new_cubesz.push_back(bc[0]);
           new_cubesz.push_back(bc[1]);
        } else {
          new_cubesz.push_back(c3);
        }
      }
      for (auto& c3: new_cubesz) {
        if (!c1.contains(c3)) {
          new_cubes.push_back(c3);
        }
      }
    }
    v2 = std::move(new_cubes);
  }  
  v1.insert(v1.end(), v2.begin(), v2.end());
}

void operator-=(std::vector<cube> &v1, const cube &c) {
  std::vector<cube> v_new; 
  for (const auto& c1: v1) {
    if (c1.no_overlap(c)) {v_new.push_back(c1); continue;} 
    auto next = c1;
    if (next.x1 < c.x1) {
      auto ab = next.splitX(c.x1-1);
      v_new.push_back(ab[0]);
      next = ab[1];
    }
    if (next.x2 > c.x2) {
      auto ab = next.splitX(c.x2);
      v_new.push_back(ab[1]);
      next = ab[0];
    }
    if (next.y1 < c.y1) {
      auto ab = next.splitY(c.y1-1);
      v_new.push_back(ab[0]);
      next = ab[1];
    }
    if (next.y2 > c.y2) {
      auto ab = next.splitY(c.y2);
      v_new.push_back(ab[1]);
      next = ab[0];
    }
    if (next.z1 < c.z1) {
      auto ab = next.splitZ(c.z1-1);
      v_new.push_back(ab[0]);
      next = ab[1];
    }
    if (next.z2 > c.z2) {
      auto ab = next.splitZ(c.z2);
      v_new.push_back(ab[1]);
    }
  }
  v1 = std::move(v_new);
}

std::ostream& operator<<(std::ostream& os, const cube& c) {
  os << "x=" << c.x1 << ".." << c.x2 << ",y=" << c.y1 << ".." << c.y2 << ",z=" << c.z1 << ".." << c.z2;
  return os;
}

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");

  std::string toggle, line;
  std::vector<std::pair<cube,bool>> cube_toggles;
  while (input >> toggle >> line) {
    auto x_start = line.find("x=");
    auto x_range = line.find("..",x_start);
    auto y_start = line.find(",y=",x_range);
    auto y_range = line.find("..",y_start);
    auto z_start = line.find(",z=",y_range);
    auto z_range = line.find("..",z_start);
    auto x1 = std::stoi(line.substr(x_start+2, x_range)); 
    auto x2 = std::stoi(line.substr(x_range+2, y_start));
    auto y1 = std::stoi(line.substr(y_start+3, y_range));
    auto y2 = std::stoi(line.substr(y_range+2, z_start));
    auto z1 = std::stoi(line.substr(z_start+3, z_range));
    auto z2 = std::stoi(line.substr(z_range+2));
    cube_toggles.push_back({{x1,x2,y1,y2,z1,z2},{toggle=="on"}}); 
  }
  
  std::vector<cube> cubesum;
  for (auto& [c,t]: cube_toggles) {
    if (c.x1 < -50) {break;}
    if (t) {
      cubesum += c;
    } else {
      cubesum -= c;
    }
  }

  auto part1 = std::accumulate(cubesum.begin(), cubesum.end(), 0L, [](const auto acc, auto& c){return acc + c.volume();});

  std::cout << "Day 22 part 1: " << part1 << "\n";

  std::vector<cube> cubesum2;
  for (auto& [c,t]: cube_toggles) {
    if (c.x1 >= -50 && c.x1 <= 50) {continue;}
    if (t) {
      cubesum2 += c;
    } else {
      cubesum2 -= c;
    }
  }

  auto part2 = std::accumulate(cubesum2.begin(), cubesum2.end(), 0L, [](const auto acc, auto& c){return acc + c.volume();});

  std::cout << "Day 22 part 2: " << part1+part2 << "\n";
}

