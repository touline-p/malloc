#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <strings.h>
#include <unistd.h>

#include "malloc.h"
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
	size_t disponible_chunk_size;
	bool top_was_last = mask_is_set(*top_ptr, LAST_IN_ZONE);
	printf("given to allocate_memory %p\n", *top_ptr);

	size += SIZE_CHUNK_HEADER;
	size = size_allocation(size);

	disponible_chunk = *top_ptr;
	disponible_chunk_size = get_size(disponible_chunk);
	if ((disponible_chunk_size - size) < (size_min)) {
		*top_ptr = NULL;
		printf("i give all of the chunk\n");
		toggle_mask(disponible_chunk, CHUNK_IN_USE);
		return get_addr_from_header(disponible_chunk);
	}

	toggle_mask(disponible_chunk, CHUNK_IN_USE);
	if (top_was_last)
		toggle_mask(disponible_chunk, LAST_IN_ZONE);
	set_size(disponible_chunk, size);

	disponible_chunk_size -= size;
	*top_ptr = (*top_ptr) + size;
	set_size(*(chunk_info_t **)top_ptr, disponible_chunk_size);
	if (top_was_last)
		toggle_mask(*top_ptr, LAST_IN_ZONE);
	printf("i partition the chunk\n");
	return get_addr_from_header(disponible_chunk);
}

size_t fast_allocation_nb;

void *get_next_freed(freed_chunk_t **freed_chunks_ptr, size_t size, size_t size_min) {
	freed_chunk_t *tmp_ptr;
	freed_chunk_t tmp;
	void *ret_val;
	void *to_init;
	void *header;

	tmp_ptr = *freed_chunks_ptr ;
	while (NULL != tmp_ptr && get_size(tmp_ptr) < size + SIZE_CHUNK_HEADER) {
		tmp_ptr = tmp_ptr->next;
	}
	if (NULL == tmp_ptr) {
		return NULL;
	}
	tmp = *tmp_ptr;
	to_init = tmp_ptr;
	ret_val = allocate_memory(&to_init, size, size_min);
	header = get_header_from_addr(ret_val);
	if (to_init == NULL) {
		if (tmp.next)
			tmp.next->prev = tmp.prev;
		if (tmp.prev)
			tmp.prev->next = tmp.next;
	} else {
		if (tmp.next)
			tmp.next->prev = to_init;
		if (tmp.prev)
			tmp.prev->next = to_init;
		tmp_ptr = to_init;
		tmp_ptr->next = tmp.next;
		tmp_ptr->prev = tmp.prev;
	}
	printf("free a %p et ret %p\n", *freed_chunks_ptr, get_header_from_addr(ret_val));
	show_alloc_mem();
	if (get_addr_from_header(*freed_chunks_ptr) == ret_val) {
		printf("check diff %d\n", to_init - (void *)*freed_chunks_ptr);
		*freed_chunks_ptr = to_init;
		printf("premier chainon -> global set a ->%p\n", *freed_chunks_ptr);
	}
	fast_allocation_nb += 1;
	return ret_val;
}


void *group_malloc(void ***zone, size_t size) {
	void *ret_val;

	printf("top : %p, free : %p\n", *zone[TOP], *zone[FREED]);
	if (*zone[FREED] != NULL &&
			NULL != (ret_val = get_next_freed(((freed_chunk_t **)zone[FREED]), size, (size_t)zone[SIZE_MIN_ALLOC]))) {
		printf("found in free\n");
		return ret_val;
	}
	if (*zone[TOP] == NULL && !ALLOC_CAST(zone[ALLOCATION_FN])(zone[TOP], (size_t)zone[SIZE_MAX_ALLOC], size))
		return NULL;
	printf("post process top: %p, size %d\n", *zone[TOP], get_size(*zone[TOP]));
	if (size + SIZE_CHUNK_HEADER > get_size(*zone[TOP])) {
		printf("zone is not enough i free");
		free(get_addr_from_header(*zone[TOP]));
		*zone[TOP] = NULL;
	}
	else 
		printf("zone is enoughly big\n");
	if (*zone[TOP] == NULL && !ALLOC_CAST(zone[ALLOCATION_FN])(zone[TOP], (size_t)zone[SIZE_MAX_ALLOC], size))
		return NULL;

	printf("pre allocate top: %p, size %d\n", *zone[TOP], get_size(*zone[TOP]));
	ret_val = allocate_memory(zone[TOP], size, (size_t)zone[SIZE_MIN_ALLOC]);
	if (NULL == *zone[TOP])
		return ret_val;
	printf("post allocate top: %p, size %d\n", *zone[TOP], get_size(*zone[TOP]));
	return ret_val;
}

size_t ft_strlen(char *str) {
	char *ptr = str;

	while (*ptr) ptr++;
	return ptr - str;
}


void *malloc(uint64_t size) {
	printf("\n--MALLOC--\nAllocating %d -- ", size);
	static size_t nb_alloc = 0;
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
			(void *)BIGGEST_TINY + SIZE_CHUNK_HEADER,
			(void *)BIGGEST_MEDIUM,
			(void *)"medium\n",
			(void *)&allocate_memory_pool,
		},
		{
			(void *)&is_big,
			&arena_g.big,
			&arena_g.free_big,
			(void *)&fast_allocation_nb,
			(void *)BIGGEST_MEDIUM + SIZE_CHUNK_HEADER,
			(void *)IGNORE_ARGUMENTS,
			(void *)"Large\n",
			(void *)&allocate_unique_zone,
		},
	};
	enum zone_e zone;

	if (size == 0)
		return NULL;
	zone = TINY;
	while (false == COMP_CAST(fn_addr[zone][COMPARAISON])(size + SIZE_CHUNK_HEADER))
		++zone;
	printf("zone is %d\n", zone);
	void *ret_val = group_malloc(fn_addr[zone++], size);
	display_arena();
	show_alloc_mem();
	printf("Ret addr -> %p\n", ret_val);
	return ret_val;
}

