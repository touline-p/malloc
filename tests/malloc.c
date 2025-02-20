#include "malloc.h"
#include "resetMalloc.h"
#include "utest.h"
#include <string.h>

UTEST_MAIN();

UTEST_F(resetMalloc, malloc_return_null_on_size_zero) {
	ASSERT_EQ(malloc(0), NULL);
}
