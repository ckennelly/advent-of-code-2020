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

    std::vector<std::pair<int, int>> ranges;

    for (absl::string_view section : absl::StrSplit(sections[0], '\n')) {
        if (section.empty()) { continue; }

        size_t pos = section.find(':');
        section = section.substr(pos + 1);

        unsigned lo1, hi1, lo2, hi2;

        // The backing store is null-terminated.
        sscanf(section.data(), "%u-%u or %u-%u\n", &lo1, &hi1, &lo2, &hi2);

        ranges.push_back(std::make_pair(lo1, hi1));
        ranges.push_back(std::make_pair(lo2, hi2));
    }

    assert(!ranges.empty());

    std::vector<int> values;
    for (absl::string_view line : absl::StrSplit(sections[2], '\n')) {
        if (line.empty()) {
            continue;
        }

        for (absl::string_view field : absl::StrSplit(line, ',')) {
            int value;
            if (absl::SimpleAtoi(field, &value)) {
                values.push_back(value);
            }
        }
    }

    int64_t invalid = 0;
    for (int i : values) {
        bool valid = false;
        for (auto& [lo, hi] : ranges) {
            if (lo <= i && i <= hi) {
                valid = true;
                break;
            }
        }

        if (valid) {
            continue;
        }

        invalid += i;
    }

    printf("%ld\n", invalid);
}
