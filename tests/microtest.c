#include "malloc.h"
#include "resetMalloc.h"
#include "size.h"

UTEST_MAIN();

UTEST_F(resetMalloc, this_is_fine) {
	void *addr = malloc(TINY_TEST_SIZE);
	free(addr);
}
