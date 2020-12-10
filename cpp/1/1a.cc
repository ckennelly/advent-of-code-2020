#include "absl/container/flat_hash_set.h"

int main(int argc, char **argv) {
	absl::flat_hash_set<int> values;
	int value;
	while (fscanf(stdin, "%d", &value) == 1) {
		values.insert(value);
	}

	const int kTarget = 2020;
    bool found = false;
	int a, b;
	for (auto entry : values) {
		if (values.contains(kTarget - entry)) {
            found = true;
			a = entry;
			b = kTarget - entry;
			break;
		}
	}

	if (!found) {
        printf("not found!\n");
		return 1;
	}

	printf("%d\n", a * b);
}
