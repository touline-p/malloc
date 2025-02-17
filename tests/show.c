#include "mymalloc.h"
#include "resetMalloc.h"
#include "size.h"
#include "utest.h"
#include "globals.h"

UTEST_MAIN() 

UTEST_F(resetMalloc, show_basic) {
	void *addr = mymalloc(TINY_TEST_SIZE);
	addr = mymalloc(MEDIUM_TEST_SIZE);
	addr = mymalloc(LARGE_TEST_SIZE);

	show_alloc_mem();
}
