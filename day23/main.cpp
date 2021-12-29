#include <algorithm>
#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

constexpr std::array<int,7> corridor{1,2,4,6,8,10,11};
constexpr std::array<int,4> homes{3,5,7,9};
constexpr int ycorridor = 1;


int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");

  std::vector<std::string> grid;
  std::string line;
  while (std::getline(input, line)) {
    grid.push_back(std::move(line));
  }
  
  struct my_state {
    std::vector<std::string> grid;
    int score;
    long hashval;
 
    my_state (const std::vector<std::string>& grid, int score) : grid(grid), score(score) {calc_hashval();}
    my_state (std::vector<std::string>&& grid, int score) : grid(std::move(grid)), score(score) {calc_hashval();}

    void calc_hashval() {
      auto c2l = [](auto c){return c=='.' ? 0L : c-'A'+1L;};
      long mult = 1L;
      hashval = 0L;
      for (size_t icorridor = 0; icorridor < corridor.size(); ++icorridor) {
        hashval += c2l(grid[ycorridor][corridor[icorridor]])*mult;
        mult *= 5L;
      }
      for (size_t y=2; y<grid.size()-1; ++y) {
        for (size_t ihome = 0; ihome < homes.size(); ++ihome) {
          hashval += c2l(grid[y][homes[ihome]])*mult;
          mult *= 5L;
        }
      }
    }
  };

  constexpr int scores[] = {1, 10, 100, 1000};

  for (int ipart=1; ipart<=2; ++ipart) {

    const size_t h = 2+ipart*2;
    if (ipart == 2) {
      const std::string insert1 = "  #D#C#B#A#";
      const std::string insert2 = "  #D#B#A#C#";
      constexpr int insert_offset = 3;
      grid.insert(grid.begin()+insert_offset, insert2);
      grid.insert(grid.begin()+insert_offset, insert1);
    }
  
    auto finished_grid = grid;
    for (size_t y=2; y<grid.size()-1; ++y) {
      for (size_t ihome=0; ihome<homes.size(); ++ihome) {
        finished_grid[y][homes[ihome]] = 'A'+ihome;
      }
    }
    my_state finished_state = {finished_grid,0};
    long finished_hash = finished_state.hashval;
  
    std::vector<my_state> all_states{{grid,0}}; 
    auto comp_score = [](const auto& l, const auto& r){return l.score > r.score;};
    std::priority_queue<my_state, std::vector<my_state>, decltype(comp_score)> check_states(comp_score);
    check_states.push({grid,0});
    my_state final_state = finished_state;
    auto has_state = [](const my_state& l, const my_state& r) noexcept { return l.hashval == r.hashval; };
  
    while (!check_states.empty()) {
      auto state = std::move(check_states.top());
      check_states.pop();
      if (state.hashval == finished_hash) {
        final_state = std::move(state);
        break;
      } 
   
      // Corridor moves 
      for (size_t icorridor = 0; icorridor<corridor.size(); ++icorridor) {
        auto x = corridor[icorridor]; 
        auto c = state.grid[ycorridor][x]; 
        if (c == '.') {continue;}
        auto xhome = homes[c-'A'];
        auto x1 = x;
        auto jcorridor = icorridor;
        bool openpath = true;
        while (x1+1 < xhome) {
          x1 = corridor[++jcorridor];
          if (state.grid[ycorridor][x1] != '.') {openpath = false; break;}
        }
        while (x1-1 > xhome) {
          x1 = corridor[--jcorridor];
          if (state.grid[ycorridor][x1] != '.') {openpath = false; break;}
        }
        if (!openpath) {continue;}
        int yhome = h-1;
        for (int y=h-1; y>1; --y) {
          if (state.grid[y][xhome] == '.') {continue;}
          if (state.grid[y][xhome] != c) {openpath = false; break;}
          yhome = y-1;
        }
        if (!openpath) {continue;}
        auto new_state = state;
        std::swap(new_state.grid[ycorridor][x], new_state.grid[yhome][xhome]);
        new_state.calc_hashval();
        new_state.score += scores[c-'A']*(std::abs(xhome-x) + yhome-1) ;
        auto a = std::bind_front(has_state, std::cref(new_state));
        auto found_state = std::find_if(all_states.begin(), all_states.end(), a);
        if (found_state == all_states.end()) {
          all_states.push_back(new_state);
          check_states.push(std::move(new_state));
        } else if (new_state.score < found_state->score) {
          found_state->score = new_state.score;
          check_states.push(std::move(new_state));
        }
      }
  
      // Moves going out of house
      for (int ihome = 0; ihome<std::ssize(homes); ++ihome) {
        auto x = homes[ihome];
        for (size_t y = 2; y<h; ++y) {
          auto c = state.grid[y][x];
          if (c == '.') {continue;}
          // Only allow leaving home if there are still strangers
          if (c-'A' == ihome) {
            bool ok = false;
            for (size_t y2=y+1; y2<h; ++y2) {
              if (state.grid[y2][x] != c) {ok = true; break;}
            }
            if (!ok) {break;}
          } 
          for (int icorridor = ihome+1; icorridor>=0; --icorridor) {
            auto xcorridor = corridor[icorridor];
            if (state.grid[ycorridor][xcorridor] != '.') {break;}
            auto new_state = state;
            std::swap(new_state.grid[ycorridor][xcorridor], new_state.grid[y][x]);
            new_state.calc_hashval();
            new_state.score += scores[c-'A']*(std::abs(xcorridor-x) + y-1) ;
            auto a = std::bind_front(has_state, std::cref(new_state));
            auto found_state = std::find_if(all_states.begin(), all_states.end(), a);
            if (found_state == all_states.end()) {
              all_states.push_back(new_state);
              check_states.push(std::move(new_state));
            } else if (new_state.score < found_state->score) {
              found_state->score = new_state.score;
              check_states.push(std::move(new_state));
            }
          }
          for (size_t icorridor = ihome+2; icorridor<corridor.size(); ++icorridor) {
            auto xcorridor = corridor[icorridor];
            if (state.grid[ycorridor][xcorridor] != '.') {break;}
            auto new_state = state;
            std::swap(new_state.grid[ycorridor][xcorridor], new_state.grid[y][x]);
            new_state.calc_hashval();
            new_state.score += scores[c-'A']*(std::abs(xcorridor-x) + y-1) ;
            auto a = std::bind_front(has_state, std::cref(new_state));
            auto found_state = std::find_if(all_states.begin(), all_states.end(), a);
            if (found_state == all_states.end()) {
              all_states.push_back(new_state);
              check_states.push(std::move(new_state));
            } else if (new_state.score < found_state->score) {
              found_state->score = new_state.score;
              check_states.push(std::move(new_state));
            }
          }
          break;
        }
      }
    } 
  
    auto score = final_state.score;
    
    std::cout << "Day 23 part " << ipart << ": " << score << "\n";

  }
}

