#include <cassert>
#include <cstdio>
#include <cstring>

int main(int argc, char **argv) {
    int max_seat_id = -1;
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
        if (seat_id > max_seat_id) {
            max_seat_id = seat_id;
        }
    }

    printf("seat ID %d\n", max_seat_id);
}
