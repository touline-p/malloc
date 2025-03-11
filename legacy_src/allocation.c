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
	ft_printf("allocate_unique_zone\n");
	size_t tmp_TODO = size;
	(void)ignore;
	size += SIZE_CHUNK_HEADER;
	size /= sysconf(_SC_PAGESIZE);
	size += 1;
	size *= sysconf(_SC_PAGESIZE);
	ft_printf("for %d i alloc %d\n", tmp_TODO, size);
	return call_mmap_and_set_header(size, to_init);
}

int allocate_memory_pool(void **to_init, size_t size, size_t ignore) {
	(void)ignore;
	ft_printf("pre proc size %d\n", size);
	size += SIZE_CHUNK_HEADER;
	ft_printf("size + CHUNK_HEADER %d\n", size);
	size *= 100;
	ft_printf("memory_pool is of size -> %d\n",size);
	return call_mmap_and_set_header(size, to_init);
}

void *mmap_call(void *proximity, size_t length) {
	ft_printf("calling mmap\n");
	void *addr = mmap(proximity, length, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	ft_printf("return addr from mmap %p - indexing\n", addr);
	index_page(addr, length);
	return addr;
}

static bool call_mmap_and_set_header(
	size_t allocation_size,
	void **dst
) {
	chunk_info_t *chunk;

	*dst = mmap_call(NULL, allocation_size);
	ft_printf("ptr from mmap is %p\n", dst);
	if (*dst == MAP_FAILED) {
		ft_printf("map failed\n");
		return false;
	}
	chunk = *dst;
	bzero(chunk, allocation_size);
	chunk->size = allocation_size;
	toggle_mask(chunk, FIRST_IN_ZONE);
	toggle_mask(chunk, LAST_IN_ZONE);
	return true;
}
