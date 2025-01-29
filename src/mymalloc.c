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

void *allocate_memory(void **top_ptr, size_t size, size_t size_min) {
	chunk_info_t * disponible_chunk;
	size_t used_size = size;

	size = size_allocation(size);
	printf("for %ld size i alloc %ld to allign it\n", used_size, size);
	disponible_chunk = *top_ptr;
	size_t disponible_chunk_size = GET_SIZE(disponible_chunk);


	printf("disponible_chunk_size is : %ld and i allocate %ld\n", disponible_chunk_size, size);
	if ((disponible_chunk_size - size - size_min - SIZE_CHUNK_HEADER) < (size_min + SIZE_CHUNK_HEADER)) {
		*top_ptr = NULL;
		return get_addr_from_header(disponible_chunk);
	}

	*disponible_chunk = size;
	toggle_mask(disponible_chunk, CHUNK_IN_USE);
	SET_USE_SIZE(disponible_chunk, used_size);
	*top_ptr = *top_ptr + size + SIZE_CHUNK_HEADER;
	**(chunk_info_t **)top_ptr = disponible_chunk_size - size - SIZE_CHUNK_HEADER;
	return get_addr_from_header(disponible_chunk);
}

size_t fast_allocation_nb;

void *get_next_freed(void **disponible_chunks, size_t size, size_t size_min) {
	void *tmp;
	void *ret_val;

	ret_val = NEXT_FREED_CHUNK(*disponible_chunks);
	if (GET_SIZE(*disponible_chunks) >= size) {
		ret_val = allocate_memory(disponible_chunks, size, size_min);
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

void *group_malloc(void ***zone, size_t size) {
	void *ret_val;

	if (*zone[FREED] != NULL &&
			NULL != (ret_val = get_next_freed(zone[FREED], size, (size_t)zone[SIZE_MIN_ALLOC]))) {
		printf("taking a freed one\n");
		return ret_val;
	}
	if (*zone[TOP] == NULL && printf("top is NULL\n") && !ALLOC_CAST(zone[ALLOCATION_FN])(zone[TOP], (size_t)zone[SIZE_MAX_ALLOC], size)) {
		return NULL;
	}
	if (size + SIZE_CHUNK_HEADER > GET_SIZE(*zone[TOP])) {
		printf("size is more than we can afford");
		stock_and_reinit(zone[TOP], zone[FREED], size);
	}

	printf("allocating size : %ld in %p sized %ld\n", size, zone[TOP], GET_SIZE(*zone[TOP]));
	return allocate_memory(zone[TOP], size, (size_t)zone[SIZE_MIN_ALLOC]);
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
	printf("zone chosen : %d\n", zone);
	return group_malloc(fn_addr[zone++], size);
}

