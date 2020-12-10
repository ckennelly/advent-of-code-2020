#include <cassert>
#include <cstdio>

#include "absl/strings/string_view.h"

int main(int argc, char **argv) {
    int low, hi;
    char ch;
    int passstart;
    char buf[256];
    int passend;

    int valid = 0;
    while (fscanf(stdin, "%d-%d %c: %n%255s%n", &low, &hi, &ch, &passstart, buf, &passend) == 4) {
        assert(passstart <= passend);
        absl::string_view password(buf, passend - passstart);

        int count = 0;
        size_t pos = 0;
        do {
            pos = password.find(ch, pos);
            if (pos == password.npos) {
                break;
            }

            pos++;
            count++;
        } while (true);

        const char* label = (low <= count && count <= hi) ? "VALID  " : "INVALID";
        printf("%s: %d-%d %c: %s\n", label, low, hi, ch, buf);

        if (low <= count && count <= hi) {
            valid++;
        }
    }

    printf("%d\n", valid);
}
