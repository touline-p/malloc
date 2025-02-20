#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include "malloc.h"
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
	clean_malloc()
	;
}

void init_malloc(void) {
	memset(&arena_g, 0, sizeof(arena_t));
	fast_allocation_nb = 0;

	size_t size = sysconf(_SC_PAGESIZE);

	arena_g.pages_arr = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (MAP_FAILED == arena_g.pages_arr) {
		exit(errno);
	}
}

void clean_malloc(void) {
	size_t page_nb = arena_g.page_nb;
	for (size_t index = 0; index < page_nb ; index++) {
		munmap(arena_g.pages_arr[index].ptr, arena_g.pages_arr[index].length);
	}
	munmap(arena_g.pages_arr, realloc_page_arr_if_necessary());
	arena_g.pages_arr = NULL;
}
