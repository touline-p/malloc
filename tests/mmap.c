#include "globals.h"
#include "size.h"
#include "type.h"
#include "utest.h"
#include "mymalloc.h"
#include <alloca.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "resetMalloc.h"
#include "testUtils.h"

UTEST_MAIN();

UTEST_F(resetMalloc, malloc_return_non_null) {
	ASSERT_NE(mymalloc(TINY_TEST_SIZE), NULL);
}

UTEST_F(resetMalloc, malloc_return_readable_and_writable) {
	int *ptr = mymalloc(sizeof(int));
	int value = 5;
	*ptr = value;
	int bonjour = *ptr;
	ASSERT_EQ(bonjour, value);
}

size_t allocated_page(void) {
	return arena_g.used_size / sizeof(page_info_t);
}


UTEST_F(resetMalloc, one_hundred_allocation_fit_in_a_page) {
	size_t size = BIGGEST_TINY;
	size_t arrlen = 100;
	char *allocations[arrlen]; 

	ASSERT_EQ(allocated_page(), 0);

	for ( int index = 0 ; index < arrlen ; index ++ ) {
		allocations[index] = mymalloc(size);
		memset(allocations[index], index, size);
		
	}

	for ( int index = 0 ; index < arrlen ; index ++ ) {
		ASSERT_TRUE(is_memset_to(allocations[index], index, size));
	}
	ASSERT_EQ(allocated_page(), 1);
}

UTEST_F(resetMalloc, one_hundred_and_one_allocation_fit_in_a_page) {
	display_arena();
	size_t size = BIGGEST_TINY;
	size_t arrlen = 101;
	char *allocations[arrlen]; 

	for ( int index = 0 ; index < arrlen ; index ++ ) {
		allocations[index] = mymalloc(size);
		memset(allocations[index], index, size);
		
	}

	for ( int index = 0 ; index < arrlen ; index ++ ) {
		ASSERT_TRUE(is_memset_to(allocations[index], index, size));
	}
	ASSERT_EQ(allocated_page(), 2);
}

UTEST_F(resetMalloc, two_hundred_and_one_allocation_fit_in_a_page) {
	size_t size = BIGGEST_TINY;
	size_t arrlen = 203;
	char *allocations[arrlen]; 

	for ( int index = 0 ; index < arrlen ; index ++ ) {
		allocations[index] = mymalloc(size);
		memset(allocations[index], index, size);
	}

	for ( int index = 0 ; index < arrlen ; index ++ ) {
		ASSERT_TRUE(is_memset_to(allocations[index], index, size));
	}
	ASSERT_EQ(allocated_page(), 3);
}

UTEST_F(resetMalloc, freed_mem_stay_usable_in_free_tiny) {
	char *ptr = mymalloc(TINY_TEST_SIZE);
	memset(ptr, 2, 5);
	myfree(ptr);
	ASSERT_NE(arena_g.free_tiny, NULL);
	char *ptr_sec = mymalloc(TINY_TEST_SIZE);
	ASSERT_EQ(ptr, ptr_sec);
}

#define NB_ITER 55
UTEST_F(resetMalloc, fast_alloc_can_occure_once_by_free_of_same_size) {
	size_t iterations = NB_ITER;
	char *ptr = mymalloc(TINY_TEST_SIZE);

	while (iterations) {
		myfree(ptr);
		mymalloc(TINY_TEST_SIZE);
		--iterations;
	}
	ASSERT_EQ(fast_allocation_nb, NB_ITER);
}

UTEST_F(resetMalloc, malloc_can_allocate_multiple_pages) {
	size_t iterations = NB_ITER;

	void *addr[NB_ITER];
	while (iterations--) {
		addr[iterations] = mymalloc(LARGE_TEST_SIZE);
	}
}
