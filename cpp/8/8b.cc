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


    enum type {
        acc,
        jmp,
        nop,
    };



struct instruction {
    type op;
    int offset;
};

int main(int argc, char **argv) {
    std::string input = readAll();

    std::vector<instruction> instructions;

    for (absl::string_view line : absl::StrSplit(input, '\n')) {
        if (line.empty()) {
            break;
        }
        char buf[256];
        int offset;

        sscanf(line.data(), "%s %d", buf, &offset);

        instruction i;
        if (buf == absl::string_view("nop")) {
            i.op = nop;
        } else if (buf == absl::string_view("acc")) {
            i.op = acc;
        } else if (buf == absl::string_view("jmp")) {
            i.op = jmp;
        } else {
            assert(false);
        }
        i.offset = offset;
        instructions.push_back(i);
    }

    printf("%d\n", instructions.size());

    for (int i = 0; i < instructions.size(); i++) {
        if (instructions[i].op == acc) {
            continue;
        }

        if (instructions[i].op == jmp) {
            instructions[i].op = nop;
        } else {
            instructions[i].op = jmp;
        }

        int accumulator = 0;
        absl::flat_hash_set<int> visited;

        int rip = 0;
        bool infinite = false;
        do {
            if (!visited.insert(rip).second) {
                // Infinite loop
                infinite = true;
                break;
            }

            switch (instructions[rip].op) {
                case jmp:
                    rip += instructions[rip].offset;
                    break;
                case acc:
                    accumulator += instructions[rip].offset;
                    rip++;
                    break;
                case nop:
                    rip++;
                    break;
                default:
                    break;
            }
        } while (rip < instructions.size());

        // Put back
        if (instructions[i].op == jmp) {
            instructions[i].op = nop;
        } else {
            instructions[i].op = jmp;
        }

        if (!infinite) {
            printf("%d\n", accumulator);
            return 0;
        }
    }

    assert(false);
}
