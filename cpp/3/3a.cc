#include <cstdio>
#include <vector>

struct Row {
    bool b[64];
};

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

    int trees = 0;
    for (int x = 3, y = 1; y < (int) rows.size(); x += 3, y++) {
        if (x >= size) {
            x -= size;
        }

        trees += rows[y].b[x] ? 1 : 0;
    }

    printf("%d\n", trees);
}
