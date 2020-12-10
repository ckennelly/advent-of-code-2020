#include <stdint.h>

#include "absl/container/flat_hash_set.h"
#include "absl/strings/str_format.h"

int main(int argc, char **argv) {
	absl::flat_hash_set<int> values;
	int value;
	while (fscanf(stdin, "%d", &value) == 1) {
		values.insert(value);
	}

	const int kTarget = 2020;
    bool found = false;
	int64_t a, b, c;
	for (auto entry_a : values) {
        for (auto entry_b : values) {
            if (values.contains(kTarget - entry_a - entry_b)) {
                found = true;
                a = entry_a;
                b = entry_b;
                c = kTarget - entry_a - entry_b;
                break;
            }
        }

        if (found) {
            break;
        }
	}

	if (!found) {
        printf("not found!\n");
		return 1;
	}

	absl::PrintF("%lld\n", a * b * c);
}
