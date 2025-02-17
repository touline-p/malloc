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
	size_t disponible_chunk_size;

	size = size_allocation(size);
	size += SIZE_CHUNK_HEADER;

	disponible_chunk = *top_ptr;
	disponible_chunk_size = GET_SIZE(disponible_chunk);
	if ((disponible_chunk_size - size) < (size_min)) {
		*top_ptr = NULL;
		toggle_mask(disponible_chunk, LAST_IN_ZONE);
		SET_USE_SIZE(disponible_chunk, size);
		return get_addr_from_header(disponible_chunk);
	}

	toggle_mask(disponible_chunk, CHUNK_IN_USE);
	ASSIGN_SIZE(disponible_chunk, &size);
	SET_USE_SIZE(disponible_chunk, used_size);

	*top_ptr = *top_ptr + size;
	**(chunk_info_t **)top_ptr = disponible_chunk_size - size;
	disponible_chunk_size -= size;
	ASSIGN_SIZE(*(chunk_info_t **)top_ptr, &disponible_chunk_size);
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
	if (NULL == tmp_ptr) {
		return NULL;
	}

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

	printf("pre free\n");
	if (*zone[FREED] != NULL &&
			NULL != (ret_val = get_next_freed(zone[FREED], size, (size_t)zone[SIZE_MIN_ALLOC]))) {
		return ret_val;
	}
	printf("no free found\n");
	if (*zone[TOP] == NULL && !ALLOC_CAST(zone[ALLOCATION_FN])(zone[TOP], (size_t)zone[SIZE_MAX_ALLOC], size))
		return NULL;
	printf("alloca fn done\n");
	if (size + SIZE_CHUNK_HEADER > GET_SIZE(*zone[TOP])) {
		myfree(*zone[TOP]);
		*zone[TOP] = NULL;
	}
	if (*zone[TOP] == NULL && !ALLOC_CAST(zone[ALLOCATION_FN])(zone[TOP], (size_t)zone[SIZE_MAX_ALLOC], size))
		return NULL;

	printf("pre allocate_memory\n");
	ret_val = allocate_memory(zone[TOP], size, (size_t)zone[SIZE_MIN_ALLOC]);
	printf("post allocate_memory\n");

	void *header = get_header_from_addr(ret_val);
	return ret_val;
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
			(void *)48,
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

	if (size == 0)
		return NULL;
	zone = TINY;
	ret_val = NULL;
	while (false == COMP_CAST(fn_addr[zone][COMPARAISON])(size))
		++zone;
	printf("zone : %d\n", zone);
	return group_malloc(fn_addr[zone++], size);
}

