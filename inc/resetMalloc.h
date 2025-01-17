#ifndef RESETMALLOC_H
# define RESETMALLOC_H

#include "utest.h"
#include "mymalloc.h"

struct resetMalloc {
};

UTEST_F_SETUP(resetMalloc) {
	init_malloc();
	ASSERT_EQ(allocated_page, 0);
}

UTEST_F_TEARDOWN(resetMalloc) {
	unmap_all_page();
}

#endif // !RESETMALLOC_H
