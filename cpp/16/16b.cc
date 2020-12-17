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
    std::string lines = readAll();
    std::vector<std::string> sections = absl::StrSplit(lines, "\n\n");

    std::vector<std::vector<std::pair<int, int>>> ranges;
    absl::flat_hash_set<int> departures;

    for (absl::string_view section : absl::StrSplit(sections[0], '\n')) {
        if (section.empty()) { continue; }

        size_t pos = section.find(':');
        bool is_departure = absl::StartsWith(section, "departure ");

        section = section.substr(pos + 1);

        unsigned lo1, hi1, lo2, hi2;

        // The backing store is null-terminated.
        sscanf(section.data(), "%u-%u or %u-%u\n", &lo1, &hi1, &lo2, &hi2);

        ranges.emplace_back();
        ranges.back().push_back(std::make_pair(lo1, hi1));
        ranges.back().push_back(std::make_pair(lo2, hi2));

        if (is_departure) {
            departures.insert(ranges.size() - 1);
        }
    }

    assert(!ranges.empty());

    std::vector<absl::flat_hash_set<int>> columns;

    std::vector<int> values;
    for (absl::string_view line : absl::StrSplit(sections[2], '\n')) {
        if (line.empty() ){
            continue;
        }

        std::vector<int> row;

        for (absl::string_view field : absl::StrSplit(line, ',')) {
            int value;
            if (absl::SimpleAtoi(field, &value)) {
                row.push_back(value);
            }
        }

        if (row.empty()) {
            continue;
        }

        if (columns.empty()) {
            columns.resize(row.size());
            for (int i = 0; i < row.size(); i++) {
                for (int j = 0; j < ranges.size(); j++) {
                    columns[i].insert(j);
                }
            }
            assert(!columns.empty());
        }

        bool valid = true;
        for (int i : row) {
            bool cell_valid = false;
            for (auto& pairs : ranges) {
                for (auto& [lo, hi] : pairs) {
                    if (lo <= i && i <= hi) {
                        cell_valid = true;
                        break;
                    }
                }
            }

            if (!cell_valid) {
                valid = false;
                break;
            }
        }

        if (!valid) {
            continue;
        }

        for (int col = 0; col < row.size(); col++) {
            const int value = row[col];

            for (auto it = columns[col].begin(); it != columns[col].end(); ) {
                const int candidate = *it;
                bool valid = false;
                for (const auto& [lo, hi] : ranges[candidate]) {
                    if (lo <= value && value <= hi) {
                        valid = true;
                        break;
                    }
                }

                if (!valid) {
                    columns[col].erase(it++);
                } else {
                    ++it;
                }
            }
        }
    }

    bool reduced;
    do {
        reduced = false;
        for (int col = 0; col < columns.size(); col++) {
            if (columns[col].size() == 1) {
                const int candidate = *columns[col].begin();
                for (int i = 0; i < columns.size(); i++) {
                    if (i == col) {
                        continue;
                    }

                    if (columns[i].contains(candidate)) {
                        columns[i].erase(candidate);
                        reduced = true;
                    }
                }
            }
        }
    } while (reduced);

    int64_t product = 1;
    for (absl::string_view line : absl::StrSplit(sections[1], '\n')) {
        if (line.empty()){
            continue;
        } else if (line == "your ticket:") {
            continue;
        }

        std::vector<int> row;

        for (absl::string_view field : absl::StrSplit(line, ',')) {
            int value;
            if (absl::SimpleAtoi(field, &value)) {
                row.push_back(value);
            }
        }

        if (row.empty()) {
            continue;
        }

        assert(row.size() == columns.size());

        int chunks = 0;
        for (int i = 0; i < columns.size(); i++) {
            if (columns[i].empty()) {
                continue;
            }
            int column = *columns[i].begin();
            if (!departures.contains(column)) {
                continue;
            }

            assert(columns[i].size() == 1);
            product *= row[i];
            chunks++;
        }
        assert(chunks == departures.size());
    }

    printf("%ld\n", product);
}
