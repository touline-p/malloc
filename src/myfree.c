#include "maskmanipulation.h"
#include "globals.h"


void myfree(void *addr) {
	void *header = get_header_from_addr(addr);
	NEXT_FREED_CHUNK(header) = arena_g.free_tiny;
	arena_g.free_tiny = header;
}

