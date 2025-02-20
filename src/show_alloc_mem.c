#include "globals.h"
#include "maskmanipulation.h"
#include "type.h"
#include <stdio.h>

void display_indexed_page();
void show_page_mem(page_info_t ptr);

char *zone_str(enum zone_e zone) {
	return "";
	if (TINY == zone) {
		return "TINY";
	}
	if (MEDIUM == zone) {
		return "MEDIUM";
	}
	if (LARGE == zone) {
		return "LARGE";
	}
	return "";
}


void print_chunk_info_t(chunk_info_t *chunk) {
	printf("chunk addr : %10p -- ", chunk);
	printf("size: %10d, used_size: %10d -- ", GET_SIZE(chunk), GET_USE_SIZE(chunk));
	printf("|in use %d | first in zone %d | last in zone %d |\n", mask_is_set(chunk, CHUNK_IN_USE), mask_is_set(chunk, FIRST_IN_ZONE), mask_is_set(chunk, LAST_IN_ZONE));
}

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
	printf("ERROR : addr is not in any page indexed\n");
	return NULL;
}

void print_info_from_addr(void *addr) {
	printf("print info from addr %p --", addr);
	page_info_t *page = get_containing_page(addr);
	printf("page found %p", page);
	printf("-- first chunk %p\n", page->ptr);
	show_page_mem(*page);
}

void show_page_mem(page_info_t ptr) {
	printf("page : %p -- ", ptr.ptr);
	printf("len %d\n", ptr.length);
	void *tmp = ptr.ptr;
	while (mask_is_set(tmp, LAST_IN_ZONE) == false) {
		print_chunk_info_t(tmp);
		tmp += GET_SIZE(tmp);
	}
	print_chunk_info_t(tmp);
}

void show_alloc_mem() {
	printf("show alloc mem\n");
	size_t index = 0;
	page_info_t *pages = arena_g.pages_arr;
	while (index < arena_g.page_nb) {
		show_page_mem(pages[index]);
		index++;
	}
}

void display_indexed_page() {
	printf("display indexed page : ");
	printf("pages nb : %d\n", arena_g.page_nb);
	size_t nb_page = 0;
	page_info_t ptr;
	while (nb_page < arena_g.page_nb) {
		ptr = arena_g.pages_arr[nb_page];
		printf("len %d, addr %p -",  ptr.length, ptr.ptr);
		nb_page++;
	}
	printf("end indexed print\n");
}
