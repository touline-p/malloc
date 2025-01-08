#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#include <sys/mman.h>
#include <unistd.h>

#define ALLOCATION_GROUP_NUMBER 3

size_t size_allocation(size_t size);

void *mymalloc(size_t size) {
	uint64_t *top = arena_g.top;
	*top = size_allocation(size);

	arena_g.top = (void *)(*top + (void *)arena_g.top);
	toggle_mask(top, CHUNK_IN_USE);
	return top + OFFSET_HEADER;
}

void *mmap_call(void *addr, size_t length) {
	return mmap(addr, length, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

void *allocate_page(void *proximity) {
	arena_g.nb_alloc += 1;
	return mmap(proximity,
		sysconf(_SC_PAGE_SIZE),
		PROT_WRITE | PROT_READ,
		MAP_PRIVATE | MAP_ANONYMOUS,
		-1,
		0
	);
}
