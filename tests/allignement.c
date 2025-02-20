#include "malloc.h"
#include "testUtils.h"
#include "utest.h"
#include <stdint.h>
#include "resetMalloc.h"

UTEST_MAIN();

UTEST_F(resetMalloc, allignment_is_fine) {
	size_t size = 1;
	void **addr[size];

	for ( int a = 0 ; a < size ; a ++ ) {
		addr[a] = malloc(3 * sizeof(void *));
		ASSERT_EQ(0, ((uint64_t)addr[a] % 16));
		addr[a][0] = malloc(TINY_TEST_SIZE);
		ASSERT_EQ(0, (uint64_t)addr[a][0] % 16);
		addr[a][1] = malloc(MEDIUM_TEST_SIZE); 
		ASSERT_EQ(0, (uint64_t)addr[a][1] % 16);
		addr[a][2] = malloc(LARGE_TEST_SIZE);
		ASSERT_EQ(0, (uint64_t)addr[a][2] % 16);
	}
}
