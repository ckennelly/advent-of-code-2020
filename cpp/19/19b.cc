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

std::string constructRE(absl::flat_hash_map<int, std::string>& rules, int rule) {
    auto it = rules.find(rule);
    assert(it != rules.end());
    const std::string& raw_rule = it->second;

    if (raw_rule == "\"a\"") {
        printf("rule %d: a\n", rule);
        return "a";
    } else if (raw_rule == "\"b\"") {
        printf("rule %d: b\n", rule);
        return "b";
    } else if (rule == 8) {
        // raw[8] = "42 | 42 8";
        std::string ret = absl::StrCat("(", constructRE(rules, 42), ")+");
        printf("rule %d: %s\n", rule, ret.c_str());
        return ret;
    } else if (rule == 11) {
        // raw[11] = "42 31 | 42 11 31";
        auto r42 = constructRE(rules, 42);
        auto r31 = constructRE(rules, 31);

        // Rather than use a look ahead expression...
        std::vector<std::string> terms;
        for (int i = 1; i < 8; i++) {
            std::string first, second;
            for (int j = 0; j < i; j++) {
                absl::StrAppend(&first, r42);
                absl::StrAppend(&second, r31);
            }
            terms.push_back(absl::StrCat("(", first, second, ")"));
        }

        std::string ret = absl::StrCat("(", absl::StrJoin(terms, "|"), ")");
        printf("rule %d: %s\n", rule, ret.c_str());
        return ret;
    } else if (absl::StrContains(raw_rule, " | ")) {
        std::vector<std::string> parts = absl::StrSplit(raw_rule, "|");

        std::vector<std::string> subREs;
        for (const auto& part : parts) {
            std::string subRE;
            std::vector<std::string> terms = absl::StrSplit(part, " ", absl::SkipEmpty());
            for (const std::string& term : terms) {
                int subrule;
                if (absl::SimpleAtoi(term, &subrule)) {
                    absl::StrAppend(&subRE, constructRE(rules, subrule));
                } else {
                    assert(false);
                }
            }

            subREs.emplace_back(std::move(subRE));
        }

        std::string ret = absl::StrCat("(", absl::StrJoin(subREs, "|"), ")");
        printf("rule %d: %s\n", rule, ret.c_str());
        return ret;
    } else {
        std::string subRE;
        std::vector<std::string> terms = absl::StrSplit(raw_rule, " ", absl::SkipEmpty());
        for (const std::string& term : terms) {
            int subrule;
            if (absl::SimpleAtoi(term, &subrule)) {
                absl::StrAppend(&subRE, constructRE(rules, subrule));
            } else {
                assert(false);
            }
        }

        std::string ret = absl::StrCat("(", subRE, ")");
        printf("rule %d: %s\n", rule, ret.c_str());
        return ret;
    }
}

int main(int argc, char **argv) {
    std::vector<std::string> lines =
        absl::StrSplit(readAll(), "\n\n", absl::SkipEmpty());

    std::vector<std::string> rules =
        absl::StrSplit(lines[0], '\n', absl::SkipEmpty());
    std::vector<std::string> inputs =
        absl::StrSplit(lines[1], '\n', absl::SkipEmpty());

    absl::flat_hash_map<int, std::string> raw;
    for (const auto& rule : rules) {
        int number;
        int index;
        sscanf(rule.c_str(), "%d: %n", &number, &index);

        bool success = raw.emplace(number, rule.substr(index)).second;
        (void) success;
        assert(success);
    }

    raw[8] = "42 | 42 8";
    raw[11] = "42 31 | 42 11 31";

    std::string RE = constructRE(raw, 0);
    printf("re: %s\n", RE.c_str());
    std::regex cregex(RE);
    int matches = 0;
    for (const auto& input : inputs) {
        if (std::regex_match(input, cregex)) {
            printf("   match: %s\n", input.c_str());
            matches++;
        } else {
            printf("no match: %s\n", input.c_str());
        }
    }

    printf("%d\n", matches);
}
