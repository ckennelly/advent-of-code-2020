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
    std::vector<absl::optional<int64_t>> stack;
    stack.resize(1);

    std::vector<absl::optional<Op>> ops;
    ops.resize(1);

    for (int i = 0; i < line.size(); i++) {
        const char c = line[i];
        if (c >= '0' && c <= '9') {
            auto& top = stack.back();
            auto& top_op = ops.back();

            if (top_op.has_value()) {
                // Pending operation, process it.
                assert(top.has_value());

                switch (*top_op) {
                    case Add:
                        top = *top + (c - '0');
                        break;
                    case Multiply:
                        top = *top * (c - '0');
                        break;
                    default:
                        assert(false);
                }

                top_op.reset();
            } else {
                assert(!top.has_value());

                top = (c - '0');
            }
        } else if (c == ' ') {
            continue;
        } else if (c == '+') {
            auto& top_op = ops.back();
            assert(!top_op.has_value());

            top_op = Add;
        } else if (c == '*') {
            auto& top_op = ops.back();
            assert(!top_op.has_value());

            top_op = Multiply;
        } else if (c == '(') {
            stack.push_back(absl::nullopt);
            ops.push_back(absl::nullopt);
        } else if (c == ')') {
            assert(!ops.back().has_value());
            int64_t last = *stack.back();

            stack.pop_back();
            ops.pop_back();

            auto& top = stack.back();
            auto& top_op = ops.back();

            // Merge the result of the subexpression into the outermost
            // expression
            if (top_op.has_value()) {
                assert(top.has_value());

                switch (*top_op) {
                    case Add:
                        top = *top + last;
                        break;
                    case Multiply:
                        top = *top * last;
                        break;
                    default:
                        assert(false);
                }

                top_op.reset();
            } else {
                assert(!top.has_value());
                top = last;
            }
        } else {
            assert(false);
        }
    }

    assert(stack.size() == 1);
    assert(ops.size() == 1);
    assert(stack.back().has_value());

    return *stack.back();
}

int main(int argc, char **argv) {
    std::vector<std::string> lines =
        absl::StrSplit(readAll(), '\n', absl::SkipEmpty());

    assert(computeLine("2 * 3 + (4 * 5)") == 26);
    assert(computeLine("5 + (8 * 3 + 9 + 3 * 4 * 3)") == 437);
    assert(computeLine("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))") == 12240);
    assert(computeLine("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2") == 13632);

    int64_t result = 0;
    for (const auto& line : lines) {
        result += computeLine(line);
    }

    printf("%ld\n", result);
}
