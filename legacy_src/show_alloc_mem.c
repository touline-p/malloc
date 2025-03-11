#include "globals.h"
#include "type.h"
#include <stdio.h>
#include "malloc.h"

void display_indexed_page();
void show_page_mem(page_info_t ptr);


bool is_in_page(page_info_t page, void *addr) {
	if (page.ptr <= addr && addr < page.ptr + page.length)
		return true;
	return false;
}

void *get_containing_page(void *addr) {
	page_info_t tmp;
	size_t index = 0;
	display_indexed_page();
	while (index < arena_g.page_nb) {
		tmp = arena_g.pages_arr[index];
		if (is_in_page(tmp, addr)) {
			return &arena_g.pages_arr[index];
		}
		index++;
	}
	printf_ft("ERROR : addr is not in any page indexed\n");
	return NULL;
}

void print_info_from_addr(void *addr) {
	printf_ft("print info from addr %p --", addr);
	page_info_t *page = get_containing_page(addr);
	printf_ft("page found %p", page);
	printf_ft("-- first chunk %p\n", page->ptr);
	show_page_mem(*page);
}


void show_alloc_mem() {
	printf_ft("show alloc mem\n");
	size_t index = 0;
	page_info_t *pages = arena_g.pages_arr;
	while (index < arena_g.page_nb) {
		show_page_mem(pages[index]);
		index++;
	}
}

void display_indexed_page() {
	printf_ft("display indexed page : ");
	printf_ft("pages nb : %d\n", arena_g.page_nb);
	size_t nb_page = 0;
	page_info_t ptr;
	while (nb_page < arena_g.page_nb) {
		ptr = arena_g.pages_arr[nb_page];
		printf_ft("addr %p -- len %d\n",  ptr.ptr, ptr.length);
		nb_page++;
	}
	printf_ft("end indexed print\n");
}
