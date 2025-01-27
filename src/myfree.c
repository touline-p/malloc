#include "maskmanipulation.h"
#include "globals.h"
#include "mymalloc.h"
#include "size.h"
#include "type.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>

static void free_zone(void ***fn_addr, void *addr, size_t size);
void unmap_zone(void *header);

void myfree(void *addr) {
	void *header;
	size_t chunk_size;
	static void **fn_addr[ZONE_NB][FUNCTION_NB_F] = {
		{
			(void *)&is_tiny,
			"tiny",
			&arena_g.free_tiny,
			NULL,
		},
		{
			(void *)&is_medium, 
			"med",
			&arena_g.free_medium,
			NULL,
		},
		{
			(void *)&is_big,
			"big",
			NULL,
			(void *)&unmap_zone,
		}
	};
	enum zone_e zone = TINY;

	header = get_header_from_addr(addr);
	chunk_size = GET_SIZE(header);

	while (false == COMP_CAST(fn_addr[zone][COMPARAISON])(chunk_size)){
		zone++;
	}

	free_zone(fn_addr[zone], header, chunk_size);
}

void unmap_zone(void *header) {
	munmap(header, GET_SIZE(header));
}

static void free_zone(void ***fn_addr, void *header, size_t size) {
	if (fn_addr[MESSAGE_F]) {
		;
	}
	if (fn_addr[FREED_F]) {
		NEXT_FREED_CHUNK(header) = *fn_addr[FREED_F];
		*fn_addr[FREED_F] = header;
	}
	if (fn_addr[POST_FREE_FN_F])
		(POST_FREE_CAST(fn_addr[POST_FREE_FN_F]))(header);
}
