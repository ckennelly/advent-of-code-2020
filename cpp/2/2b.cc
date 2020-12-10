#include <cassert>
#include <cstdio>

#include "absl/strings/string_view.h"

int main(int argc, char **argv) {
    int lo, hi;
    char ch;
    int passstart;
    char buf[256];
    int passend;

    int valid = 0;
    while (fscanf(stdin, "%d-%d %c: %n%255s%n", &lo, &hi, &ch, &passstart, buf, &passend) == 4) {
        assert(passstart <= passend);
        absl::string_view password(buf, passend - passstart);

        lo--;
        hi--;

        bool lo_good = password.size() > lo && password[lo] == ch;
        bool hi_good = password.size() > hi && password[hi] == ch;
        bool is_valid = lo_good ^ hi_good;

        const char* label = is_valid ? "VALID  " : "INVALID";
        printf("%s: %d-%d %c: %s\n", label, lo, hi, ch, buf);

        if (is_valid) {
            valid++;
        }
    }

    printf("%d\n", valid);
}
