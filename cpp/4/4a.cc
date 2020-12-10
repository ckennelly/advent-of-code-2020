#include <cstdio>

#include "absl/strings/string_view.h"

struct Validation {
    bool byr = false;
    bool iyr = false;
    bool eyr = false;
    bool hgt = false;
    bool hcl = false;
    bool ecl = false;
    bool pid = false;
    bool cid = false;

    void ParseLine(absl::string_view sv);

    bool IsValid() const {
        return byr && iyr && eyr && hgt && hcl && ecl && pid;
    }
};

void Validation::ParseLine(absl::string_view sv) {
    for (size_t pos = 0; pos < sv.size(); pos++) {
        size_t next = sv.find(' ', pos);
        if (next == absl::string_view::npos) {
            next = sv.size();
        }

        absl::string_view entry = sv.substr(pos, next - pos);
        size_t colon = entry.find(':');
        assert(colon == 3);
        absl::string_view key = entry.substr(0, colon);
        if (key == "byr") {
            byr = true;
        } else if (key == "iyr") {
            iyr = true;
        } else if (key == "eyr") {
            eyr = true;
        } else if (key == "hgt") {
            hgt = true;
        } else if (key == "hcl") {
            hcl = true;
        } else if (key == "ecl") {
            ecl = true;
        } else if (key == "pid") {
            pid = true;
        } else if (key == "cid") {
            cid = true;
        } else {
            assert(false);
        }

        pos = next;
    }

}

int main(int argc, char **argv) {
    char buf[257];

    int valid = 0;
    bool next = true;
    Validation v;
    do {
        char* fret = fgets(&buf[0], 256, stdin);
        if (fret == nullptr) {
            break;
        }

        size_t len = strlen(buf);
        if (len == 0 || (len == 1 && buf[0] == '\n')) {
            if (!next) {
                valid += v.IsValid();
            }

            next = true;
            v = Validation();
            continue;
        }

        v.ParseLine(buf);
        next = false;
    } while (true);

    if (!next) {
        valid += v.IsValid();
    }

    printf("%d", valid);
}
