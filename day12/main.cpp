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

  std::map<std::string,std::vector<std::string>> conns;
  std::string line;
  while (input >> line) {
    auto pos = line.find("-"); 
    std::string a = line.substr(0, pos);
    std::string b = line.substr(pos+1);
    conns[a].push_back(b);
    conns[b].push_back(a);
  }

  std::vector<std::vector<std::string>> routes{{"start"}};
  std::vector<std::vector<std::string>> newroutes;
  std::vector<std::vector<std::string>> finishedroutes;
  while (!routes.empty()) {
    for (auto& route: routes) {
      for (auto& step: conns[route.back()]) {
        if (step == "end") {
          auto newroute = route;
          newroute.push_back(step);
          finishedroutes.push_back(std::move(newroute));
        } else if (step[0] <= 'Z' || std::find(route.begin(), route.end(), step) == route.end()) {
          auto newroute = route;
          newroute.push_back(step);
          newroutes.push_back(std::move(newroute));
        }
      } 
    }
    routes.swap(newroutes);
    newroutes.clear();
  }
  
  const int part1 = finishedroutes.size();
  std::cout << "Day 12 part 1: " << part1 << "\n"; 

  routes = {{"0", "start"}};
  finishedroutes.clear();
  while (!routes.empty()) {
    for (auto& route: routes) {
      for (auto& step: conns[route.back()]) {
        if (step == "start") {continue;}
        if (step == "end") {
          auto newroute = route;
          newroute.push_back(step);
          finishedroutes.push_back(std::move(newroute));
        } else if (step[0] <= 'Z' || std::find(route.begin(), route.end(), step) == route.end()) {
          auto newroute = route;
          newroute.push_back(step);
          newroutes.push_back(std::move(newroute));
        } else if (route[0] == "0") {
          auto newroute = route;
          newroute[0] = "1";
          newroute.push_back(step);
          newroutes.push_back(std::move(newroute));
        }
      } 
    }
    routes.swap(newroutes);
    newroutes.clear();
  }

  const int part2 = finishedroutes.size();
  std::cout << "Day 12 part 2: " << part2 << "\n";
}
