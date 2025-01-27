#include <errno.h>
#include <stddef.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <strings.h>
#include <unistd.h>

#include "mymalloc.h"
#include "size.h"
#include "type.h"
#include "maskmanipulation.h"
#include "globals.h"

#define ALLOCATION_GROUP_NUMBER 3


size_t size_allocation(size_t size);
int try_init_page(void **to_init, size_t size);
int try_init_adjusted(void **to_init, size_t max_size, size_t size);

void *allocate_memory(void **arena, size_t size) {
	chunk_info_t * disponible_chunk;
	size_t used_size = size;

	size = size_allocation(size);
	disponible_chunk = *arena;
	size_t disponible_chunk_size = GET_SIZE(disponible_chunk);

	printf("size a allouer : %lu\n", size);
	printf("taille disponible %ld\n", disponible_chunk);

	if (disponible_chunk_size - size - TINIEST_TINY_SIZE <= 0) {
		*arena = NULL;
		return get_addr_from_header(disponible_chunk);
	}

	*disponible_chunk = size;
	toggle_mask(disponible_chunk, CHUNK_IN_USE);
	SET_USE_SIZE(disponible_chunk, used_size);
	*arena = *arena + size + SIZE_CHUNK_HEADER;
	**(chunk_info_t **)arena = disponible_chunk_size - size - SIZE_CHUNK_HEADER;
	return get_addr_from_header(disponible_chunk);
}

size_t fast_alloc;

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
	if (COMP_CAST(fn_addr[COMPARAISON])(size)) {
		printf("group_malloc : size : %lu\n", size);
		printf("this is a %s\n", fn_addr[MESSAGE]);

		if (*fn_addr[FREED] != NULL &&
			NULL != (ret_val = get_next_freed(fn_addr[FREED], size))) {
			printf("I return next_freed\n");
			return ret_val;
		}
		if (*fn_addr[TOP] == NULL && !ALLOC_CAST(fn_addr[ALLOCATION_FN])(fn_addr[TOP], (size_t)fn_addr[SIZE_MAX_ALLOC], size)) {
			printf("mistake was made.\n");
			return NULL;
		}
		if (size > GET_SIZE(fn_addr[TOP])) {
			printf("i restock\n");
			stock_and_reinit(fn_addr[TOP], fn_addr[FREED], size);
		}
		return allocate_memory(fn_addr[TOP], size);
	}
	return NULL;
}



void *mymalloc(uint64_t size) {
	void *ret_val;
	static void **fn_addr[ZONE_NB][ZONE_FUNCTION_NB] = {
		{
			(void *)&is_tiny,
			&arena_g.tiny,
			&arena_g.free_tiny,
			(void *)&fast_alloc,
			(void *)BIGGEST_TINY,
			(void *)"little\n",
			(void *)&try_init_page,
		},
		{
			(void *)&is_medium,
			&arena_g.medium,
			&arena_g.free_medium,
			(void *)&fast_alloc,
			(void *)BIGGEST_MEDIUM,
			(void *)"medium\n",
			(void *)&try_init_page,
		},
		{
			(void *)&is_big,
			&arena_g.big,
			&arena_g.free_big,
			(void *)&fast_alloc,
			(void *)0,
			(void *)"Large\n",
			(void *)&try_init_adjusted,
		},
	};
	enum zone_e zone;

	zone = TINY;
	ret_val = NULL;
	while (zone < ZONE_NB && NULL == ret_val) {
		ret_val = group_malloc(fn_addr[zone++], size); 
	}
	return ret_val;
}

int try_init_adjusted(void **to_init, size_t max_size, size_t size) {
	printf("init adjusted\n");
	chunk_info_t *chunk;

	printf("allocating size : %ld\n", size);
	size += SIZE_CHUNK_HEADER;
	size /= sysconf(_SC_PAGESIZE);
	size += 1;
	size *= sysconf(_SC_PAGESIZE);
	printf("allocating size : %ld\n", size);

	*to_init = mmap_call(NULL, size);

	printf("%d\n", errno);

	if (*to_init == (void *)-1)
		return false;
	chunk = *to_init;
	bzero(chunk, size);
	*chunk = (chunk_info_t)size;
	toggle_mask(chunk, FIRST_IN_ZONE);
	return true;
}

int try_init_page(void **to_init, size_t size) {
	chunk_info_t *chunk;

	size += SIZE_CHUNK_HEADER;
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

