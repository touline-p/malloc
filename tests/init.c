#include "utest.h"
#include "mymalloc.h"

arena_t arena_g;

UTEST_MAIN()

UTEST(arena_init, init_pre_main) {
	ASSERT_NE(arena_g.top, NULL);
	int * ptr = arena_g.top;
	*ptr = 5;
}