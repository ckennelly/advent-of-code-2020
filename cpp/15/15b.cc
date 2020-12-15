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
    std::vector<int64_t> inputs = {20,9,11,0,1,2};

    std::vector<int64_t> numbers = inputs;
    absl::flat_hash_map<int64_t, std::pair<int64_t, int64_t>> lasts;
    for (int64_t i = 0; i < inputs.size(); i++) {
        lasts[inputs[i]].first = i + 1;
        lasts[inputs[i]].second = 0;
    }

    int64_t last = inputs.back();
    for (int64_t turn = inputs.size() + 1; turn <= 30000000; turn++) {
        auto it = lasts.find(last);
        int64_t next;
        if (it == lasts.end()) {
            // Number previously not spoken.
            next = 0;

            lasts[0].first = turn;
            lasts[0].second = -1;
        } else if (it->second.second <= 0) {
            // Number only spoken once.
            next = 0;

            lasts[0].second = lasts[0].first;
            lasts[0].first = turn;
        } else {
            // Number spoken before.  Next number is the difference.
            next = it->second.first - it->second.second;
            assert(next > 0);

            lasts[next].second = lasts[next].first;
            lasts[next].first = turn;
        }

        numbers.push_back(next);
        last = next;
    }

    absl::PrintF("%d\n", last);
}
