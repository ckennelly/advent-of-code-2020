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
    ints.push_back(0);

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
    
    std::vector<int> diffs;
    for (int i = 0; i < ints.size() - 1; i++) {
        diffs.push_back(ints[i+1]-ints[i]);
    }
    diffs.push_back(3);

    int count_1 = 0;
    int count_3 = 0;

    for (int i : diffs) {
        switch (i) {
            case 1:
                count_1++;
                break;
            case 3:
                count_3++;
                break;
        }
    }

    printf("%d", count_1*count_3);
}
