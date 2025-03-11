#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

void set_mask(chunk_info_t *addr, size_t mask, bool dst) {
	if (mask_is_set(addr, mask) != dst)
		toggle_mask(addr, mask);
}

chunk_info_t *get_header_from_addr(void *addr)
{
	return (((chunk_info_t *)addr) - 1);
}

void *get_addr_from_header(chunk_info_t *freed_chunk) 
{
	return freed_chunk + 1;
}
