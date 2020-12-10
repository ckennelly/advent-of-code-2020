#include <cassert>
#include <cstdio>
#include <cstring>

#include "absl/container/btree_set.h"

int main(int argc, char **argv) {
    absl::btree_set<int> seats;

    char buf[128];
    while (fscanf(stdin, "%s", buf) == 1) {
        unsigned row = 0;
        assert(strlen(buf) == 10);
        for (int i = 0; i < 7; i++) {
            const char c = buf[i];
            assert(c == 'F' || c == 'B');
            if (c == 'B') {
                row |= (unsigned{1} << (6 - i));
            }
        }

        unsigned seat = 0;
        for (int i = 0; i < 3; i++) {
            const char c = buf[7 + i];
            assert(c == 'L' || c == 'R');
            if (c == 'R') {
                seat |= (unsigned{1} << (2 - i));
            }
        }

        int seat_id = row * 8 + seat;
        seats.insert(seat_id);
    }

    int last = INT_MIN;
    for (int seat : seats) {
        if (seat == last + 2) {
            printf("%d [%d] %d\n", last, last + 1, seat);
            break;
        }

        last = seat;
    }

}
