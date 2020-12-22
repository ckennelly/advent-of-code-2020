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

std::deque<int> parsePlayer(const std::string& line) {
    std::deque<int> ret;

    for (absl::string_view sv : absl::StrSplit(line, '\n', absl::SkipEmpty())) {
        int card;
        if (absl::SimpleAtoi(sv, &card)) {
            ret.push_back(card);
        }
    }

    return ret;
}

int main(int argc, char **argv) {
    std::vector<std::string> lines =
        absl::StrSplit(readAll(), "\n\n", absl::SkipEmpty());
    assert(lines.size() == 2);

    std::deque<int> player1, player2;
    player1 = parsePlayer(lines[0]);
    player2 = parsePlayer(lines[1]);

    while (!(player1.empty() || player2.empty())) {
        int p1 = player1.front();
        player1.pop_front();
        int p2 = player2.front();
        player2.pop_front();

        if (p1 < p2) {
            player2.push_back(p2);
            player2.push_back(p1);
        } else {
            assert(p1 != p2);
            assert(p2 < p1);

            player1.push_back(p1);
            player1.push_back(p2);
        }
    }

    assert(player1.empty() || player2.empty());
    assert(!player1.empty() || !player2.empty());

    int64_t score = 0;
    while (!player1.empty()) {
        score += player1.size() * player1.front();
        player1.pop_front();
    }
    while (!player2.empty()) {
        score += player2.size() * player2.front();
        player2.pop_front();
    }

    printf("%ld\n", score);
}
