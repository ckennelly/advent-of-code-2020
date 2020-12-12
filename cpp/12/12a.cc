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

    int pos_x = 0;
    int pos_y = 0;

    int dir_x = 1;
    int dir_y = 0;

    std::vector<std::string> lines = absl::StrSplit(input, "\n");

    for (absl::string_view line : lines) {
        if (line.empty()) { continue; }

        char delta_dir = line[0];
        int amount;
        absl::SimpleAtoi(line.substr(1), &amount);

        switch (delta_dir) {
            case 'N':
                pos_y += amount;
                break;
            case 'S':
                pos_y -= amount;
                break;
            case 'E':
                pos_x += amount;
                break;
            case 'W':
                pos_x -= amount;
                break;
            case 'L': {
                assert(amount % 90 == 0);
                int turns = amount / 90;
                while (turns-- > 0) {
                    int new_x = -dir_y;
                    int new_y = dir_x;

                    dir_x = new_x;
                    dir_y = new_y;
                }
                break;
            }
            case 'R': {
                assert(amount % 90 == 0);
                int turns = amount / 90;
                while (turns-- > 0) {
                    int new_x = dir_y;
                    int new_y = -dir_x;

                    dir_x = new_x;
                    dir_y = new_y;
                }
                break;
            }
            case 'F':
                pos_x += amount * dir_x;
                pos_y += amount * dir_y;
                break;
        }
    }

    printf("%d\n", abs(pos_x) + abs(pos_y));
}
