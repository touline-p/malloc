#include "define.h"
#include "globals.h"
#include "intern_allocator.h"
#include "malloc.h"
#include "maskmanipulation.h"
#include "printf_ft.h"
#include "type.h"
#include "zone_management.h"

#include <stdbool.h>
#include <stdio.h>
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
	printf_ft("medium allocator", size);
	void *ret_val;

	ret_val = _take_from_medium_list(size);

	if (NULL != arena_g.medium_list)
		return ret_val;

	if (NULL == arena_g.medium_list && zone_allocator(&arena_g.medium_list, MEDIUM_MAX_SIZE) == MAP_FAILED) {
		printf_ft("medium allocator failed\n");
		return NULL;
	}
	printf_ft("take _from medium returning first from list\n");

	return _take_from_medium_list(size);
}

static void *_take_from_medium_list(size_t size) {
	freed_chunk_t *ret_val;
	printf_ft("take from medium list\n", size);

	ret_val = arena_g.medium_list;
	if (ret_val == NULL)
		return ret_val;
	arena_g.medium_list = ret_val->next;
	if (arena_g.medium_list != NULL)
		arena_g.medium_list->prev = NULL;
	set_mask((chunk_info_t *)ret_val, CHUNK_IN_USE, true);
	return ret_val;
}

static void _insert_chunk_in_linked_list(freed_chunk_t *header, freed_chunk_t **list);

void medium_unallocator(freed_chunk_t *header, size_t size) {
	set_mask((chunk_info_t*)header, CHUNK_IN_USE, false);
	if (arena_g.medium_list == NULL) {
		arena_g.medium_list = header;
		display_free(arena_g.medium_list);
		return ;
	}
	_insert_chunk_in_linked_list(header, &arena_g.medium_list);
}

static void _insert_chunk_in_linked_list(freed_chunk_t *header, freed_chunk_t **list) {
	freed_chunk_t *pin;

	pin = *list;
	if (header < pin) {
		header->prev = NULL;
		header->next = pin;
		if (header->next == NULL) {
			printf_ft("je rentre la ");
			header->next->prev = header;
		}
		*list = header;
		return ;
	}
	while (pin->next != NULL && header < pin->next) {
		pin = pin->next;
	}
	header->next = pin->next;
	header->prev = pin;
	if (header->next)
		header->next->prev = header;
	pin->next = header;
}
