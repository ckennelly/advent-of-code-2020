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
    std::vector<std::string> lines =
        absl::StrSplit(readAll(), '\n', absl::SkipEmpty());

    const int cycles = 8;
    const int height = lines.size() + 2 * cycles;
    const int width = lines[0].size() + 2 * cycles;

    const int zoffset = cycles;
    const int depth = zoffset * 2 + 1;
    const int wmax = cycles * 2 + 1;
    std::vector<std::vector<std::vector<std::string>>> states;
    states.resize(wmax);
    for (int w = 0; w < wmax; w++) {
        states[w].resize(depth);
        for (int i = 0; i < depth; i++) {
            if (w == zoffset && i == zoffset) {
                for (int j = 0; j < height; j++) {
                    if (j >= cycles && j < cycles + lines.size()) {
                        states[w][i].push_back(absl::StrCat(std::string(cycles, '.'), lines[j - cycles], std::string(cycles, '.')));
                    } else {
                        states[w][i].push_back(std::string(width, '.'));
                    }
                }

                continue;
            }

            for (int j = 0; j < height; j++) {
                states[w][i].push_back(std::string(width, '.'));
            }
        }
    }

    int active = 0;
    for (int w = 0; w < wmax; w++) {
        for (int z = 0; z < depth; z++) {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    if (states[w][z][y][x] == '#') {
                        active++;
                    }
                }
            }
        }
    }

    for (int i = 0; i < 6; i++) {
        std::vector<std::vector<std::vector<std::string>>> next = states;

        for (int w = 0; w < wmax; w++) {
            for (int z = 0; z < depth; z++) {
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {

                        int neighbors = 0;
                        for (int dx = -1; dx <= 1; dx++) {
                            for (int dy = -1; dy <= 1; dy++) {
                                for (int dz = -1; dz <= 1; dz++) {
                                    for (int dw = -1; dw <= 1; dw++) {
                                        if (dx == 0 && dy == 0 && dz == 0 && dw == 0) {
                                            continue;
                                        }

                                        if (x + dx < 0) {
                                            continue;
                                        }
                                        if (x + dx >= width) {
                                            continue;
                                        }
                                        if (y + dy < 0) {
                                            continue;
                                        }
                                        if (y + dy >= height) {
                                            continue;
                                        }
                                        if (z + dz < 0) {
                                            continue;
                                        }
                                        if (z + dz >= depth) {
                                            continue;
                                        }
                                        if (w + dw < 0) {
                                            continue;
                                        }
                                        if (w + dw >= wmax) {
                                            continue;
                                        }

                                        if (states[w + dw][z + dz][y + dy][x + dx] == '#') {
                                            neighbors++;
                                        }
                                    }
                                }
                            }
                        }

                        bool is_active = states[w][z][y][x] == '#';

                        if (is_active) { 
                            if (!(neighbors == 2 || neighbors == 3)) {
                                next[w][z][y][x] = '.';
                            } else {
                                next[w][z][y][x] = '#';
                            }
                        } else if (neighbors == 3) {
                            next[w][z][y][x] = '#';
                        } else {
                            next[w][z][y][x] = '.';
                        }
                    }
                }
            }
        }

        states = next;
    }

    active = 0;
    for (int w = 0; w < wmax; w++) {
        for (int z = 0; z < depth; z++) {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    if (states[w][z][y][x] == '#') {
                        active++;
                    }
                }
            }
        }
    }

    printf("%d\n", active);

}
