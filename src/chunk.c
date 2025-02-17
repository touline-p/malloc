#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>

#include "maskmanipulation.h"
#include "size.h"
#include "mymalloc.h"

size_t size_allocation(size_t size) {
	size -= 1;
	size = size >> 4;
	size += 1;
	size = size << 4;
	return size;
}

uint64_t *get_header_from_addr(uint64_t *header) {
	return header - OFFSET_HEADER;
}

uint64_t *get_addr_from_header(uint64_t *addr) {
	return addr + OFFSET_HEADER;
}
