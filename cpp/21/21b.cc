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

struct Records {
    int seen;
    std::map<std::string, int> allergies;
};

struct Parsed {
    absl::flat_hash_set<std::string> ingredients;
    absl::flat_hash_set<std::string> allergies;
};

int main(int argc, char **argv) {
    std::vector<std::string> lines =
        absl::StrSplit(readAll(), "\n", absl::SkipEmpty());

    std::vector<Parsed> parsed;
    absl::flat_hash_set<std::string> ingredients, allergies;
    absl::flat_hash_map<std::string, int> seen;

    for (const auto& line : lines) {
        const size_t pos = line.find('(');
        std::string raw_ingredients, raw_allergies;
        if (pos == std::string::npos) {
            raw_ingredients = line;
        } else {
            raw_ingredients = line.substr(0, pos);
            const size_t offset = pos + strlen("(contains ");
            raw_allergies = line.substr(offset, line.size() - offset - 1);
        }

        Parsed p;
        p.ingredients = absl::StrSplit(raw_ingredients, ' ', absl::SkipEmpty());
        p.allergies = absl::StrSplit(raw_allergies, ", ", absl::SkipEmpty());

        ingredients.insert(p.ingredients.begin(), p.ingredients.end());
        allergies.insert(p.allergies.begin(), p.allergies.end());

        for (const auto& ingredient : p.ingredients) {
            seen[ingredient]++;
        }

        parsed.push_back(std::move(p));
    }

    absl::flat_hash_map<std::string, absl::flat_hash_set<std::string>> possible;
    for (const auto& allergy : allergies) {
        possible[allergy] = ingredients;
    }

    for (const auto& parse : parsed) {
        for (const auto& allergy : parse.allergies) {
            for (const auto& ingredient : ingredients) {
                if (!parse.ingredients.contains(ingredient)) {
                    possible[allergy].erase(ingredient);
                }
            }
        }
    }

    std::set<std::string> known_allergies;
    while (known_allergies.size() < allergies.size()) {
        for (const auto& allergy : allergies) {
            const auto& candidate_ingredients = possible[allergy];
            if (candidate_ingredients.size() == 1 &&
                    known_allergies.insert(allergy).second) {
                for (auto& [lhs, rhs] : possible) {
                    if (lhs == allergy) {
                        continue;
                    }

                    rhs.erase(*candidate_ingredients.begin());
                }
            }
        }
    }

    std::vector<std::string> known_ingredients;
    for (const auto& allergy : known_allergies) {
        known_ingredients.push_back(*possible[allergy].begin());
    }

    absl::PrintF("%s", absl::StrJoin(known_ingredients, ","));
}
