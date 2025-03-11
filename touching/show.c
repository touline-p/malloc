#include "malloc.h"
#include "resetMalloc.h"
#include "size.h"
#include "utest.h"
#include "globals.h"

UTEST_MAIN() 

UTEST_F(resetMalloc, show_basic) {
	void *addr = malloc(TINY_TEST_SIZE);
	addr = malloc(MEDIUM_TEST_SIZE);
	addr = malloc(LARGE_TEST_SIZE);

	show_alloc_mem();
}
