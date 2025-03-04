#include <stdbool.h>
#include <stdint.h>
#include "type.h"

bool mask_is_set(chunk_info_t *addr, uint64_t mask) {
	return addr->flags & mask;
}

void toggle_mask(chunk_info_t *addr, uint64_t mask) {
	addr->flags ^= mask;
}

size_t get_size(chunk_info_t *addr) {
	return addr->size;
}

void set_size(chunk_info_t *addr, size_t size) {
	addr->size = size;
}
