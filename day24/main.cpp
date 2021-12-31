#include <algorithm>
#include <array>
#include <iostream>
#include <utility>
#include <vector>

int main() {
  // Input manually parsed
  // 14 repeat operations on the input digits, with small variations

  constexpr int digits = 14;
  std::array<int,digits> n1{13, 15, 15, 11, -16, -11, -6, 11, 10, -10, -8, -11, 12, -15};
  std::array<int,digits> n2{5, 14, 15, 16, 8, 9, 2, 13,16, 6, 6, 9, 11, 5};
  std::array<int,digits> div{1, 1, 1, 1, 26, 26, 26, 1, 1, 26, 26, 26, 1, 26};

  struct model_number {
    long n, z;
  };

  for (int ipart=1; ipart <= 2; ++ipart) {
    std::vector<model_number> to_check{{0, 0}};
    long idigit = 1; 
    //Go from 9-1 in part1, 1-9 in part2
    long w_start = ipart==1 ? 9 : 1;
    long w_end = ipart==1 ? 0 : 10;
    long w_incr = ipart==1 ? -1 : 1;
    for (int round = 13; round >= 0; --round) {
      std::vector<model_number> new_check;
      for (long i=0; i<1000000; ++i) {
        for (long w=w_start; w!=w_end; w+=w_incr) {
          long z = i;
          long x = (((z%26)+n1[round]) != w);
          z /= div[round];
          z *= 25 * x + 1;
          z += (w+n2[round])*x;
          auto found = std::find_if(to_check.begin(), to_check.end(), [z](auto mn){return mn.z == z;});
          if (found != to_check.end()) {
            model_number a = {found->n + w*idigit, i};
            new_check.push_back(a);
            break;
          }
        }
      } 
      to_check = std::move(new_check);
      idigit *= 10;
    }
    auto res = ipart == 1 ? std::min_element(to_check.begin(), to_check.end(), [](auto a, auto b){return a.n < b.n;})
                          : std::max_element(to_check.begin(), to_check.end(), [](auto a, auto b){return a.n < b.n;});
    std::cout << "Day 24 part " << ipart << ": " << res->n << "\n";
  }
}

