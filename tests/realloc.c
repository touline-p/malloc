#include "utest.h"
#include "resetMalloc.h"
#include "mymalloc.h"

UTEST_MAIN();

UTEST_F(resetMalloc, malloc_page_has_usable_size) {
	size_t size = 12;
	void *addr = mymalloc(size);
	size_t upper_size = size_allocation(size);

	void *header = get_header_from_addr(addr);
	ASSERT_EQ(size, GET_USE_SIZE(header));
	ASSERT_EQ(upper_size, GET_SIZE(header));
}
