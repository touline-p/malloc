#include "maskmanipulation.h"
#include "globals.h"
#include "malloc.h"
#include "size.h"
#include "type.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

static void free_zone(void ***fn_addr, void *addr);
void unmap_zone(freed_chunk_t *header);
void coalesce_chunk(freed_chunk_t *header);
void coalescing_medium(void *header);

void free(void *addr) {
	void *header;
	size_t size;
	static void **fn_addr[ZONE_NB][FUNCTION_NB_F] = {
		{
			(void *)&is_tiny,
			(void *)"tiny",
			&arena_g.free_tiny,
			NULL,
		},
		{
			(void *)&is_medium, 
			(void *)"med",
			&arena_g.free_medium,
			(void *)&coalescing_medium,
		},
		{
			(void *)&is_big,
			(void *)"big",
			&arena_g.free_big,
			(void *)&unmap_zone,
		}
	};
	enum zone_e zone = TINY;

	if (addr == NULL)
		return ;
	header = get_header_from_addr(addr);
	size = GET_USE_SIZE(header);
	NEXT_FREED_CHUNK(header) = NULL;

	while (false == COMP_CAST(fn_addr[zone][COMPARAISON])(size)){
		zone++;
	}


	free_zone(fn_addr[zone], header);
}

void suppress_chunk(freed_chunk_t *to_suppress) {
	if (to_suppress->next)
		to_suppress->next->prev = to_suppress->prev;
	if (to_suppress->prev)
		to_suppress->prev->next = to_suppress->next;
}

void coalescing_medium(void *addr) {
	freed_chunk_t *header = addr;
	if (header->next)
		coalesce_chunk(header);
	if (header->prev)
		coalesce_chunk(header->prev);
}

void coalesce_chunk(freed_chunk_t *header) {
	size_t new_size;

	if (GET_SIZE(header) + (void *)header == header->next) {
		new_size = GET_SIZE(header) + GET_SIZE(header->next);
		ASSIGN_SIZE((chunk_info_t*)header, &new_size);
		suppress_chunk(header->next);
		
	}
}

void unmap_zone(freed_chunk_t *header) {
	printf("i unmap %p\n", header);
	munmap((void *)header, GET_SIZE(header));
	arena_g.free_big = NULL;
}

static void free_zone(void ***fn_addr, void *uncast_header) {
	freed_chunk_t *header = uncast_header;
	freed_chunk_t *link = *fn_addr[FREED_F];
	freed_chunk_t *tmp;

	toggle_mask(uncast_header, CHUNK_IN_USE);
	if (*fn_addr[FREED_F] == NULL) {
		printf("first_free\n");
		*fn_addr[FREED_F] = header;
		header->next = NULL;
		header->prev = NULL;
	}
	else if (uncast_header < *fn_addr[FREED_F]) {
		printf("free au premier chainon\n");
		header->next = *fn_addr[FREED_F];
		header->prev = NULL;
		*fn_addr[FREED_F] = header;
	}
	else {
		while (link->next != NULL && header < link->next)
			link = link->next;
		printf("free dans la chaine\n");
		tmp = link->next;
		link->next = header;
		header->prev = link;
		header->next = tmp;
		if (NULL != tmp)
			tmp->prev = header;
	}
	if (fn_addr[POST_FREE_FN_F]) {
		POST_FREE_CAST(fn_addr[POST_FREE_FN_F])(header);
	}
}
