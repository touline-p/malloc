#include "maskmanipulation.h"
#include "define.h"
#include "malloc.h"
#include "printf_ft.h"
#include "type.h"

void _print_freed_chunk(freed_chunk_t *link);

void display_free(freed_chunk_t *link) {
	int index = 1;
	while (link) {
		if (index > 0) {
			printf_ft("%d : ", index);
			_print_freed_chunk(link);
		}
		index++;
		link = link->next;
	}
	printf_ft("%d chunk in list\n", index);
}



void _print_freed_chunk(freed_chunk_t *link) {
	printf_ft("%p | size : %8d | n : %8p | prev : %p ", link, get_size((chunk_info_t *)link),
			link->next, link->prev, link->infos);
	printf_ft("|in use %d | fz %d | lz %d | fc %d | lc %d |\n",
			mask_is_set(link, CHUNK_IN_USE), mask_is_set(link, FIRST_IN_ZONE), mask_is_set(link, LAST_IN_ZONE),
			mask_is_set(link, FIRST_IN_CHUNK), mask_is_set(link, LAST_IN_CHUNK));
}

