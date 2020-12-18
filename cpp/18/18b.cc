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
#include "absl/types/optional.h"

std::string readAll() {
    std::stringstream buffer;
    buffer << std::cin.rdbuf();
    return buffer.str();
}

enum Op {
    Add,
    Multiply,
};

int64_t computeLine(absl::string_view line) {
    // The inner vector is all of the deferred multiplications.
    std::vector<std::vector<int64_t>> stack;
    stack.resize(1);

    std::vector<absl::optional<Op>> ops;
    ops.resize(1);

    for (int i = 0; i < line.size(); i++) {
        const char c = line[i];
        if (c >= '0' && c <= '9') {
            auto& top_op = ops.back();

            if (top_op.has_value() && *top_op == Add) {
                // Merge add immediately.
                assert(!stack.back().empty());
                auto& top = stack.back().back();
                top = top + (c - '0');
            } else {
                // Defer
                stack.back().push_back(c - '0');
            }
        } else if (c == ' ') {
            continue;
        } else if (c == '+') {
            ops.back() = Add;
        } else if (c == '*') {
            ops.back() = Multiply;
        } else if (c == '(') {
            stack.emplace_back();
            ops.push_back(absl::nullopt);
        } else if (c == ')') {
            int64_t last = 1;
            // Merge all of the subexpression components.  They are
            // multiplication.
            assert(!stack.back().empty());
            for (auto value : stack.back()) {
                last *= value;
            }

            stack.pop_back();
            ops.pop_back();

            auto& top = stack.back().back();
            auto& top_op = ops.back();

            if (top_op.has_value() && *top_op == Add) {
                top = top + last;
            } else {
                stack.back().push_back(last);
            }
        } else {
            assert(false);
        }
    }

    assert(stack.size() == 1);
    assert(ops.size() == 1);
    assert(!stack.back().empty());

    int64_t v = 1;
    for (auto value : stack.back()) {
        v *= value;
    }

    return v;
}

int main(int argc, char **argv) {
    std::vector<std::string> lines =
        absl::StrSplit(readAll(), '\n', absl::SkipEmpty());

    assert(computeLine("1 + (2 * 3) + (4 * (5 + 6))") == 51);
    assert(computeLine("2 * 3 + (4 * 5)") == 46);
    assert(computeLine("5 + (8 * 3 + 9 + 3 * 4 * 3)") == 1445);
    assert(computeLine("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))") == 669060);
    assert(computeLine("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2") == 23340);

    int64_t result = 0;
    for (const auto& line : lines) {
        result += computeLine(line);
    }

    printf("%ld\n", result);
}
