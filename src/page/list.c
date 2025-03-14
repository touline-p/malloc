#include "printf_ft.h"
#include "type.h"

freed_chunk_t *merge_list(freed_chunk_t *big, freed_chunk_t *little) {
	printf_ft("merging list\n");

	freed_chunk_t *pin;
	freed_chunk_t *next;
	freed_chunk_t *first;
	freed_chunk_t *last;

	first = little;
	last = first;
	while (last->next)
		last = last->next;

	if (last < big) {
		last->next = big;
		big->prev = last;
		return first;
	}

	pin = big;
	while (pin->next && pin->next < first) {
		pin = pin->next;
	}
	next = pin->next;
	pin->next = first;
	first->prev = pin;
	if (next)
		next->prev = last;
	last->next = next;
	return big;
}
