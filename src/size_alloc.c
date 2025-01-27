#include "size.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

bool is_tiny(size_t size) {
	dprintf(2, "%ld is tiny %d\n", size, IS_TINY(size));
	return IS_TINY(size);
}

bool is_medium(size_t size) {
	dprintf(2, "%ld is medium %d\n", size, IS_TINY(size));
	return IS_MEDIUM(size);
}

bool ret_true(size_t size) {
	(void)size;
	return true;
}

