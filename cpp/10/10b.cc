#include <algorithm>
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
    std::string input = readAll();

    std::vector<int> ints;

    int highest = INT_MIN;
    for (absl::string_view line : absl::StrSplit(input, '\n')) {
        if (line.empty()) {
            break;
        }

        int v;
        absl::SimpleAtoi(line, &v);
        ints.push_back(v);

        highest = std::max(highest, v);
    }

    std::sort(ints.begin(), ints.end());

    absl::flat_hash_map<int, int64_t> ways;
    ways[0] = 1;

    for (int i = 0; i < ints.size(); i++) {
        int value = ints[i];

        int64_t sum = 0;
        for (int k = 0; k <= 3; k++) {
            if (value - k < 0) {
                continue;
            }

            sum += ways[value - k];
        }

        assert(sum >= 0);
        ways[value] = sum;
    }

   
    printf("%ld", ways[highest]);
}
