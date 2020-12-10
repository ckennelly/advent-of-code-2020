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

struct Parsed {
    int number;
    std::string color;
};

int main(int argc, char **argv) {
    absl::flat_hash_map<std::string, std::set<std::string>> containers;    

    std::string input = readAll();
    for (absl::string_view line : absl::StrSplit(input, '\n')) {
        if (line[line.size() - 1] == '.') {
            line = line.substr(0, line.size() - 1);
        }

        if (line.empty()) {
            continue;
        }

        std::vector<std::string> parts = absl::StrSplit(line, " contain ");
        assert(parts.size() == 2);

        std::string container = parts[0];
        container = absl::StripSuffix(container, " bags");
        container = absl::StripSuffix(container, " bag");
        container = absl::StripAsciiWhitespace(container);

        std::vector<std::string> contained;
        if (parts[1] != "no other bags") {
            contained = absl::StrSplit(parts[1], ',');
        }

        std::map<std::string, int> parsed;
        for (auto& l : contained) {
            l = absl::StripSuffix(l, " bags");
            l = absl::StripSuffix(l, " bag");
            l = absl::StripAsciiWhitespace(l);

            Parsed p;
            int bytes;
            sscanf(l.c_str(), "%d%n", &p.number, &bytes);
            p.color = l.data() + bytes + 1;

            parsed.emplace(p.color, p.number);
        }

        for (auto& [l, r] : parsed) {
            containers[l].insert(container);
        }
    }

    std::vector<std::string> to_search;
    {
        to_search.emplace_back("shiny gold");
    }

    absl::flat_hash_set<std::string> visited;

    while (!to_search.empty()) {
        std::string c = to_search.back();
        to_search.pop_back();
        visited.insert(c);

        auto it = containers.find(c);
        if (it == containers.end()) {
            continue;
        }

        for (auto& s : it->second) {
            to_search.emplace_back(s);
        }
    }

    printf("%zu\n", visited.size() - 1);

}
