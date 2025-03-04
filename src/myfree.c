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

void print_arena() {
	printf("tiny top %p free %p\n", arena_g.tiny, arena_g.free_tiny);
	printf("mid  top %p free %p\n", arena_g.medium, arena_g.free_medium);
	printf("big  top %p free %p\n", arena_g.big, arena_g.free_big);
}

void free(void *addr) {

	printf("\n\n--FREE-- : %p\n", addr);
	print_chunk_info_t(get_header_from_addr(addr));


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

	if (addr == NULL) {printf("NULL inputed so return\n"); return ;}
	header = get_header_from_addr(addr);
	size = get_size(header);
	NEXT_FREED_CHUNK(header) = NULL;


	while (false == COMP_CAST(fn_addr[zone][COMPARAISON])(size)){
		zone++;
	}

	printf("free addr : %p : header : %p len : %d zone : %d\n", addr, header, size, zone);


	free_zone(fn_addr[zone], header);

	printf("post free arena :\n");
	print_arena();
	printf("tiny free chained list :\n");
	display_free(*fn_addr[TINY][FREED]);
	printf("medium free chained list :\n");
	display_free(*fn_addr[MEDIUM][FREED]);
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

	printf("coalescing %p\n", header);
	if (get_size(header) + (void *)header == header->next) {
		if (mask_is_set(header->next, LAST_IN_ZONE))
			toggle_mask(header, LAST_IN_ZONE);
		printf("really coalescing %p\n", header);
		new_size = get_size(header) + get_size(header->next);
		set_size((chunk_info_t*)header, new_size);
		suppress_chunk(header->next);
		header->next = ((void *)header) + new_size;
		
	}
}

void unmap_zone(freed_chunk_t *header) {
	printf("i unmap %p\n", header);
	desindex_page(header, get_size(header));
	munmap((void *)header, get_size(header));
	arena_g.free_big = NULL;
}

static void free_zone(void ***fn_addr, void *uncast_header) {
	freed_chunk_t *header = uncast_header;
	freed_chunk_t *link = *fn_addr[FREED_F];
	freed_chunk_t *tmp;

	printf("--- free zone ---\n");
	show_alloc_mem();
	toggle_mask(uncast_header, CHUNK_IN_USE);
	show_alloc_mem();
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
	printf("pre specific function\n");
	show_alloc_mem();
	if (fn_addr[POST_FREE_FN_F]) {
		POST_FREE_CAST(fn_addr[POST_FREE_FN_F])(header);
	}
	printf("post specific function\n");
	show_alloc_mem();
}
