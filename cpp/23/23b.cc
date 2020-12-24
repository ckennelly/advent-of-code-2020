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

int64_t doGame(absl::string_view line) {
    std::list<int> cups;
    absl::flat_hash_map<int, std::list<int>::iterator> cup_labels;
    int max_cup = INT_MIN;
    for (auto c : line) {
        int cup = c - '0';

        auto it = cups.insert(cups.end(), cup);
        bool success = cup_labels.emplace(cup, it).second;
        assert(success);
        (void) success;

        max_cup = std::max(max_cup, cup);
    }
    assert(max_cup == cups.size());

    for (int i = max_cup + 1; i <= 1000000; i++) {
        auto it = cups.insert(cups.end(), i);
        bool success = cup_labels.emplace(i, it).second;
        assert(success);
        (void) success;
    }
    assert(cups.size() == 1000000);
    assert(cup_labels.size() == 1000000);
    max_cup = 1000000;

    for (int move = 0; move < 10 * 1000 * 1000; move++) {
        const size_t old_size = cups.size();

        const int current_label = cups.front();
        cups.erase(cups.begin());

        int destination_cup = current_label;

        std::array<int, 3> picked_up;
        auto pickup_it = cups.begin();
        for (int i = 0; i < 3; i++) {
            picked_up[i] = *pickup_it;
            pickup_it = cups.erase(pickup_it);
        }

        do {
            destination_cup--;
            if (destination_cup == 0) {
                destination_cup = max_cup;
            }

            if (destination_cup != picked_up[0] &&
                    destination_cup != picked_up[1] &&
                    destination_cup != picked_up[2]) {
                break;
            }
        } while (true);

        // The vector looks like:
        // [clockwise of current cup] ... [destination] [ picked up] ... current_label
        auto label_it = cup_labels.find(destination_cup);
        assert(*label_it->second == destination_cup);
        assert(label_it != cup_labels.end());

        auto insert_loc = label_it->second;
        for (auto v : picked_up) {
            ++insert_loc;
            insert_loc = cups.insert(insert_loc, v);
            cup_labels[v] = insert_loc;
        }

        cup_labels[current_label] = cups.insert(cups.end(), current_label);

        assert(cups.size() == old_size);
        (void) old_size;
    }

    auto it = cup_labels.find(1);
    assert(it != cup_labels.end());
    auto cup_it = it->second;
    assert(*cup_it == 1);

    int64_t product = 1;
    for (int i = 0; i < 2; i++) {
        ++cup_it;
        if (cup_it == cups.end()) {
            cup_it == cups.begin();
        }

        product *= static_cast<int64_t>(*cup_it);
    }

    absl::FPrintF(stderr, "input %s => product %d\n", line, product);
    return product;
}

int main(int argc, char **argv) {
    std::vector<std::string> lines =
        absl::StrSplit(readAll(), "\n", absl::SkipEmpty());
    assert(lines.size() == 1);

    assert(doGame("389125467") == 149245887792);

    absl::PrintF("%d\n", doGame(lines[0]));
}
