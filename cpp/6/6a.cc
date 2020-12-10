#include <cstdio>
#include <cstring>

#include "absl/algorithm/container.h"
#include "absl/strings/ascii.h"
#include "absl/strings/match.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"
#include "absl/strings/strip.h"

struct Group {
    Group() {
        memset(answers, 0, sizeof(answers));
    }

    bool answers[26];

    void ParseLine(absl::string_view sv) {
        for (char c : sv) {
            if (!(c >= 'a' && c <= 'z')) {
                continue;
            }

            answers[c - 'a'] = true;
        }
    }

    int Count() const {
        int x = 0;
        for (bool a : answers) {
            x += a;
        }
        return x;
    }
};

int main(int argc, char **argv) {
    char buf[257];

    int counts = 0;
    bool next = true;
    Group g;
    do {
        char* fret = fgets(&buf[0], 256, stdin);
        if (fret == nullptr) {
            break;
        }

        size_t len = strlen(buf);
        if (len == 0 || (len == 1 && buf[0] == '\n')) {
            if (!next) {
                counts += g.Count();
            }

            next = true;
            g = Group();
            continue;
        }

        g.ParseLine(buf);
        next = false;
    } while (true);

    if (!next) {
        counts += g.Count();
    }

    printf("%d", counts);
}
