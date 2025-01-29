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
/*
UTEST_F(resetMalloc, med_freed_mem_is_coalescing) {
	void *arr[ALLOC_BY_ZONE];

	for (int i = 0; i < (ALLOC_BY_ZONE - 1); i++) {
		arr[i] = mymalloc(BIGGEST_MEDIUM);
	}
	printf("init\n");
	void *two_smaller_alloc[2];

	printf("allocating %ld\n", ((BIGGEST_MEDIUM + SIZE_CHUNK_HEADER) / 2) - SIZE_CHUNK_HEADER);
	two_smaller_alloc[0] = mymalloc(((BIGGEST_MEDIUM + SIZE_CHUNK_HEADER) / 2) - SIZE_CHUNK_HEADER);
	two_smaller_alloc[1] = mymalloc(((BIGGEST_MEDIUM + SIZE_CHUNK_HEADER) / 2) - SIZE_CHUNK_HEADER);
	printf("small alloc\n");
	myfree(two_smaller_alloc[0]);
	myfree(two_smaller_alloc[1]);
	printf("freed\n");
	void *coalesced_zone = mymalloc(BIGGEST_MEDIUM);
	printf("re use\n");
	display_arena();

	ASSERT_EQ(arena_g.medium, NULL);


}
*/
