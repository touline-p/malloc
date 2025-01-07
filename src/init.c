#include "mymalloc.h"

arena_t arena_g;
uint64_t size_mask;

__attribute__((constructor))
void initialiser() {
	arena_g.top = allocate_page(NULL);
}

__attribute__((destructor)) 
void desinitialiser() {
	;
}

