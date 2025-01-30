#include "maskmanipulation.h"
#include "size.h"
#include "utest.h"
#include "resetMalloc.h"
#include "mymalloc.h"

UTEST_MAIN();


UTEST_F(resetMalloc, malloc_tiny_has_usable_size) {
	size_t size = 12;
	void *addr = mymalloc(size);
	size_t upper_size = size_allocation(size);

	void *header = get_header_from_addr(addr);
	ASSERT_EQ(size, GET_USE_SIZE(header));
	ASSERT_EQ(upper_size, GET_SIZE(header));
}

UTEST_F(resetMalloc, tiny_to_tiny_use_size) {
	size_t size = 10;
	size_t bigger_size = size + 2;
	void *addr = mymalloc(size);
	void *realloc_addr = myrealloc(addr, size + 2);
	ASSERT_EQ(GET_USE_SIZE(get_header_from_addr(realloc_addr)), size + 2);
}

UTEST_F(resetMalloc, tiny_to_tinier_reuse_same_addr) {
	void *addr = mymalloc(BIGGEST_TINY);
	void *realloc_addr = myrealloc(addr, BIGGEST_TINY - 2);

	ASSERT_EQ(addr, realloc_addr);
}

UTEST_F(resetMalloc, tiny_to_tinier_keep_first_info) {

}

UTEST_F(resetMalloc, tiny_to_tiny_alloc_is_usable) {
	size_t size = 10;
	size_t bigger_size = size + 2;
	void *addr = mymalloc(size);
	void *realloc_addr = myrealloc(addr, size + 2);
	size_t use_size = GET_USE_SIZE(get_header_from_addr(realloc_addr));
	memset(realloc_addr, 0, use_size);
}

UTEST_F(resetMalloc, tiny_to_medium_use_size) {
	size_t size = BIGGEST_TINY;
	void *addr = mymalloc(size);
	size_t bigger_size = size + 2;
	void *realloc_addr = myrealloc(addr, bigger_size);
	ASSERT_EQ(GET_USE_SIZE(get_header_from_addr(realloc_addr)), bigger_size);
}

#define ITER 100
#define REALLOCED_INDEX 50

UTEST_F(resetMalloc, tiny_to_medium_alloc_is_usable) {
	void *arr[ITER];
	size_t size = BIGGEST_TINY;
	for (size_t index = 0; index < ITER; index++) {
		arr[index] = mymalloc(size);
		memset(arr[index], index, size);
	}
	size_t bigger_size = BIGGEST_TINY + 55;

	void *realloc_addr = myrealloc(arr[REALLOCED_INDEX], bigger_size);
	memset(realloc_addr, REALLOCED_INDEX, bigger_size);

	for (size_t index = 0; index < ITER; index++) {
		if (index == REALLOCED_INDEX)
			continue ;
		ASSERT_TRUE(check_integrity(index, size, arr[index]));
	}
}
