#include <stddef.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "maskmanipulation.h"
#include "mymalloc.h"
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

void *return_same(void ***zone_data, void *to_realloc, size_t size) {
	printf("Return _same for group : %s\n", (char *)zone_data[MESSAGE]);
	SET_USE_SIZE(get_header_from_addr(to_realloc), size);
	return to_realloc;
}

void *realloc_medium(void ***zone_data, void *to_realloc, size_t size) {
	return return_same(zone_data, to_realloc, size);
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
	void *ret_val;

	zone = TINY;
	while (false == COMP_CAST(fn_addr[zone][CMP_FUNC])(size))
		++zone;
	ret_val = NULL;
	return REALLOC_FN_CAST(fn_addr[zone][REALLOC_FUNC])(fn_addr[zone], addr, size);
}

void *myrealloc(void *addr, size_t size) {
	void *new_addr;
	chunk_info_t *header;
	size_t used_size;

	if (0 == size) {
		myfree(addr);
		return NULL;
	}

	header = get_header_from_addr(addr);
	if (true == is_in_same_pool(GET_SIZE(header), size)) {
		return same_pool_reallocation(addr, size);
	}

	new_addr = mymalloc(size);
	memcpy(new_addr, addr, GET_USE_SIZE(header));
	myfree(addr);
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
