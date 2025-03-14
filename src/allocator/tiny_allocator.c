#include "define.h"
#include "globals.h"
#include "intern_allocator.h"
#include "type.h"
#include "zone_management.h"
#include "maskmanipulation.h"

#include <stddef.h>
#include <stdint.h>
#include <sys/mman.h>


void *take_first_from_tiny_list(void);
void *take_from_tiny_list(void);
size_t tiny_zone_size(void);

void *tiny_allocator(size_t size)
{
	void *ret_val;
	if (arena_g.tiny_list == NULL && zone_allocator(&arena_g.tiny_list, TINY_MAX_SIZE) == MAP_FAILED)
		return NULL;
	return take_from_tiny_list();
}

void *take_from_tiny_list(void) {
	freed_chunk_t *ret_val;
		
	ret_val = arena_g.tiny_list;
	arena_g.tiny_list = ret_val->next;
	return ret_val;
}

void tiny_unallocator(freed_chunk_t *header, size_t size) {
	header->next = arena_g.tiny_list;
	arena_g.tiny_list = header;
}
