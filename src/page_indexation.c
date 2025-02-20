#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/mman.h>
#include <unistd.h>
#include "malloc.h"
#include "globals.h"
#include "type.h"

int realloc_page_arr_if_necessary(void);
void insert_page_in_global_arr(void *page, size_t size);
void *mmap_call_no_index(void *proximity, size_t length);

int index_page(void *page, size_t size) {
	printf("youpi");
	realloc_page_arr_if_necessary();
	printf("not going anywhere");
	insert_page_in_global_arr(page, size);
	printf("this was fine end indexpage end\n");
	arena_g.page_nb++;
	return 0;
}

int realloc_page_arr_if_necessary(void) {
	static size_t size_index = 0;
	void *new_page;
	size_t size_page = sysconf(_SC_PAGESIZE);

	printf("size index : %d nb page : %d -> used %d\n", size_index, arena_g.page_nb, arena_g.page_nb * sizeof(page_info_t));
	if (size_index - (arena_g.page_nb * sizeof(page_info_t)) > sizeof(page_info_t)) {
		return size_index;
	}
	printf("\nnew page indexation\n");
	size_page = sysconf(_SC_PAGESIZE);
	new_page = mmap_call_no_index(arena_g.pages_arr, size_index + size_page);
	if (MAP_FAILED == new_page)
		return (0);
	if (new_page != arena_g.pages_arr) {
		printf("this\n");
		memmove(new_page,  arena_g.pages_arr, size_page);
		munmap(arena_g.pages_arr, size_page);
		arena_g.pages_arr = new_page;
	}
	printf("this is fine\n");
	size_index = size_index + size_page;
	return size_index;
}

void insert_page_in_global_arr(void *to_insert, size_t size_to_insert) {
	page_info_t *dst;
	size_t len_to_move;
	size_t index;

	dst = arena_g.pages_arr;
	index = 0;
	while (index < arena_g.page_nb && arena_g.pages_arr[index].ptr > to_insert) {
		index++;
	}
	printf("going out\n\n:");
	printf("%p -> %p, size %d\n", arena_g.pages_arr + index, arena_g.pages_arr + index + 1, (arena_g.page_nb - index) * sizeof(page_info_t));
	memmove(arena_g.pages_arr + index + 1, arena_g.pages_arr + index, (arena_g.page_nb - index) * sizeof(page_info_t));
	printf("post mem move\n\n:");
	arena_g.pages_arr[index].ptr = to_insert;
	printf("post mem move\n\n:");
	arena_g.pages_arr[index].length = size_to_insert;
	printf("post mem move\n\n:");
}
