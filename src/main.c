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

bool is_tiny(size_t size) {
	return IS_TINY(size);
}

bool is_medium(size_t size) {
	dprintf(2, "bool : medium : %d\n", IS_MEDIUM(size));
	return IS_MEDIUM(size);
}

void *get_next_freed(void **disponible_chunks, size_t size) {
	void *ret_val;

	if (GET_SIZE(*disponible_chunks) >= size) {
		fast_alloc += 1;
		return allocate_memory(disponible_chunks, size);
	}

	while (NEXT_FREED_CHUNK(disponible_chunks)) {
		if (GET_SIZE(NEXT_FREED_CHUNK(disponible_chunks)) >= size) {
			fast_alloc += 1;
			ret_val = allocate_memory(&NEXT_FREED_CHUNK(disponible_chunks), size);
			return ret_val;
		}
		disponible_chunks = &NEXT_FREED_CHUNK(disponible_chunks);
	}
	return NULL;
	
}

void *group_malloc(void ***fn_addr, size_t size) {
	void *ret_val;
	dprintf(2, (char *)(fn_addr[MESSAGE]));
	if (COMP_CAST(fn_addr[COMPARAISON])(size)) {

		if (*fn_addr[FREED] != NULL &&
			NULL != (ret_val = get_next_freed(fn_addr[FREED], size)))
			return ret_val;
		if (*fn_addr[TOP] == NULL && !try_init_page(fn_addr[TOP], (size_t)fn_addr[SIZE_MAX_ALLOC]))
			exit(1);
		if (size > GET_SIZE(fn_addr[TOP]))
			stock_and_reinit(fn_addr[TOP], fn_addr[FREED], size);
		return allocate_memory(fn_addr[TOP], size);
	}
	return NULL;
}

void *mymalloc(uint64_t size) {
	void *ret_val;

	static void **fn_addr[ZONE_NUMBER][FONCTIONAL_NUMBER] = { \
		{ \
			(void *)&is_tiny, \
			&arena_g.tiny, \
			&arena_g.free_tiny, \
			(void *)&fast_alloc, \
			(void *)BIGGEST_TINY, \
			"little\n",
		}, \
		{ \
			(void *)&is_medium, \
			&arena_g.medium, \
			&arena_g.free_medium, \
			(void *)&fast_alloc, \
			(void *)BIGGEST_MEDIUM, \
			"medium\n",
		} \
	};
	enum zone_e zone = TINY;

	while (NULL == (ret_val = group_malloc(fn_addr[zone++], size))) { dprintf(2,"looping\n"); }

	return ret_val;
}

void myfree(void *addr) {
	void *header = get_header_from_addr(addr);
	NEXT_FREED_CHUNK(header) = arena_g.free_tiny;
	arena_g.free_tiny = header;
}

int try_init_page(void **to_init, size_t size) {
	chunk_info_t *chunk;

	size = size + SIZE_CHUNK_HEADER;
	size *= 100;

	*to_init = mmap_call(NULL, size);
	if (*to_init == (void *)-1)
		return false;
	chunk = *to_init;
	bzero(*to_init, size);
	*chunk = (chunk_info_t)size - SIZE_CHUNK_HEADER;
	toggle_mask(chunk, FIRST_IN_ZONE);
	return true;
}

