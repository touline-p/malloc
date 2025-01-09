#include "utest.h"
#include "mymalloc.h"

arena_t arena_g;

UTEST_MAIN()

UTEST(arena_init, init_pre_main) {
	ASSERT_EQ(arena_g.top, NULL);
}

UTEST(arena_init, top_alloc_is_done_at_init) {
	ASSERT_EQ(allocated_page, 0);
}
