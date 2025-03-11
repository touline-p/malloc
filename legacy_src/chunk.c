#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>

#include "maskmanipulation.h"
#include "size.h"
#include "malloc.h"

size_t size_allocation(size_t size) {
	size -= 1;
	size = size >> 4;
	size += 1;
	size = size << 4;
	return size;
}

uint64_t *get_header_from_addr(void *header) {
	return header - SIZE_CHUNK_HEADER;
}

uint64_t *get_addr_from_header(void *addr) {
	return addr + SIZE_CHUNK_HEADER;
}
