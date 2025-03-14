#include <stdint.h>
#include <stdio.h>
#include <strings.h>
#include <sys/mman.h>
#include <unistd.h>
#include "malloc.h"
#include "globals.h"
#include "printf_ft.h"
#include "type.h"
#include "libft.h"

int realloc_page_arr_if_necessary(void);
void insert_page_in_global_arr(void *page, size_t size);
void *mmap_call_no_index(void *proximity, size_t length);
void suppress_page_out_off_global_arr(void *page);

void display_pages() {
	size_t index;

	index = 0;
	while (index < arena_g.page_nb) {
		index++;
	}
}

int index_page(void *page, size_t size) {
	realloc_page_arr_if_necessary();
	insert_page_in_global_arr(page, size);
	arena_g.page_nb++;
	return 0;
}

void desindex_page(void *page) {
	suppress_page_out_off_global_arr(page);
	arena_g.page_nb--;
}

int realloc_page_arr_if_necessary(void) {
	static size_t size_index = 0;
	void *new_page;
	size_t size_page = sysconf(_SC_PAGESIZE);

	if (size_index - (arena_g.page_nb * sizeof(page_info_t)) > sizeof(page_info_t)) {
		return size_index;
	}


	new_page = mmap(arena_g.pages_arr, size_index + size_page, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (MAP_FAILED == new_page)
		return ((int)(uint64_t)MAP_FAILED);
	if (new_page != arena_g.pages_arr)
	{
		memmove_ft(new_page,  arena_g.pages_arr, size_index);
		munmap(arena_g.pages_arr, size_page);
		arena_g.pages_arr = new_page;
	}
	size_index = size_index + size_page;
	return size_index;
}

void insert_page_in_global_arr(void *to_insert, size_t size_to_insert) {
	page_info_t *dst;
	size_t len_to_move;
	size_t index;

	dst = arena_g.pages_arr;
	index = 0;
	while (index < arena_g.page_nb && arena_g.pages_arr[index].ptr < to_insert) {
		index++;
	}
	memmove_ft(arena_g.pages_arr + index + 1, arena_g.pages_arr + index, (arena_g.page_nb - index) * sizeof(page_info_t));
	arena_g.pages_arr[index].ptr = to_insert;
	arena_g.pages_arr[index].length = size_to_insert;
}


void suppress_page_out_off_global_arr(void *page) {
	page_info_t *to_suppr;
	size_t index;

	to_suppr = arena_g.pages_arr;
	index = 0;
	while (to_suppr[index].ptr != page) {
		index++;
	}
	memmove_ft(arena_g.pages_arr + index, arena_g.pages_arr + index + 1, (arena_g.page_nb - index) * sizeof(page_info_t));

}
