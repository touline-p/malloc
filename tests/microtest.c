#include "mymalloc.h"
#include "resetMalloc.h"
#include "size.h"

UTEST_MAIN();

UTEST_F(resetMalloc, this_is_fine) {
	void *addr = mymalloc(TINY_TEST_SIZE);
	myfree(addr);
}
