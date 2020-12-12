#include <algorithm>
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

enum States {
    Empty,
    Occupied,
    Floor,
};

using Line = std::vector<States>;

int main(int argc, char **argv) {
    std::string input = readAll();

    std::vector<std::string> lines = absl::StrSplit(input, '\n');
    std::vector<Line> map;
    for (auto& s : lines) {
        if (s.empty()) {continue;} 

        map.emplace_back();
        auto& last = map.back();

        last.reserve(s.size());
        for (int i = 0; i < s.size(); i++) {
            switch (s[i]) {
                case 'L':
                    last.push_back(Empty);
                    break;
                case '#':
                    last.push_back(Occupied);
                    break;
                case '.':
                    last.push_back(Floor);
                    break;
            }
        }
    }

    while (true) {
        std::vector<Line> next = map;

        for (int i = 0; i < map.size(); i++) {
            for (int j = 0; j < map[i].size(); j++) {
                switch (map[i][j]) {
                    case Empty:
                    case Occupied: {
                        int neighbors = 0;
                        for (int di = -1; di <= 1; di++) {
                            if (i + di < 0) {
                                continue;
                            }

                            if (i + di >= map.size()) {
                                continue;
                            }

                            for (int dj = -1; dj <= 1; dj++) {
                                if (di == 0 && dj == 0) {
                                    continue;
                                }

                                if (j + dj < 0) {
                                    continue;
                                }

                                if (j + dj >= map[i + di].size()) {
                                    continue;
                                }

                                if (map[i + di][j + dj] == Occupied) {
                                    neighbors++;
                                }
                            }
                        }

                        if (map[i][j] == Empty) {
                            if (neighbors == 0) {
                                next[i][j] = Occupied;
                            } else {
                                next[i][j] = Empty;
                            }
                        } else {
                            if (neighbors >= 4) {
                                next[i][j] = Empty;
                            } else {
                                next[i][j] = Occupied;
                            }
                        }
                        break;
                    }
                    case Floor:
                        next[i][j] = Floor;
                        break;
                }
            }
        }

        if (next == map) {
            break;
        }

        map = std::move(next);
    }

    int total = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == Occupied) {
                total++;
            }
        }
    }

    printf("%d\n", total);
}
