#include <algorithm>
#include <array>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");
  std::string ignore;

  constexpr size_t nplayers = 2;
  std::array<int,nplayers> pos;
  for (size_t iplayer=0; iplayer<nplayers; ++iplayer) {
    input >> ignore >> ignore >> ignore >> ignore >> pos[iplayer];
  }
  auto pos2 = pos;

  constexpr int start_score = 0;
  constexpr int spaces = 10;
  constexpr int winscore = 1000;
  constexpr int rolls_per_turn = 3;
  constexpr int one_indexing = 1; // Subtract by one before modulo, increase by one after the modulo
  constexpr int modroll_increase = 9-one_indexing; // We increase 3 rolls by 3, so total increase of 9.
  std::array<int,nplayers> scores;
  std::fill(scores.begin(), scores.end(), start_score);
  int part1 = 0;
  int nturns = 0;
  int modroll = 6-one_indexing; // First increase is 1+2+3=6

  while (part1 == 0) {
    for (size_t iplayer=0; iplayer<nplayers; ++iplayer) {
      pos[iplayer] = (pos[iplayer] + modroll)%spaces + one_indexing; 
      scores[iplayer] += pos[iplayer];
      modroll = (modroll+modroll_increase)%spaces + one_indexing;
      nturns++;
      if (scores[iplayer] >= winscore) {
        part1 = scores[(iplayer+1)%nplayers] * nturns*rolls_per_turn;
        break;
      } 
    }

  }

  std::cout << "Day 21 part 1: " << part1 << "\n";


  std::array<long,nplayers> wins;
  std::fill(wins.begin(), wins.end(), 0);

  constexpr int roll_combinations = 7;
  std::array<int,roll_combinations> rolls{2, 3, 4, 5, 6, 7, 8}; //Offset by one for one indexing
  std::array<int,roll_combinations> rolls_m{1, 3, 6, 7, 6, 3, 1};
  
  constexpr size_t maxturns = 21;
  std::array<std::array<std::array<long,spaces+one_indexing>,maxturns>,nplayers> universes;
  for (auto& player_scores: universes) {
    for (auto& player_pos: player_scores) {
      std::fill(player_pos.begin(), player_pos.end(), 0);
    }
  }

  long new_universes = 1;
  for (size_t iplayer=0; iplayer<nplayers; ++iplayer) {
    universes[iplayer][start_score][pos2[iplayer]] = new_universes;
  }
   
  for (size_t iturn=0; iturn<maxturns; ++iturn) {
    for (size_t iplayer=0; iplayer<nplayers; ++iplayer) {
      long total_added_universes = 0;
      auto& s = universes[iplayer];
      for (size_t iscore=maxturns-1; iscore<maxturns && iscore>=iturn; iscore--) {
        auto& positions = s[iscore];
        for (size_t ipos=one_indexing; ipos<=spaces; ++ipos) {
          auto& u = positions[ipos];
          if (u==0) {continue;}
          for (size_t iroll = 0; iroll<roll_combinations; ++iroll) {
            auto nextpos = (ipos+rolls[iroll])%spaces + one_indexing; 
            auto nextscore = iscore+nextpos; 
            auto added_universes = u*rolls_m[iroll];
            if (nextscore >= maxturns) {
              wins[iplayer] += added_universes*new_universes;
            } else {
              s[nextscore][nextpos] += added_universes;
              total_added_universes += added_universes;
            }
          }
          u = 0;
        }
      }
      new_universes = total_added_universes;
    }
  }

  const auto part2 = *std::max_element(wins.begin(), wins.end()); 
  std::cout << "Day 21 part 2: " << part2 << "\n";
}

