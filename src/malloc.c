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
	printf("allocate_memory -> top : %p\n", *top_ptr);
	chunk_info_t * disponible_chunk;
	size_t used_size = size;
	size_t disponible_chunk_size;

	size = size_allocation(size);
	size += SIZE_CHUNK_HEADER;

	disponible_chunk = *top_ptr;
	disponible_chunk_size = GET_SIZE(disponible_chunk);
	if ((disponible_chunk_size - size) < (size_min)) {
		printf("je passe la\n\n\n");
		*top_ptr = NULL;
		toggle_mask(disponible_chunk, CHUNK_IN_USE);
		SET_USE_SIZE(disponible_chunk, size);
		return get_addr_from_header(disponible_chunk);
	}

	toggle_mask(disponible_chunk, CHUNK_IN_USE);
	toggle_mask(disponible_chunk, LAST_IN_ZONE);
	ASSIGN_SIZE(disponible_chunk, &size);
	SET_USE_SIZE(disponible_chunk, used_size);

	*top_ptr = *top_ptr + size;
	**(chunk_info_t **)top_ptr = disponible_chunk_size - size;
	disponible_chunk_size -= size;
	ASSIGN_SIZE(*(chunk_info_t **)top_ptr, &disponible_chunk_size);
	toggle_mask(*top_ptr, LAST_IN_ZONE);

	printf("--RET--");
	print_chunk_info_t(disponible_chunk);
	printf("--TOP--");
	print_chunk_info_t(*top_ptr);

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
		printf("not found cause too small\n");
		return NULL;
	}

	// ici on a le probleme de l'utilisation du premier chainon de la chaine de free
	tmp = *tmp_ptr;
	ret_val = allocate_memory((void **)&tmp_ptr, size, size_min);
	if (tmp_ptr == NULL) {
		if (tmp.next)
			tmp.next->prev = tmp.prev;
		if (tmp.prev)
			tmp.prev->next = tmp.next;
	} else {
		if (tmp.next)
			tmp.next->prev = tmp_ptr;
		if (tmp.prev)
			tmp.prev->next = tmp_ptr;
	}
	if (*freed_chunks_ptr == ret_val + sizeof(chunk_info_t)) {
		*freed_chunks_ptr = NULL;
	}
	printf("fast_allocated\n");
	fast_allocation_nb += 1;
	return ret_val;
}


void *group_malloc(void ***zone, size_t size) {
	void *ret_val;

	printf("freed = %p :: top = %p\n", *zone[FREED], *zone[TOP]);
	if (*zone[FREED] != NULL &&
			NULL != (ret_val = get_next_freed(((freed_chunk_t **)zone[FREED]), size, (size_t)zone[SIZE_MIN_ALLOC]))) {
		printf("-FREE--");
		print_chunk_info_t(get_header_from_addr(ret_val));
		return ret_val;
	}
	if (*zone[TOP] == NULL && printf("reallocating top\n") && !ALLOC_CAST(zone[ALLOCATION_FN])(zone[TOP], (size_t)zone[SIZE_MAX_ALLOC], size))
		return NULL;
	if (size + SIZE_CHUNK_HEADER > GET_SIZE(*zone[TOP])) {
		printf("Size is not enough so i free\n");
		free(*zone[TOP]);
		*zone[TOP] = NULL;
	}
	if (*zone[TOP] == NULL && !ALLOC_CAST(zone[ALLOCATION_FN])(zone[TOP], (size_t)zone[SIZE_MAX_ALLOC], size))
		return NULL;

	ret_val = allocate_memory(zone[TOP], size, (size_t)zone[SIZE_MIN_ALLOC]);
	return ret_val;
}

size_t ft_strlen(char *str) {
	char *ptr = str;

	while (*ptr) ptr++;
	return ptr - str;
}


void *malloc(uint64_t size) {
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
	static int arr[ZONE_NB] = {};
	printf("arena page count %d\n", arena_g.page_nb);
	printf("%p\n", arena_g.medium);
	printf("I try to alloc %d -> ", size);

	if (size == 0)
		return NULL;
	zone = TINY;
	while (false == COMP_CAST(fn_addr[zone][COMPARAISON])(size))
		++zone;
	printf("zone is %d ->\n", zone);
	void *ret_val = group_malloc(fn_addr[zone++], size);
//	show_alloc_mem();
	printf("%p\n\n\n", ret_val);
	return ret_val;
}

