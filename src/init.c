#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include "mymalloc.h"
#include "type.h"
#include "globals.h"


arena_t arena_g;
uint64_t size_mask;
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
	void *casted;

	memset(&arena_g, 0, sizeof(arena_t));
	fast_allocation_nb = 0;

	size_t size = sysconf(_SC_PAGESIZE);

	arena_g.pages_arr = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (MAP_FAILED == arena_g.pages_arr) {
		exit(errno);
	}

	arena_g.max_size = size;
	arena_g.used_size = 0;
}

void clean_malloc(void) {
	void *end_ptr = (void *)arena_g.pages_arr + arena_g.used_size;
	for (page_info_t *ptr = arena_g.pages_arr ; ptr < end_ptr ; ++ptr) {
		munmap(ptr->ptr, ptr->length);
	}
	munmap(arena_g.pages_arr, arena_g.max_size);
	arena_g.used_size = 0;
	arena_g.pages_arr = NULL;
	arena_g.max_size = 0;
}
