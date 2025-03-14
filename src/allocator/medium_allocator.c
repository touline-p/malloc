#include "define.h"
#include "globals.h"
#include "intern_allocator.h"
#include "macro.h"
#include "maskmanipulation.h"
#include "printf_ft.h"
#include "type.h"
#include "zone_management.h"
#include "display.h"

#include <stdbool.h>
#include <stdio.h>
#include <strings.h>
#include <sys/mman.h>

static int _list_size(freed_chunk_t *ptr) {
	int ret = 1;
	while (ptr->next != NULL) {
		ptr = ptr->next;
		ret++;
	}
	return ret;
}

static void *_take_from_medium_list(size_t size);

void *medium_allocator(size_t size) {
	void *ret_val;

	ret_val = _take_from_medium_list(size);

	if (ret_val != NULL)
		return ret_val;

	if (zone_allocator(&arena_g.medium_list, MEDIUM_MAX_SIZE) == MAP_FAILED) {
		return NULL;
	}

	return _take_from_medium_list(size);
}

static freed_chunk_t *_fragment_chunk_if_possible(freed_chunk_t *chunk, size_t size) {
	size_t chunk_size;
	freed_chunk_t *fragmented;

	chunk_size = get_size((chunk_info_t*)chunk);
	if (chunk_size - size >= MEDIUM_MIN_SIZE) {
		fragmented = ((void *)chunk) + size;
		bzero(fragmented, sizeof(freed_chunk_t));
		set_size((chunk_info_t *)chunk, size);
		set_size((chunk_info_t *)fragmented, chunk_size - size);

		fragmented->prev = chunk->prev;
		fragmented->next = chunk->next;
		if (fragmented->prev)
			fragmented->prev->next = fragmented;
		if (fragmented->next)
			fragmented->next->prev = fragmented;

		if (chunk == arena_g.medium_list) {
			arena_g.medium_list = fragmented;
		}

		chunk->next = NULL;
		chunk->prev = NULL;

		if (mask_is_set((chunk_info_t *)chunk, LAST_IN_CHUNK)) {
			set_mask((chunk_info_t *)chunk, LAST_IN_CHUNK, false);
			set_mask((chunk_info_t *)fragmented, LAST_IN_CHUNK, true);
		}
		if (mask_is_set((chunk_info_t *)chunk, LAST_IN_ZONE)) {
			set_mask((chunk_info_t *)chunk, LAST_IN_ZONE, false);
			set_mask((chunk_info_t *)fragmented, LAST_IN_ZONE, true);
		}

		return chunk;
	}
	if (chunk == arena_g.medium_list) {
		arena_g.medium_list = chunk->next;
	}
	return chunk;
}

static void _suppress_from_list(freed_chunk_t *chunk) {
	freed_chunk_t *next;
	freed_chunk_t *prev;


	next = chunk->next;
	prev = chunk->prev;
	if (next != NULL) {
		next->prev = prev;
	}
	if (prev != NULL) {
		prev->next = next;
	}
}

static void *_take_from_medium_list(size_t size) {
	freed_chunk_t *pin;
	freed_chunk_t *ret_val;

	size = ROUND(size, HEADER_LEN);
	pin = arena_g.medium_list;

	while (pin != NULL && get_size((chunk_info_t *)pin) < size) {
		pin = pin->next;
	}
	if (pin == NULL) {
		return NULL;
	}
	_fragment_chunk_if_possible(pin, size);
	_suppress_from_list(pin);
	return pin;
}

static void _insert_chunk_in_linked_list(freed_chunk_t *header, freed_chunk_t **list);
void *_coalescing_neighbours(void *first, void *second);

void _coalescing_process(freed_chunk_t *chunk) {
	void *prev = chunk->prev;
	void *next = chunk->next;

	if (prev && mask_is_set(prev, LAST_IN_CHUNK) == false) 
		chunk = _coalescing_neighbours(prev, chunk);
	if (next && mask_is_set((void *)chunk, LAST_IN_CHUNK) == false) 
		_coalescing_neighbours(chunk, next);
}

void *_coalescing_neighbours(void *first, void *second) {
	static int nb_coalescing = 0;
	if (get_size(first) + first != second) {
		return second;
	}
	printf_ft("coalescing %d\n", nb_coalescing);
	set_size(first, get_size(first) + get_size(second));
	_suppress_from_list(second);
	return first;
}

void medium_unallocator(freed_chunk_t *header, size_t size) {
	freed_chunk_t *pin;
	freed_chunk_t *next;

	if (arena_g.medium_list == NULL) {
		header->prev = NULL;
		header->next = NULL;
		arena_g.medium_list = header;
		return ;
	}
	if (header < arena_g.medium_list) {
		header->prev = NULL;
		header->next = arena_g.medium_list;
		arena_g.medium_list->prev = header;
		arena_g.medium_list = header;
		return ;
	}


	pin = arena_g.medium_list;
	while (header < pin && pin->next) {
		pin = pin->next;
	}

	next = pin->next;
	pin->next = header;
	header->prev = pin;
	header->next = next;
	if (next)
		next->prev = header;

	// TODO fonction de coalescing 
}

