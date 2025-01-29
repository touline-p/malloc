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
void coalesce_zone(void *header);

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
			(void *)&coalesce_zone,
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

void coalesce_zone(void *header) {
	if (false == mask_is_set(header, FIRST_IN_ZONE)) {
		;
	}
}

void unmap_zone(void *header) {
	munmap(header, GET_SIZE(header));
}

static void free_zone(void ***fn_addr, void *header, size_t size) {
	if (*fn_addr[FREED_F]) {
		printf("re free\n");
		NEXT_FREED_CHUNK(header) = *fn_addr[FREED_F];
	} else {
		printf("first free\n");
		NEXT_FREED_CHUNK(header) = NULL;
	}
	*fn_addr[FREED_F] = header;
	if (fn_addr[POST_FREE_FN_F])
		(POST_FREE_CAST(fn_addr[POST_FREE_FN_F]))(header);
}
