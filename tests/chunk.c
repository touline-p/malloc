#include "utest.h"
#include "mymalloc.h"
#include <stdbool.h>
#include <stdint.h>
#include <strings.h>

UTEST_MAIN();

UTEST(chunk, malloc_chunk_is_set_for_tiny) {
	uint64_t sizes[] = {5, 8, 10, 15, 32};

	for (int index = 0 ; index < 5 ; index ++ ) {

		uint64_t size = sizes[index];
		chunk_info_t *addr = (uint64_t*)mymalloc(size);
		chunk_info_t *info = get_header_from_addr(addr);
		ASSERT_EQ(0, GET_SIZE(info) % 16);
	}
}

UTEST(chunk, size_allocation) {
	uint64_t sizes[] = {15, 18, 20, 68, 140};
	uint64_t results[] = {16, 32, 32, 80, 144};
	for (int index = 0 ; index < 5 ; index ++ ) {
		ASSERT_EQ(results[index], size_allocation(sizes[index]));
	}
}

UTEST(chunk, two_subsequent_alloc_are_diff) {
	chunk_info_t *first = mymalloc(1);
	chunk_info_t *second = mymalloc(2);

	ASSERT_TRUE(first != second);
}
