#include "maskmanipulation.h"
#include "utest.h"
#include "malloc.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <strings.h>
#include "resetMalloc.h"

UTEST_MAIN();



UTEST(chunk, malloc_chunk_is_set_for_tiny) {
	uint64_t sizes[] = {5, 8, 10, 15, 32};
	dprintf(2,"bonjour\n");

	for (int index = 0 ; index < 5 ; index ++ ) {

		dprintf(2,"pre malloc\n");
		uint64_t size = sizes[index];
		chunk_info_t *addr = (uint64_t*)malloc(size);
		chunk_info_t *info = get_header_from_addr(addr);
		dprintf(2,"post malloc\n");
		ASSERT_EQ(0, GET_SIZE(info) % 16);
	}
	dprintf(2, "le free");
}

UTEST(chunk, size_allocation) {
	uint64_t sizes[] = {15, 18, 20, 68, 140};
	uint64_t results[] = {16, 32, 32, 80, 144};
	for (int index = 0 ; index < 5 ; index ++ ) {
		ASSERT_EQ(results[index], size_allocation(sizes[index]));
	}
}

UTEST(chunk, two_subsequent_alloc_are_diff) {
	chunk_info_t *first = malloc(1);
	chunk_info_t *second = malloc(2);

	ASSERT_TRUE(first != second);
}



UTEST_F(resetMalloc, used_size_is_set) {
	void *first = malloc(123);
	void *second = malloc(123);
	void *header = get_header_from_addr(first);	
	ASSERT_FALSE(mask_is_set(header, LAST_IN_ZONE));
	ASSERT_EQ(NEXT_CHUNK_IN_ZONE(header), get_header_from_addr(second));
}
