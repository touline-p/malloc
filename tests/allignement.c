#include "mymalloc.h"
#include "testUtils.h"
#include "utest.h"
#include <stdint.h>
#include "resetMalloc.h"

UTEST_MAIN();

UTEST_F(resetMalloc, allignment_is_fine) {
	size_t size = 8;
	void **addr[size];

	for ( int a = 0 ; a < size ; a ++ ) {
		addr[a] = mymalloc(3 * sizeof(void *));
		ASSERT_EQ(0, ((uint64_t)addr[a] % 16));
		addr[a][0] = mymalloc(TINY_TEST_SIZE);
		ASSERT_EQ(0, (int)addr[a][0] % 16);
		addr[a][1] = mymalloc(MEDIUM_TEST_SIZE); 
		ASSERT_EQ(0, (int)addr[a][1] % 16);
		addr[a][2] = mymalloc(LARGE_TEST_SIZE);
		ASSERT_EQ(0, (int)addr[a][2] % 16);
	}
}
