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

static void *mmap_call(void *proximity, size_t length);

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
	printf("allocating a new memory pool\n");
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

static void *mmap_call(void *proximity, size_t length) {
	void *addr = mmap(proximity, length, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	printf("mmap was called\n");
	pages[allocated_page].addr = addr;
	pages[allocated_page].size = length;
	allocated_page += 1;
	return addr;
}

static bool call_mmap_and_set_header(
	size_t allocation_size,
	void **dst
) {
	chunk_info_t *chunk;

	*dst = mmap_call(NULL, allocation_size);
	if (*dst == (void *)-1)
		return false;
	chunk = *dst;

	bzero(chunk, allocation_size);
	*chunk = (chunk_info_t)allocation_size;
	printf("size in mmap %ld\n", (size_t)GET_SIZE(*dst));
	toggle_mask(chunk, FIRST_IN_ZONE);
	return true;
}
