#include <stdbool.h>
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
#include "allocation.h"

#define ALLOCATION_GROUP_NUMBER 3


size_t size_allocation(size_t size);
int allocate_memory_pool(void **to_init, size_t size);
int allocate_unique_zone(void **to_init, size_t max_size, size_t size);

void *allocate_memory(void **arena, size_t size) {
	chunk_info_t * disponible_chunk;
	size_t used_size = size;

	size = size_allocation(size);
	printf("for %ld size i alloc %ld to allign it\n", used_size, size);
	disponible_chunk = *arena;
	size_t disponible_chunk_size = GET_SIZE(disponible_chunk);


	printf("disponible_chunk_size is : %ld and i allocate %ld\n", disponible_chunk_size, size);
	if ((disponible_chunk_size - size - TINIEST_TINY_SIZE) < TINIEST_TINY_SIZE) {
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

size_t fast_allocation_nb;

void *get_next_freed(void **disponible_chunks, size_t size) {
	void *tmp;
	void *ret_val;

	ret_val = NEXT_FREED_CHUNK(*disponible_chunks);
	if (GET_SIZE(*disponible_chunks) >= size) {
		ret_val = allocate_memory(disponible_chunks, size);
		if (*disponible_chunks) {
			NEXT_FREED_CHUNK(*disponible_chunks) = tmp;
		} else {
			*disponible_chunks = tmp;
		}
		fast_allocation_nb += 1;
		return ret_val;
	}
	return NULL;
}

void *group_malloc(void ***fn_addr, size_t size) {
	void *ret_val;

	if (*fn_addr[FREED] != NULL &&
			NULL != (ret_val = get_next_freed(fn_addr[FREED], size))) {
		printf("taking a freed one\n");
		return ret_val;
	}
	if (*fn_addr[TOP] == NULL && printf("top is NULL\n") && !ALLOC_CAST(fn_addr[ALLOCATION_FN])(fn_addr[TOP], (size_t)fn_addr[SIZE_MAX_ALLOC], size)) {
		return NULL;
	}
	if (size + SIZE_CHUNK_HEADER > GET_SIZE(*fn_addr[TOP])) {
		printf("size is more than we can afford");
		stock_and_reinit(fn_addr[TOP], fn_addr[FREED], size);
	}

	printf("allocating size : %ld in %p sized %ld\n", size, fn_addr[TOP], GET_SIZE(*fn_addr[TOP]));
	return allocate_memory(fn_addr[TOP], size);
}



void *mymalloc(uint64_t size) {
	void *ret_val;
	static void **fn_addr[ZONE_NB][ZONE_FUNCTION_NB] = {
		{
			(void *)&is_tiny,
			&arena_g.tiny,
			&arena_g.free_tiny,
			(void *)&fast_allocation_nb,
			(void *)SMALLEST_TINY,
			(void *)BIGGEST_TINY,
			(void *)"little\n",
			(void *)&allocate_memory_pool,
		},
		{
			(void *)&is_medium,
			&arena_g.medium,
			&arena_g.free_medium,
			(void *)&fast_allocation_nb,
			(void *)SMALLEST_MEDIUM,
			(void *)BIGGEST_MEDIUM,
			(void *)"medium\n",
			(void *)&allocate_memory_pool,
		},
		{
			(void *)&is_big,
			&arena_g.big,
			&arena_g.free_big,
			(void *)&fast_allocation_nb,
			(void *)IGNORE_ARGUMENTS,
			(void *)IGNORE_ARGUMENTS,
			(void *)"Large\n",
			(void *)&allocate_unique_zone,
		},
	};
	enum zone_e zone;

	zone = TINY;
	ret_val = NULL;
	while (false == COMP_CAST(fn_addr[zone][COMPARAISON])(size))
		++zone;
	display_arena();
	printf("zone chosen : %d\n", zone);
	return group_malloc(fn_addr[zone++], size);
}

