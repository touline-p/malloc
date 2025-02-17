#include "globals.h"
#include "maskmanipulation.h"
#include "type.h"
#include <stdio.h>

void display_arena(void) {
	printf("arena state\n");
	printf("TINY\n");
	printf("top -> %12p freed -> %12p\n", arena_g.tiny, arena_g.free_tiny);
	printf("MEDIUM\n");
	printf("top -> %12p freed -> %12p\n", arena_g.medium, arena_g.free_medium);
	if (arena_g.medium)
		printf("top size : %lu\n", GET_SIZE(arena_g.medium));
	printf("LARGE\n");
	printf("top -> %12p freed -> %12p\n", arena_g.big, arena_g.free_big);
}

void display_free(freed_chunk_t *link) {
	printf("display_free :\n");
	size_t index = 0;
	while (NULL != link) {
		printf("%d %p size : %5d\n", index++, link, GET_SIZE(link));
		link = link->next;
	}
}
