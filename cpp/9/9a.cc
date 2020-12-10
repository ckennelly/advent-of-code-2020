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

    std::vector<int64_t> v;

    for (absl::string_view line : absl::StrSplit(input, '\n')) {
        if (line.empty()) {
            break;
        }

        int64_t d;
        sscanf(line.data(), "%lld", &d);
        v.push_back(d);
    }

    for (int i = 25; i < v.size(); i++) {
        const int64_t current = v[i];

        absl::flat_hash_set<int64_t> past;
        for (int j = i - 25; j < i; j++) {
            assert(past.insert(v[j]).second);
        }
        bool found = false;
        for (int j = i - 25; j < i; j++) {
            if (past.contains(current - v[j])) {
                found = true;
                break;
            }
        }

        if (!found) {
            printf("%lld\n", current);
            break;
        }
    }

}
