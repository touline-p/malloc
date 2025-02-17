#include "globals.h"
#include "type.h"
#include <stdio.h>

char *zone_str(enum zone_e zone) {
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

void show_page_mem(page_info_t *ptr) {
	printf("%s\n", zone_str(ptr->zone));
}

void show_alloc_mem() {
	size_t used = arena_g.used_size;
	page_info_t *page = arena_g.pages_arr;
	while (used) {
		show_page_mem(page++);
		used -= sizeof(page_info_t);
	}
}


