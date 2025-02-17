#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "mymalloc.h"
#include "globals.h"
#include "type.h"

int realloc_page_arr_if_necessary(void);
void insert_page_in_global_arr(void *page, size_t size);

int index_page(void *page, size_t size) {
	printf("pre realloc if necessary\n");
	realloc_page_arr_if_necessary();
	printf("post realloc if necessary\n");
	printf("pre insert\n");
	insert_page_in_global_arr(page, size);
	printf("post insert\n");
	return 0;
}

int realloc_page_arr_if_necessary(void) {
	void *new_page;
	void *casted;

	if (arena_g.max_size - arena_g.used_size> sizeof(page_info_t)) {
		return 0;
	}
	arena_g.page_nb++;
	new_page = mmap_call(arena_g.pages_arr, arena_g.page_nb * sysconf(_SC_PAGESIZE));
	if (MAP_FAILED == new_page)
		return (-1);
	if (new_page != arena_g.pages_arr) {
		memmove(new_page,  arena_g.pages_arr, arena_g.used_size);
		munmap(arena_g.pages_arr, arena_g.page_nb * sysconf(_SC_PAGESIZE));
		arena_g.pages_arr = new_page;
	}
	arena_g.page_nb++;
	arena_g.max_size = arena_g.page_nb * sysconf(_SC_PAGESIZE);
	return (0);
}

void insert_page_in_global_arr(void *to_insert, size_t size_to_insert) {
	page_info_t *dst;
	size_t len_to_move;

	dst = arena_g.pages_arr;
	len_to_move = arena_g.used_size;
	printf("pre boucle\n");
	while (len_to_move && dst->ptr > to_insert) {
		++dst;
		len_to_move -= sizeof(page_info_t);
	}
	printf("post boucle\n");
	memmove(dst + 1, dst, len_to_move);
	dst->ptr = to_insert;
	dst->length = size_to_insert;
	arena_g.used_size += sizeof(page_info_t);
}
