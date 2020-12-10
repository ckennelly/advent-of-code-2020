#include <cstdio>
#include <cstdint>
#include <vector>

#include "absl/types/span.h"

struct Row {
    bool b[64];
};

int64_t DoSlope(absl::Span<const Row> rows, const int width, int dx, int dy) {
    int trees = 0;
    for (int x = dx, y = dy; y < (int) rows.size(); x += dx, y += dy) {
        if (x >= width) {
            x -= width;
        }

        trees += rows[y].b[x] ? 1 : 0;
    }

    return trees;
}

int main(int argc, char **argv) {
    char buf[65];
    int size;

    std::vector<Row> rows;
    while (fscanf(stdin, "%64s%n", buf, &size) == 1) {
        rows.emplace_back();
        Row& r = rows.back();

        for (int i = 0; i < size; i++) {
            r.b[i] = buf[i] == '#';
        }
    }

    // lines include the end
    size--;

    int64_t run_a = DoSlope(rows, size, 1, 1);
    int64_t run_b = DoSlope(rows, size, 3, 1);
    int64_t run_c = DoSlope(rows, size, 5, 1);
    int64_t run_d = DoSlope(rows, size, 7, 1);
    int64_t run_e = DoSlope(rows, size, 1, 2);

    printf("%ld\n", run_a * run_b * run_c * run_d * run_e);
}
