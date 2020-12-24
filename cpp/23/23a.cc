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

int cup(absl::Span<int> cups, int index) {
    return cups[index % cups.size()];
}

std::string doGame(absl::string_view line) {
    std::vector<int> cups;
    int min_cup = INT_MAX;
    int max_cup = INT_MIN;
    for (auto c : line) {
        int cup = c - '0';
        cups.push_back(cup);
        min_cup = std::min(min_cup, cup);
        max_cup = std::max(max_cup, cup);
    }

    for (int move = 0; move < 100; move++) {
        const int current_label = cups[0];
        int destination_cup = current_label;

        do {
            if (destination_cup == min_cup) {
                destination_cup = max_cup + 1;
            }

            destination_cup--;

            bool is_picked_up = false;
            for (auto v : {cups[1], cups[2], cups[3]}) {
                if (v == destination_cup) {
                    is_picked_up = true;
                    break;
                }
            }

            if (!is_picked_up) {
                break;
            }

        } while (true);

        // The vector looks like:
        // [clockwise of current cup] ... [destination] [ picked up] ... current_label
        int label_idx = -1;
        for (int i = 0; i < cups.size(); i++) {
            if (cups[i] == destination_cup) {
                label_idx = i;
                break;
            }
        }
        assert(label_idx >= 4);
        assert(label_idx != -1);

        std::vector<int> new_cups;
        new_cups.insert(new_cups.end(), cups.begin() + 4, cups.begin() + label_idx);
        new_cups.push_back(destination_cup);
        new_cups.push_back(cups[1]);
        new_cups.push_back(cups[2]);
        new_cups.push_back(cups[3]);
        new_cups.insert(new_cups.end(), cups.begin() + label_idx + 1, cups.end());
        new_cups.push_back(current_label);

        assert(new_cups.size() == cups.size());
        cups = std::move(new_cups);
    }

    int one_idx = -1;
    for (int i = 0; i < cups.size(); i++) {
        if (cups[i] == 1) {
            one_idx = i;
            break;
        }
    }
    assert(one_idx != -1);

    std::string out;
    out.reserve(cups.size() - 1);
    for (int i = (one_idx + 1) % cups.size(); i != one_idx; i = (i + 1) % cups.size()) {
        out.push_back(cups[i] + '0');
    }
    return out;
}

int main(int argc, char **argv) {
    std::vector<std::string> lines =
        absl::StrSplit(readAll(), "\n", absl::SkipEmpty());
    assert(lines.size() == 1);

    assert(doGame("389125467") == "67384529");

    absl::PrintF("%s\n", doGame(lines[0]));
}
