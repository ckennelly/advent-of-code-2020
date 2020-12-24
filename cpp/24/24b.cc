#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/strings/ascii.h"
#include "absl/strings/numbers.h"
#include "absl/strings/match.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"
#include "absl/types/optional.h"

std::string readAll() {
    std::stringstream buffer;
    buffer << std::cin.rdbuf();
    return buffer.str();
}

enum Dirs {
    E,
    SE,
    SW,
    W,
    NW,
    NE,
};

std::vector<Dirs> parseLine(absl::string_view line) {
    std::vector<Dirs> dirs;

    size_t offset = 0;
    while (offset < line.size()) {
        size_t rem = line.size() - offset;
        if (rem >= 2) {
            if (line[offset] == 'n') {
                switch (line[offset + 1]) {
                    case 'w':
                        dirs.push_back(NW);
                        break;
                    case 'e':
                        dirs.push_back(NE);
                        break;
                    default:
                        assert(false);
                }

                offset += 2;
                continue;
            } else if (line[offset] == 's') {
                switch (line[offset + 1]) {
                    case 'w':
                        dirs.push_back(SW);
                        break;
                    case 'e':
                        dirs.push_back(SE);
                        break;
                    default:
                        assert(false);
                }

                offset += 2;
                continue;
            }
        }
        if (line[offset] == 'e') {
            dirs.push_back(E);
            offset++;
        } else if (line[offset] == 'w') {
            dirs.push_back(W);
            offset++;
        } else {
            assert(false);
        }
    }

    return dirs;
}

struct Coordinate {
    int x = 0;
    int y = 0;
    int z = 0;

    bool operator==(const Coordinate& rhs) const {
        return x == rhs.x && y ==rhs.y && z == rhs.z;
    }

    template <typename H>
    friend H AbslHashValue(H h, const Coordinate& t) {
      return H::combine(std::move(h), t.x, t.y, t.z);
    }
};

int countGrid(const absl::flat_hash_map<Coordinate, bool>& grid) {
    int count = 0;
    for (const auto& [l, r] : grid) {
        if (r) {
            count++;
        }
    }
    return count;
}

Coordinate applyDirection(Coordinate tile, Dirs direction) {
            switch (direction) {
                case NE:
                    tile.y++;
                    tile.z++;
                    break;
                case NW:
                    tile.x--;
                    tile.z++;
                    break;
                case SE:
                    tile.x++;
                    tile.z--;
                    break;
                case SW:
                    tile.y--;
                    tile.z--;
                    break;
                case E:
                    tile.x++;
                    tile.y++;
                    break;
                case W:
                    tile.x--;
                    tile.y--;
                    break;
            }
        return tile;
    }



absl::flat_hash_map<Coordinate, bool> gameOfLife(const absl::flat_hash_map<Coordinate, bool>& old) {
    absl::flat_hash_set<Coordinate> known;

    static constexpr Dirs directions[] = {NE, NW, E, W, SE, SW};
    for (const auto& [l, r] : old) {
        known.insert(l);
        for (auto direction : directions) {
            known.insert(applyDirection(l, direction));
        }
    }

    absl::flat_hash_map<Coordinate, bool> ret;
    for (Coordinate c : known) {
        int neighbors = 0;
        for (auto direction : directions) {
            Coordinate neighbor = applyDirection(c, direction);
            auto it = old.find(neighbor);
            if (it != old.end() && it->second) {
                neighbors++;
            }
        }

        auto it = old.find(c);
        if (it != old.end() && it->second) {
            // Black
            if (neighbors == 0 || neighbors > 2) {
                ret[c] = false;
            } else {
                ret[c] = true;
            }
        } else {
            // White
            if (neighbors == 2) {
                ret[c] = true;
            } else {
                ret[c] = false;
            }
        }
    }

    return ret;
}

int main(int argc, char **argv) {
    std::vector<std::string> lines =
        absl::StrSplit(readAll(), "\n", absl::SkipEmpty());

    absl::flat_hash_map<Coordinate, bool> grid;
    for (const auto& line : lines) {
        Coordinate tile;

        std::vector<Dirs> dirs = parseLine(line);
        for (auto direction : dirs) {
            tile = applyDirection(tile, direction);
        }

        grid[tile] = !grid[tile];
    }

    for (int i = 1; i <= 100; i++) {
        grid = gameOfLife(grid);
        int count = countGrid(grid);
        absl::PrintF("Day %d: %d\n", i, count);
    }

    int count = countGrid(grid);
    absl::PrintF("%d\n", count);

}
