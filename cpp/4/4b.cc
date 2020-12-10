#include <cstdio>

#include "absl/algorithm/container.h"
#include "absl/strings/ascii.h"
#include "absl/strings/match.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"
#include "absl/strings/strip.h"

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
    sv = absl::StripAsciiWhitespace(sv);

    for (size_t pos = 0; pos < sv.size(); pos++) {
        size_t next = sv.find(' ', pos);
        if (next == absl::string_view::npos) {
            next = sv.size();
        }

        absl::string_view entry = sv.substr(pos, next - pos);
        size_t colon = entry.find(':');
        assert(colon == 3);
        absl::string_view key = entry.substr(0, colon);
        absl::string_view value = entry.substr(colon + 1);
        if (key == "byr") {
            // byr (Birth Year) - four digits; at least 1920 and at most 2002.
            int year;
            if (absl::SimpleAtoi(value, &year) && year >= 1920 && year <= 2002) {
                byr = true;
            }
        } else if (key == "iyr") {
            // iyr (Issue Year) - four digits; at least 2010 and at most 2020.
            int year;
            if (absl::SimpleAtoi(value, &year) && year >= 2010 && year <= 2020) {
                iyr = true;
            }
        } else if (key == "eyr") {
            // eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
            int year;
            if (absl::SimpleAtoi(value, &year) && year >= 2020 && year <= 2030) {
                eyr = true;
            }
        } else if (key == "hgt") {
            // hgt (Height) - a number followed by either cm or in:
            //   If cm, the number must be at least 150 and at most 193.
            //   If in, the number must be at least 59 and at most 76.
            if (absl::EndsWith(value, "cm")) {
                value = absl::StripSuffix(value, "cm");

                int height;
                if (absl::SimpleAtoi(value, &height) && height >= 150 && height <= 193) {
                    hgt = true;
                }
            } else if (absl::EndsWith(value, "in")) {
                value = absl::StripSuffix(value, "in");

                int height;
                if (absl::SimpleAtoi(value, &height) && height >= 59 && height <= 76) {
                    hgt = true;
                }
            }
        } else if (key == "hcl") {
            // hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
            auto is_hex = [](char c) {
                return ('0' <= c && c <= '9') ||
                       ('a' <= c && c <= 'f');
            };
            if (value.size() == 7 &&
                value[0] == '#' &&
                is_hex(value[1]) && is_hex(value[2]) && is_hex(value[3]) &&
                is_hex(value[4]) && is_hex(value[5]) && is_hex(value[6])) {
                hcl = true;
            }
        } else if (key == "ecl") {
            // ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
            if (value == "amb" || value == "blu" || value == "brn" ||
                value == "gry" || value == "grn" || value == "hzl" ||
                value == "oth") {
                ecl = true;
            }
        } else if (key == "pid") {
            // pid (Passport ID) - a nine-digit number, including leading zeroes.
            if (value.size() == 9 && absl::c_all_of(value, absl::ascii_isdigit)) {
                pid = true;
            }
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
