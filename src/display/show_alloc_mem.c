#include "define.h"
#include "type.h"
#include "globals.h"
#include "printf_ft.h"
#include "maskmanipulation.h"
#include <stddef.h>

static void _print_chunk_info_t(chunk_info_t *chunk);
static void _show_page_mem(page_info_t ptr);


void show_alloc_mem() {
	printf_ft("show alloc mem %d pages to see\n", arena_g.page_nb);
	size_t index = 0;
	page_info_t *pages = arena_g.pages_arr;
	while (index < arena_g.page_nb) {
		_show_page_mem(pages[index]);
		index++;
	}
}

static void _show_page_mem(page_info_t ptr) {
	size_t index = 0;
	printf_ft("page : %p -- ", ptr.ptr);
	printf_ft("len %d\n", ptr.length);
	void *tmp = ptr.ptr;
	while (mask_is_set(tmp, LAST_IN_ZONE) == false) {
		printf_ft("%d : ", index++);
		_print_chunk_info_t(tmp);
		tmp += get_size(tmp);
	}
	printf_ft("%d : ", index++);
	_print_chunk_info_t(tmp);
}

void _print_chunk_info_t(chunk_info_t *chunk) {
	printf_ft("chunk addr : %10p -- ", chunk);
	printf_ft("size: %10d -- ", get_size(chunk));
	printf_ft("|in use %d | fz %d | lz %d | fc %d | lc %d |\n",
			mask_is_set(chunk, CHUNK_IN_USE), mask_is_set(chunk, FIRST_IN_ZONE), mask_is_set(chunk, LAST_IN_ZONE),
			mask_is_set(chunk, FIRST_IN_CHUNK), mask_is_set(chunk, LAST_IN_CHUNK));
}
