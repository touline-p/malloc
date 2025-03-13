#include "globals.h"
#include "maskmanipulation.h"
#include "malloc.h"
#include "printf_ft.h"
#include "type.h"

static void _print_freed_chunk(freed_chunk_t *link);

void display_free(freed_chunk_t *link) {
	int index = 1;
	while (link) {
		if (index < 5) {
			printf_ft("%d : ", index);
			_print_freed_chunk(link);
		}
		index++;
		link = link->next;
	}
	printf_ft("%d chunk in list\n", index);
}

static void _print_freed_chunk(freed_chunk_t *link) {
	printf_ft("%p | size : %d | n : %8p | prev : %p | infos %d\n", link, get_size((chunk_info_t *)link),
			link->next, link->prev, link->infos);
}

