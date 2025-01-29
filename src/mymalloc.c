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

	printf("\n\n\n\n");
	printf("size transformation : %ld allign -> %ld header -> %ld\n", size, size_allocation(size), size + SIZE_CHUNK_HEADER);
	size = size_allocation(size);
	size += SIZE_CHUNK_HEADER;
	printf("for %ld size i alloc %ld to allign it\n", used_size, size);
	disponible_chunk = *top_ptr;
	size_t disponible_chunk_size = GET_SIZE(disponible_chunk);


	printf("disponible_chunk_size is : %ld and i allocate %ld, size_min is %ld\n", disponible_chunk_size, size, size_min);
	if ((disponible_chunk_size - size) < (size_min)) {
		printf("I nullify top and give the zone.\n");

		*top_ptr = NULL;
		toggle_mask(disponible_chunk, LAST_IN_ZONE);
		return get_addr_from_header(disponible_chunk);
	}

	*disponible_chunk = size - SIZE_CHUNK_HEADER;
	toggle_mask(disponible_chunk, CHUNK_IN_USE);
	SET_USE_SIZE(disponible_chunk, used_size);
	*top_ptr = *top_ptr + size;
	**(chunk_info_t **)top_ptr = disponible_chunk_size - size;
	return get_addr_from_header(disponible_chunk);
}

size_t fast_allocation_nb;

void *get_next_freed(freed_chunk_t **freed_chunks_ptr, size_t size, size_t size_min) {
	freed_chunk_t *tmp_ptr;
	freed_chunk_t tmp;
	void *ret_val;

	tmp_ptr = *freed_chunks_ptr ;
	while (NULL != tmp_ptr && GET_SIZE(tmp_ptr) < size) {
		tmp_ptr = tmp_ptr->next;
	}
	if (NULL == tmp_ptr)
		return NULL;

	tmp = *tmp_ptr;
	ret_val = allocate_memory((void **)&tmp_ptr, size, size_min);
	if (tmp_ptr == NULL) {
		if (tmp.next)
			tmp.next->prev = tmp.prev;
		if (tmp.prev)
			tmp.prev->next = tmp.next;
	}
	fast_allocation_nb += 1;
	return ret_val;
}

void *group_malloc(void ***zone, size_t size) {
	void *ret_val;

	if (*zone[FREED] != NULL &&
			NULL != (ret_val = get_next_freed(zone[FREED], size, (size_t)zone[SIZE_MIN_ALLOC]))) {
		printf("taking a freed one\n");
		return ret_val;
	}
	if (*zone[TOP] == NULL && printf("top is NULL\n") && !ALLOC_CAST(zone[ALLOCATION_FN])(zone[TOP], (size_t)zone[SIZE_MAX_ALLOC], size))
		return NULL;
	if (size + SIZE_CHUNK_HEADER > GET_SIZE(*zone[TOP])) {
		myfree(*zone[TOP]);
		*zone[TOP] = NULL;
	}
	if (*zone[TOP] == NULL && printf("top is NULL post check_length\n") && !ALLOC_CAST(zone[ALLOCATION_FN])(zone[TOP], (size_t)zone[SIZE_MAX_ALLOC], size))
		return NULL;

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
			(void *)SMALLEST_TINY + SIZE_CHUNK_HEADER,
			(void *)BIGGEST_TINY,
			(void *)"little\n",
			(void *)&allocate_memory_pool,
		},
		{
			(void *)&is_medium,
			&arena_g.medium,
			&arena_g.free_medium,
			(void *)&fast_allocation_nb,
			(void *)SMALLEST_MEDIUM + SIZE_CHUNK_HEADER,
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

