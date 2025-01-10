#include "utest.h"
#include "resetMalloc.h"
#include "mymalloc.h"
#include "testUtils.h"
#include <stdbool.h>

UTEST_MAIN();

UTEST_F(resetMalloc, try_init_adjusted_alloc_a_usable_addr) {
	void *addr; 
	try_init_page(&addr, LARGE_TEST_SIZE);

	ASSERT_EQ(true,is_memset_to((addr + SIZE_CHUNK_HEADER), 0, LARGE_TEST_SIZE));
}

