#include "globals.h"
#include "maskmanipulation.h"
#include "size.h"
#include "type.h"
#include "utest.h"
#include "resetMalloc.h"
#include "mymalloc.h"
#include <bits/posix1_lim.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

UTEST_MAIN();

bool is_in_same_pool(size_t from, size_t to);

UTEST_F(resetMalloc, malloc_tiny_has_usable_size) {
	size_t size = 12;
	void *addr = mymalloc(size);
	size_t upper_size = size_allocation(size) + SIZE_CHUNK_HEADER;

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

UTEST(zone, is_in_same_pool_unit_test) {
	static size_t zones_max[POOL_NB] = {
		BIGGEST_TINY,
		BIGGEST_MEDIUM,
	};
	ASSERT_TRUE(is_in_same_pool(TINY_TEST_SIZE, TINY_TEST_SIZE));
	ASSERT_FALSE(is_in_same_pool(MEDIUM_TEST_SIZE, TINY_TEST_SIZE));
	ASSERT_FALSE(is_in_same_pool(LARGE_TEST_SIZE, TINY_TEST_SIZE));
	ASSERT_FALSE(is_in_same_pool(TINY_TEST_SIZE, MEDIUM_TEST_SIZE));
	ASSERT_TRUE(is_in_same_pool(MEDIUM_TEST_SIZE, MEDIUM_TEST_SIZE));
	ASSERT_FALSE(is_in_same_pool(LARGE_TEST_SIZE, MEDIUM_TEST_SIZE));
	ASSERT_FALSE(is_in_same_pool(TINY_TEST_SIZE, LARGE_TEST_SIZE));
	ASSERT_FALSE(is_in_same_pool(MEDIUM_TEST_SIZE, LARGE_TEST_SIZE));
	ASSERT_FALSE(is_in_same_pool(LARGE_TEST_SIZE, LARGE_TEST_SIZE));
}

UTEST_F(resetMalloc, tiny_to_tiny_reuse_same_addr) {
	//tinier
	void *addr = mymalloc(BIGGEST_TINY);
	void *realloc_addr = myrealloc(addr, BIGGEST_TINY - 2);
	printf("tinier\n");
	ASSERT_EQ(addr, realloc_addr);

	//same
	addr = realloc_addr;
	realloc_addr = myrealloc(addr, BIGGEST_TINY - 2);
	printf("tinier\n");
	ASSERT_EQ(addr, realloc_addr);

	//bigger	
	addr = realloc_addr;
	realloc_addr = myrealloc(addr, BIGGEST_TINY);
	printf("tinier\n");
	ASSERT_EQ(addr, realloc_addr);
}

UTEST_F(resetMalloc, medium_to_medium_reuse_same_addr) {
	//tinier
	void *addr = mymalloc(MEDIUM_TEST_SIZE);
	void *realloc_addr = myrealloc(addr, MEDIUM_TEST_SIZE + 55);
	ASSERT_EQ(addr, realloc_addr);

	//same
	addr = realloc_addr;
	realloc_addr = myrealloc(addr, MEDIUM_TEST_SIZE + 55);
	ASSERT_EQ(addr, realloc_addr);

	//bigger	
	addr = realloc_addr;
	realloc_addr = myrealloc(addr, MEDIUM_TEST_SIZE);
	ASSERT_EQ(addr, realloc_addr);
}

UTEST_F(resetMalloc, realloc_with_zero_sized_just_free) {
	void *addr = mymalloc(TINY_TEST_SIZE);
	myrealloc(addr, 0);
	ASSERT_NE(arena_g.free_tiny, NULL);

	addr = mymalloc(MEDIUM_TEST_SIZE);
	myrealloc(addr, 0);
	ASSERT_NE(arena_g.free_medium, NULL);
}


#define MARK 15

UTEST_F(resetMalloc, tiny_to_tinier_keep_data_integrity) {
	void *addr = mymalloc(BIGGEST_TINY);
	char array[BIGGEST_TINY];
	memset(addr, MARK, BIGGEST_TINY);
	memcpy(array, addr, BIGGEST_TINY);
	addr = myrealloc(addr, BIGGEST_TINY - 2);
	
	ASSERT_TRUE(memcmp(addr, array, BIGGEST_TINY - 2) == 0);
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

UTEST_F(resetMalloc, smart_allocation_if_next_is_top) {
	void *addr = mymalloc(MEDIUM_TEST_SIZE);
	void *realloc_addr = myrealloc(addr, MEDIUM_TEST_SIZE + 27);

	ASSERT_EQ(realloc_addr, addr);
}

UTEST_F(resetMalloc, smart_allocation_if_next_was_freed) {
	printf("first alloc\n");
	void *addr = mymalloc(MEDIUM_TEST_SIZE);
	printf("second alloc\n");
	void *next = mymalloc(MEDIUM_TEST_SIZE);
	printf("first free\n");
	myfree(next);
	printf("realloc\n");
	void *realloc_addr = myrealloc(addr, MEDIUM_TEST_SIZE + 27);

	ASSERT_EQ(realloc_addr, addr);
}

UTEST_F(resetMalloc, smart_medium_alloc_doesnt_pollute_over_chunk) {
	void *arr[ITER];
	for (size_t index = 0; index < ITER; index++) {
		arr[index] = mymalloc(MEDIUM_TEST_SIZE);
		memset(arr[index], index, MEDIUM_TEST_SIZE);
	}
	void *new = myrealloc(arr[0], MEDIUM_TEST_SIZE + 95);
	check_integrity(0, MEDIUM_TEST_SIZE, new);
	memset(new, 0, MEDIUM_TEST_SIZE + 95);
	for (size_t index = 1; index < ITER; index++) {
		ASSERT_TRUE(check_integrity(index, MEDIUM_TEST_SIZE, arr[index]));
	}
}

#include "debug.h"

UTEST_F(resetMalloc, this_INFO_seems_legit) {
	void *addr = mymalloc(156);
	DEBUG("%p", addr);
}
