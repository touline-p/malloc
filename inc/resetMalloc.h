#ifndef RESETMALLOC_H
# define RESETMALLOC_H

#include <utest.h>

#include "mymalloc.h"
#include "size.h"
#include "type.h"
#include "globals.h"
#include "maskmanipulation.h"

struct resetMalloc {
};

UTEST_F_SETUP(resetMalloc) {
	init_malloc();
	ASSERT_EQ(allocated_page, 0);
}

UTEST_F_TEARDOWN(resetMalloc) {
	unmap_all_page();
}

bool check_integrity(char mark, size_t size, void *addr) {
	for (size_t index = 0 ; index < size ; index++) {
		if (*(char *)addr != mark)
			return false;
	}
	return true;
}

#endif // !RESETMALLOC_H
