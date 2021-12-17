#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");

  constexpr int start_id = 0;
  constexpr int end_id = 1;
  int nextbigid = 1;
  int nextsmallid = 0;
  std::map<std::string,int> id_mapping{{"start",start_id}, {"end",end_id}};
  std::map<int,std::vector<int>> conns;
  std::string line;
  while (input >> line) {
    auto pos = line.find("-"); 
    std::string a = line.substr(0, pos);
    std::string b = line.substr(pos+1);
    if (!id_mapping.contains(a)) {id_mapping[a] = a[0]<='Z' ? nextbigid+=2 : nextsmallid+=2;}
    if (!id_mapping.contains(b)) {id_mapping[b] = b[0]<='Z' ? nextbigid+=2 : nextsmallid+=2;}
    if (b != "start" && a != "end") { conns[id_mapping[a]].push_back(id_mapping[b]); }
    if (a != "start" && b != "end") { conns[id_mapping[b]].push_back(id_mapping[a]); }
  }

  std::vector<std::vector<int>> routes{{start_id}};
  int part1 = 0;
  while (!routes.empty()) {
    auto route = std::move(routes.back());
    routes.pop_back();
    for (auto& step: conns[route.back()]) {
      if (step == end_id) {
        part1++;
      } else if (step&1 || std::find(route.begin(), route.end(), step) == route.end()) {
        auto newroute = route;
        newroute.push_back(step);
        routes.push_back(std::move(newroute));
      }
    } 
  }
  
  std::cout << "Day 12 part 1: " << part1 << "\n"; 

  routes = {{0, start_id}};
  int part2 = 0;
  while (!routes.empty()) {
    auto route = std::move(routes.back());
    routes.pop_back();
    for (auto& step: conns[route.back()]) {
      if (step == end_id) {
        part2++;
      } else if (step&1 || std::find(route.begin(), route.end(), step) == route.end()) {
        auto newroute = route;
        newroute.push_back(step);
        routes.push_back(std::move(newroute));
      } else if (route[0] == 0) {
        auto newroute = route;
        newroute[0] = 1;
        newroute.push_back(step);
        routes.push_back(std::move(newroute));
      }
    } 
  }

  std::cout << "Day 12 part 2: " << part2 << "\n";
}
