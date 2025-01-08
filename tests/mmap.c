#include "utest.h"
#include "mymalloc.h"
#include <stdbool.h>

UTEST_MAIN()

UTEST(malloc_return_non_null, youpi) {
	ASSERT_NE(mymalloc(27), NULL);
}

UTEST(malloc_return_readable_and_writable , youpi) {
	int *ptr = mymalloc(sizeof(int));
	int value = 5;
	*ptr = value;
	int bonjour = *ptr;
	ASSERT_EQ(bonjour, value);
}

UTEST(mmap, freed_mem_alloc_free_mapped_memory) {
	int *ptr = mymalloc(5);
}
