#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#include <strings.h>
#include <unistd.h>

#define ALLOCATION_GROUP_NUMBER 3


size_t size_allocation(size_t size);
int try_init_page(void **to_init, size_t size);

void *allocate_memory(void **arena, size_t size) {
	chunk_info_t * disponible_chunk;

	size = size_allocation(size);
	disponible_chunk = *arena;
	size_t disponible_chunk_size = GET_SIZE(disponible_chunk);
	if (disponible_chunk_size - size - TINIEST_TINY_SIZE <= 0) {
		*arena = NULL;
		return get_addr_from_header(disponible_chunk);
	}

	*disponible_chunk = size;
	toggle_mask(disponible_chunk, CHUNK_IN_USE);

	*arena = *arena + size + SIZE_CHUNK_HEADER;
	**(chunk_info_t **)arena = disponible_chunk_size - size - SIZE_CHUNK_HEADER;
	return get_addr_from_header(disponible_chunk);
}

size_t fast_alloc;

void *mymalloc(uint64_t size) {
	void *ret_val;

	ret_val = NULL;

	if (IS_TINY(size)) {
		if (arena_g.free_tiny != NULL) {
			fast_alloc += 1;
			ret_val = arena_g.free_tiny;
			arena_g.free_tiny = NEXT_FREED_CHUNK(ret_val);
			return ret_val;
		}
		if (arena_g.tiny == NULL && !try_init_page(&arena_g.tiny, TINY_SIZE))
			exit(1);
		if (size > GET_SIZE(arena_g.tiny))
			stock_and_reinit(&arena_g.tiny, &arena_g.free_tiny, TINY_SIZE);
		ret_val = allocate_memory(&arena_g.tiny, size);
	}
	return ret_val;
}

void myfree(void *addr) {
	size_t size = GET_SIZE(get_header_from_addr(addr));
	if (IS_TINY(size)) {
		NEXT_FREED_CHUNK(addr) = arena_g.free_tiny;
		arena_g.free_tiny = addr;
		return ;
	}
}

int try_init_page(void **to_init, size_t size) {
	chunk_info_t *chunk;

	size = size + SIZE_CHUNK_HEADER;
	size *= 100;
	INFO("bonjour\n");

	*to_init = mmap_call(NULL, size);
	if (*to_init == (void *)-1)
		return false;
	chunk = *to_init;
	bzero(*to_init, size);
	*chunk = (chunk_info_t)size - SIZE_CHUNK_HEADER;
	toggle_mask(chunk, FIRST_IN_ZONE);
	return true;
}

