#include "maskmanipulation.h"
#include "globals.h"
#include "mymalloc.h"
#include "size.h"
#include "type.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

static void free_zone(void ***fn_addr, void *addr, size_t size);
void unmap_zone(freed_chunk_t *header);
void coalesce_chunk(freed_chunk_t *header);

void myfree(void *addr) {
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
			(void *)&coalesce_chunk,
		},
		{
			(void *)&is_big,
			(void *)"big",
			&arena_g.free_big,
			(void *)&unmap_zone,
		}
	};
	enum zone_e zone = TINY;

	header = get_header_from_addr(addr);
	size = GET_SIZE(header);
	NEXT_FREED_CHUNK(header) = NULL;

	while (false == COMP_CAST(fn_addr[zone][COMPARAISON])(size)){
		zone++;
	}

	free_zone(fn_addr[zone], header, size);
}

void suppress_chunk(freed_chunk_t *to_suppress) {
	if (to_suppress->next)
		to_suppress->next->prev = to_suppress->prev;
	to_suppress->prev->next = to_suppress->next;
}

void coalesce_chunk(freed_chunk_t *header) {
	printf("coalescing\n");
	printf("%p : %p\n", GET_SIZE(header) + SIZE_CHUNK_HEADER + ((void *)header), header->next);
	printf("diff %ld\n", (GET_SIZE(header) + SIZE_CHUNK_HEADER + (size_t)header) - (size_t)header->next);
	if (GET_SIZE(header) + SIZE_CHUNK_HEADER + (void *)header == header->next) {
		*(size_t *)header = GET_SIZE(header) + GET_SIZE(header->next);
		suppress_chunk(header->next);
		printf("coalescing header\n");
	}
}

void unmap_zone(freed_chunk_t *header) {
	munmap((void *)header, GET_SIZE(header));
}

static void free_zone(void ***fn_addr, void *to_free, size_t size) {
	freed_chunk_t *header = to_free;
	freed_chunk_t *link = *fn_addr[FREED_F];
	freed_chunk_t *tmp;

	printf("free of zone of size %ld\n", GET_SIZE(to_free));
	display_free(*fn_addr[FREED_F]);
	if (*fn_addr[FREED_F] == NULL) {
		printf("first_free\n");
		*fn_addr[FREED_F] = header;
		header->next = NULL;
		header->prev = NULL;
	}
	else if (to_free < *fn_addr[FREED_F]) {
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
	if (header->next)
		coalesce_chunk(header);
	if (header->prev)
		coalesce_chunk(header->prev);
}
