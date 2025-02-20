#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#include "maskmanipulation.h"
#include "malloc.h"
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
	size_t tmp_TODO = size;
	printf("allocate unique_sone\n");
	(void)ignore;
	size += SIZE_CHUNK_HEADER;
	size /= sysconf(_SC_PAGESIZE);
	size += 1;
	size *= sysconf(_SC_PAGESIZE);
	printf("size will be %d --rounded-> %d\n", tmp_TODO, size);
	return call_mmap_and_set_header(size, to_init);
}

int allocate_memory_pool(void **to_init, size_t size, size_t ignore) {
	printf("allocate_memory_pool\n");
	(void)ignore;
	size += SIZE_CHUNK_HEADER;
	size *= 100;
	return call_mmap_and_set_header(size, to_init);
}

void *mmap_call(void *proximity, size_t length) {
	void *addr = mmap(proximity, length, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	printf("pre index\n");
	index_page(addr, length);
	printf("post index\n");
	return addr;
}

void *mmap_call_no_index(void *proximity, size_t length) {
	void *addr = mmap(proximity, length, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
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
