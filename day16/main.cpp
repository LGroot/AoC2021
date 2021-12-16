#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <algorithm>
#include <functional>
#include <filesystem>

int main(int, char* argv[]) {
  std::ifstream input(std::filesystem::path{argv[0]}.parent_path() / "input");
  
  std::string line;
  input >> line;
  std::vector<bool> bits;
  bits.reserve(line.size()*4);
  for (auto c: line) {
    unsigned char byte = (c >= 'A') ? (c - 'A' + 10) : (c - '0');
    bits.push_back(byte&0x8);
    bits.push_back(byte&0x4);
    bits.push_back(byte&0x2);
    bits.push_back(byte&0x1);
  }

  struct packet {
    int op = 0, length_type = 0;
    size_t length_val = 0;
    std::vector<long> values{};
    long apply() const {
      switch (op) {
        case 0: //sum
          return std::accumulate(values.begin(), values.end(), 0L, std::plus<>());
        case 1: //product
          return std::accumulate(values.begin(), values.end(), 1L, std::multiplies<>());
        case 2: //min
          return *std::min_element(values.begin(), values.end());
        case 3: //max
          return *std::max_element(values.begin(), values.end());
        case 4: //literal
          return values.front();
        case 5: //greater than
          return values[0] > values[1];
        case 6: //less than
          return values[0] < values[1];
        case 7: //equal to
          return values[0] == values[1];
        default:
          std::cerr << "Illegal op: " << op << "\n";
          std::exit(EXIT_FAILURE);
      }
    }
  };
  std::vector<packet> packets{{4}};

  enum class state {version_op, literal, length, check};
  state s = state::version_op;

  int total_version = 0;
  for (size_t ibit=0; ibit < bits.size();) {
    switch (s) {
      case state::version_op: {
        int version = bits[ibit++] << 2;
        version += bits[ibit++] << 1;
        version += bits[ibit++];
        total_version += version;

        int op = bits[ibit++] << 2;
        op += bits[ibit++] << 1;
        op += bits[ibit++];
        if (op == 4) {
          s = state::literal;
        } else {
          s = state::length;
          packets.push_back({op});
        }
        break; 
      }
      case state::literal: {
        long val = 0;
        do {
          for (int i=0; i<4; i++) {
            val = (val<<1) + bits[++ibit];
          }
        } while (bits[++ibit-5]);
        auto& packet = packets.back();
        packet.values.push_back(val); 
        s = state::check;
        break;
      }
      case state::length: {
        auto& packet = packets.back();
        packet.length_type = bits[ibit++];
        const int readbits = packet.length_type ? 11 : 15;
        int length = 0;
        for (int i=0; i<readbits; ++i) {
          length = (length<<1) + bits[ibit++]; 
        }
        if (packet.length_type == 0) {length += ibit;}
        packet.length_val = length;
        s = state::version_op;
        break;
      }
      case state::check: {
        auto& packet = packets.back();
        if ((packet.length_type == 0 && packet.length_val == ibit) || 
            (packet.length_type == 1 && --packet.length_val == 0)) {
          auto v = packet.apply(); 
          packets.pop_back();
          packets.back().values.push_back(v);
          if (packets.size() == 1) {s = state::version_op;}
        } else {
          s = state::version_op;
        }
        break;
      }
    } 
  }

  std::cout << "Day 15 part 1: " << total_version << "\n"; 

  const auto part2 = packets.front().apply();
  std::cout << "Day 15 part 2: " << part2 << "\n"; 
}
