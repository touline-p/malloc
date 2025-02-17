#ifndef RESETMALLOC_H
# define RESETMALLOC_H

#include <stdio.h>
# include <sys/mman.h>
# include <utest.h>

# include "mymalloc.h"
# include "size.h"
# include "type.h"
# include "globals.h"
# include "maskmanipulation.h"

# define HEADER(addr) get_header_from_addr(addr)

struct resetMalloc {
};

UTEST_F_SETUP(resetMalloc) {
	init_malloc();
}

UTEST_F_TEARDOWN(resetMalloc) {
	clean_malloc();
}

bool check_integrity(char mark, size_t size, void *addr) {
	for (size_t index = 0 ; index < size ; index++) {
		if (*(char *)addr != mark)
			return false;
	}
	return true;
}

#endif // !RESETMALLOC_H
