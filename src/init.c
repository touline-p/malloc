#include "mymalloc.h"
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "type.h"
#include "globals.h"

arena_t arena_g;
uint64_t size_mask;
size_t allocated_page;
page_t pages[MAX_PAGE];

__attribute__((constructor))
void initialiser() {
	init_malloc();
}

__attribute__((destructor)) 
void desinitialiser() {
	;
}

void init_malloc(void) {
	memset(&arena_g, 0, sizeof(arena_t));
	allocated_page = 0;
	fast_alloc = 0;
	arena_g.free_tiny = NULL;
	chunk_info_t *top = NULL;
}

void unmap_all_page(void) {
	for ( int index = 0 ; index < allocated_page ; index ++ ) {
		munmap(pages[index].addr, pages[index].size);
	}
}
