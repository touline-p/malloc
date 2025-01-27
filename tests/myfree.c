#include "mymalloc.h"
#include "size.h"
#include "utest.h"
#include "resetMalloc.h"
#include "globals.h"
#include <stdio.h>

UTEST_MAIN();



UTEST_F(resetMalloc, malloc_is_freeable) {
	void *my_malloc = mymalloc(0);
	myfree(my_malloc);
}

UTEST_F(resetMalloc, tiny_freed_mem_is_avalaible) {
	void *tiny = mymalloc(TINY_TEST_SIZE);

	ASSERT_EQ(arena_g.free_tiny, NULL);
	myfree(tiny);
	ASSERT_NE(arena_g.free_tiny, NULL);
}

UTEST_F(resetMalloc, med_freed_mem_is_avalaible) {
	void *midy = mymalloc(MEDIUM_TEST_SIZE);

	ASSERT_EQ(arena_g.free_medium, NULL);
	myfree(midy);
	ASSERT_NE(arena_g.free_medium, NULL);
}

UTEST_F(resetMalloc, big_freed_mem_is_unmapped) {
	void *biggy = mymalloc(LARGE_TEST_SIZE);
	printf("%p\n", biggy);
	myfree(biggy);
}

