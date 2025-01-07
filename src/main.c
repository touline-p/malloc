#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#include <sys/mman.h>
#include <unistd.h>

#define ALLOCATION_GROUP_NUMBER 3


void *mymalloc(size_t size) {
	return arena_g.top;
}

void *mmap_call(void *addr, size_t length) {
	return mmap(addr, length, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}


void *allocate_page(void *proximity) {
	return mmap(proximity,
		sysconf(_SC_PAGE_SIZE),
		PROT_WRITE | PROT_READ,
		MAP_PRIVATE | MAP_ANONYMOUS,
		-1,
		0
	);
}
