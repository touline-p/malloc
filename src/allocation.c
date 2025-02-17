#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#include "maskmanipulation.h"
#include "mymalloc.h"
#include "size.h"
#include "type.h"
#include "globals.h"


static bool call_mmap_and_set_header(
	size_t allocation_size,
	void **dst
);

int allocate_unique_zone(
	void **to_init,
	size_t ignore,
	size_t size
) {
	(void)ignore;
	size += SIZE_CHUNK_HEADER;
	size /= sysconf(_SC_PAGESIZE);
	size += 1;
	size *= sysconf(_SC_PAGESIZE);
	return call_mmap_and_set_header(size, to_init);
}

int allocate_memory_pool(void **to_init, size_t size, size_t ignore) {
	printf("allocate_memory_pool\n");
	(void)ignore;
	size += SIZE_CHUNK_HEADER;
	size *= 100;
	return call_mmap_and_set_header(size, to_init);
}

void stock_and_reinit(
	void **to_stock,
	void **to_init,
	size_t size
) {
	//** TODO multiple end of page going to the free must be put in chain list **//
	allocate_memory_pool(to_init, size, IGNORE_ARGUMENTS);
}

void *mmap_call(void *proximity, size_t length) {
	void *addr = mmap(proximity, length, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	printf("pre index\n");
	index_page(addr, length);
	printf("post index\n");
	return addr;
}

static bool call_mmap_and_set_header(
	size_t allocation_size,
	void **dst
) {
	chunk_info_t *chunk;

	printf("pre mmap_call\n");
	*dst = mmap_call(NULL, allocation_size);
	printf("post mmap_call\n");
	if (*dst == (void *)-1)
		return false;
	chunk = *dst;

	bzero(chunk, allocation_size);
	*chunk = (chunk_info_t)allocation_size;
	toggle_mask(chunk, FIRST_IN_ZONE);
	return true;
}
