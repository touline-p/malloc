#include "mymalloc.h"
#include <ctype.h>
#include <string.h>
#include <unistd.h>

arena_t arena_g;
uint64_t size_mask;

__attribute__((constructor))
void initialiser() {
	arena_g.nb_alloc = 0;
	arena_g.top = allocate_page(NULL);
	chunk_info_t *top = arena_g.top;
	*top = sysconf(_SC_PAGESIZE);
}

__attribute__((destructor)) 
void desinitialiser() {
	;
}

