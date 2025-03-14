#include "define.h"
#include "type.h"
#include "globals.h"
#include "printf_ft.h"
#include "maskmanipulation.h"
#include <stddef.h>

static void _print_chunk_info_t(chunk_info_t *chunk);
void show_page_mem(page_info_t ptr);


void show_alloc_mem() {
	printf_ft("show alloc mem %d pages to see\n", arena_g.page_nb);
	size_t index = 0;
	page_info_t *pages = arena_g.pages_arr;
	bool flag = false;
	while (index < arena_g.page_nb) {
		if (index < 200) {
			show_page_mem(pages[index]);
		}
		else 
			flag = true;
		index++;
	}
	if (flag) printf_ft("pages were truncated %d\n", flag);
}

void show_page_mem(page_info_t ptr) {
	size_t index = 0;
	printf_ft("page : %p -- ", ptr.ptr);
	printf_ft("len %d\n", ptr.length);
	void *tmp = ptr.ptr;
	if (get_size(tmp) == 32 || get_size(tmp) > 4096)
		return ;
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
	printf_ft("|in use %d | fz %d | lz %d | fc %d | lc %d |",
			mask_is_set(chunk, CHUNK_IN_USE), mask_is_set(chunk, FIRST_IN_ZONE), mask_is_set(chunk, LAST_IN_ZONE),
			mask_is_set(chunk, FIRST_IN_CHUNK), mask_is_set(chunk, LAST_IN_CHUNK));
	if (mask_is_set(chunk, CHUNK_IN_USE) == false) {
		printf_ft("p : %p | n: %p |", ((freed_chunk_t *)chunk)->prev,((freed_chunk_t *)chunk)->next);
	}
	printf_ft("\n");
}
