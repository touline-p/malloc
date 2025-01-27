#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>

#include "size.h"
#include "mymalloc.h"
#include "globals.h"

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

void *mmap_call(void *proximity, size_t length) {
	void *addr = mmap(proximity, length, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	printf("ptr allocated : %p\n", addr);
	pages[allocated_page].addr = addr;
	pages[allocated_page].size = length;
	allocated_page += 1;
	return addr;
}

void stock_and_reinit(void **to_stock, void **to_init, size_t size) {
	if (*to_stock == NULL) {
		*to_stock = *to_init;
	}

	try_init_page(to_init, size);
}
