#include <stdbool.h>
#include <stdint.h>

bool mask_is_set(void *addr, uint64_t mask) {
	return *(uint64_t *)addr & mask;
}

void toggle_mask(void *addr, uint64_t mask) {
	*(uint64_t *)addr ^= mask;
}
