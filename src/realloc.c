#include <stddef.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "maskmanipulation.h"
#include "malloc.h"
#include "size.h"
#include "type.h"
#include "globals.h"

bool is_in_same_pool(size_t from, size_t to);


enum pool_function_e {
	CMP_FUNC,
	TOP_P,
	FREED_P,
	MESSAGE_P,
	REALLOC_FUNC,
	POOL_FUNCTION_NB
};

void *return_same(void ***zone_data, void *header, size_t size) {
	printf("Return _same for group : %s\n", (char *)zone_data[MESSAGE_P]);
	SET_USE_SIZE(header, size);
	return get_addr_from_header(header);
}

void *resize_and_return_addr(void *header, size_t size) {
	void * ret_val;

	ret_val = malloc(size);
	memcpy(ret_val, get_addr_from_header(header), size);
	free(get_addr_from_header(header));
	return ret_val;
}

void *realloc_medium(void ***zone_data, void *header, size_t size) {
	void *next;

	if (mask_is_set(header, LAST_IN_ZONE) && printf("This chunk is last in zone\n"))
		return resize_and_return_addr(header, size);
	next = NEXT_CHUNK_IN_ZONE(header);
	if (mask_is_set(next, CHUNK_IN_USE) && printf("Next chunk is in use\n"))
		return resize_and_return_addr(header, size);
	return return_same(zone_data, header, size);
}

#define REALLOC_FN_CAST(x) ((void * (*) (void ***, void *, size_t))(x))

void *same_pool_reallocation(void *addr, size_t size) {
	static void **fn_addr[POOL_NB][POOL_FUNCTION_NB] = {
		{
			(void *)&is_tiny,
			IGNORE_ARGUMENTS,
			IGNORE_ARGUMENTS,
			(void *)"little\n",
			(void *)&return_same
		},
		{
			(void *)&ret_true,
			&arena_g.medium,
			&arena_g.free_medium,
			(void *)"medium\n",
			(void *)&realloc_medium
		},
	};
	enum zone_e zone;

	zone = TINY;
	while (false == COMP_CAST(fn_addr[zone][CMP_FUNC])(size))
		++zone;
	return REALLOC_FN_CAST(fn_addr[zone][REALLOC_FUNC])(fn_addr[zone], get_header_from_addr(addr), size);
}

void *realloc(void *addr, size_t size) {
	void *new_addr;
	chunk_info_t *header;

	if (0 == size) {
		free(addr);
		return NULL;
	}

	
	header = get_header_from_addr(addr);
	
	//if (true == is_in_same_pool(GET_USE_SIZE(header), size)) {
		
	//return same_pool_reallocation(addr, size);
	
//}

	new_addr = malloc(size);
	memcpy(new_addr, addr, GET_USE_SIZE(header));
	free(addr);
	return new_addr;
}

bool is_in_same_pool(size_t from, size_t to) {
	static size_t zones_max[POOL_NB] = {
		BIGGEST_TINY,
		BIGGEST_MEDIUM,
	};

	for (enum zone_e zone = FIRST_POOL; zone < POOL_NB; zone++) {
		if (from <= zones_max[zone])
			return to <= zones_max[zone];
		if (to <= zones_max[zone])
			return from <= zones_max[zone];
	}
	return false;
}
