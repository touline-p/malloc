#include <stdbool.h>

#include "size.h"
#include "utest.h"
#include "resetMalloc.h"
#include "testUtils.h"
#include "allocation.h"

UTEST_MAIN();

UTEST_F(resetMalloc, try_init_adjusted_alloc_a_usable_addr) {
	void *addr; 
	allocate_unique_zone(&addr, 0, LARGE_TEST_SIZE);

	ASSERT_EQ(true,is_memset_to((addr + SIZE_CHUNK_HEADER), 0, LARGE_TEST_SIZE - SIZE_CHUNK_HEADER));
}

