#include "printf_ft.h"
#include "globals.h"

void display_arena(void) {
	printf_ft("lists : tiny - %p | medium %p |\n, pages arr : %p | len %d\n",
			arena_g.tiny_list, arena_g.medium_list,
			arena_g.pages_arr, arena_g.page_nb);
}
