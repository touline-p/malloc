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
void suppress_page_out_off_global_arr(void *page);

void display_pages() {
	size_t index;

	index = 0;
	ft_printf("                    --------page indexation\n");
	while (index < arena_g.page_nb) {
		ft_printf("| %d : %p : %d ", index, arena_g.pages_arr[index].ptr, arena_g.pages_arr[index].length);
		index++;
	}
	ft_printf("|\n");
}

int index_page(void *page, size_t size) {
	display_pages();
	ft_printf("index page called\n");
	ft_printf("checking reallocation\n");
	realloc_page_arr_if_necessary();
	ft_printf("inserti in global arr\n");
	insert_page_in_global_arr(page, size);
	ft_printf("incrementing page nb\n");
	arena_g.page_nb++;
	display_pages();
	return 0;
}

void desindex_page(void *page, size_t size) {
	(void)size;
	suppress_page_out_off_global_arr(page);
	arena_g.page_nb--;
}

int realloc_page_arr_if_necessary(void) {
	static size_t size_index = 0;
	void *new_page;
	size_t size_page = sysconf(_SC_PAGESIZE);

	ft_printf("it s fine\n");
	if (size_index - (arena_g.page_nb * sizeof(page_info_t)) > sizeof(page_info_t)) {
		ft_printf("it s fine\n");
		return size_index;
	}

	ft_printf("allocating a new page for indexation purposes\n");

	new_page = mmap(arena_g.pages_arr, size_index + size_page, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	ft_printf("new addr allocated is : %p\n", new_page);
	if (MAP_FAILED == new_page) {
		ft_printf("FATAL : map failed\n");
		return (0);
	}
	if (new_page != arena_g.pages_arr) {
		ft_printf("it's different\n");
		ft_printf("memmove from %p to %p : %d bytes\n", arena_g.pages_arr, new_page, size_page);
		memmove(new_page,  arena_g.pages_arr, size_index);
		ft_printf("memeove done\n");
		munmap(arena_g.pages_arr, size_page);
		arena_g.pages_arr = new_page;
	} else {
		ft_printf("it s not\n");
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
	memmove(arena_g.pages_arr + index + 1, arena_g.pages_arr + index, (arena_g.page_nb - index) * sizeof(page_info_t));
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
	memmove(arena_g.pages_arr + index, arena_g.pages_arr + index + 1, (arena_g.page_nb - index) * sizeof(page_info_t));

}
