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

bool computeWinner(std::deque<int>* player1, std::deque<int>* player2, 
                   int depth,
                   std::vector<absl::flat_hash_set<std::pair<std::vector<int>, std::vector<int>>>>* seen) {
    assert(depth < 100);

    while (!(player1->empty() || player2->empty())) {
        std::pair<std::vector<int>, std::vector<int>> seen_key;
        seen_key.first.assign(player1->begin(), player1->end());
        seen_key.second.assign(player2->begin(), player2->end());

        if (seen->size() < depth) {
            seen->emplace_back();
            assert(seen->size == depth);
        }

        if (!(*seen)[depth - 1].insert(std::move(seen_key)).second) {
            return true;
        }

        int p1 = player1->front();
        player1->pop_front();
        int p2 = player2->front();
        player2->pop_front();

        if (p1 <= player1->size() && p2 <= player2->size()) {
            // Recursive combat
            std::deque<int> p1copy, p2copy;
            p1copy.assign(player1->begin(), player1->begin() + p1);
            p2copy.assign(player2->begin(), player2->begin() + p2);

            if (computeWinner(&p1copy, &p2copy, depth + 1, seen)) {
                player1->push_back(p1);
                player1->push_back(p2);
            } else {
                player2->push_back(p2);
                player2->push_back(p1);
            }
        } else {
            if (p1 < p2) {
                player2->push_back(p2);
                player2->push_back(p1);
            } else {
                assert(p1 != p2);
                assert(p2 < p1);

                player1->push_back(p1);
                player1->push_back(p2);
            }
        }
    }

    assert(player1->empty() || player2->empty());
    return !player1->empty();
}

int main(int argc, char **argv) {
    std::vector<std::string> lines =
        absl::StrSplit(readAll(), "\n\n", absl::SkipEmpty());
    assert(lines.size() == 2);

    std::deque<int> player1, player2;
    player1 = parsePlayer(lines[0]);
    player2 = parsePlayer(lines[1]);

    std::vector<absl::flat_hash_set<std::pair<std::vector<int>, std::vector<int>>>> seen;
    bool p1_winner = computeWinner(&player1, &player2, 1, &seen);

    int64_t score = 0;
    if (p1_winner) {
        printf("player 1: ");
        while (!player1.empty()) {
            printf("%d ", player1.front());
            score += player1.size() * player1.front();
            player1.pop_front();
        }
        printf("\n");
    } else {
        printf("player 2: ");
        while (!player2.empty()) {
            printf("%d ", player2.front());
            score += player2.size() * player2.front();
            player2.pop_front();
        }
        printf("\n");
    }

    printf("%ld\n", score);
}
