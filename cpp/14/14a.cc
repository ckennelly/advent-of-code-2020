#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/strings/ascii.h"
#include "absl/strings/numbers.h"
#include "absl/strings/match.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_split.h"

std::string readAll() {
    std::stringstream buffer;
    buffer << std::cin.rdbuf();
    return buffer.str();
}

int main(int argc, char **argv) {
    std::string input = readAll();
    std::vector<std::string> lines = absl::StrSplit(input, '\n');

    uint64_t ones_mask = 0;
    uint64_t zeroes_mask = -1;
    absl::flat_hash_map<uint64_t, uint64_t> memory;
    for (const std::string& line : lines) {
        if (line.empty()) {
            continue;
        }

        if (absl::StartsWith(line, "mask = ")) {
            ones_mask = 0;
            zeroes_mask = -1;

            for (int i = 7; i < line.size(); i++) {
                ones_mask <<= 1;
                zeroes_mask = (zeroes_mask << 1) | uint64_t{1};

                switch (line[i]) {
                    case 'X':
                        break;
                    case '1':
                        ones_mask |= uint64_t{1};
                        break;
                    case '0':
                        zeroes_mask &= static_cast<uint64_t>(-2);
                        break;
                }
            }
        } else {
            uint64_t address;
            uint64_t value;
            sscanf(line.c_str(), "mem[%lu] = %lu", &address, &value);

            value |= ones_mask;
            value &= zeroes_mask;

            memory[address] = value;
        }
    }

    uint64_t sum = 0;
    for (auto& [l, r] : memory) {
        sum += r;
    }

    printf("%lu\n", sum);
}
