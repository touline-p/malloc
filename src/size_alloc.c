#include "size.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

bool is_tiny(size_t size) {
	return IS_TINY(size);
}

bool is_medium(size_t size) {
	return IS_MEDIUM(size);
}

bool is_big(size_t size) {
	return IS_BIG(size);
}

bool ret_true(size_t size) {
	(void)size;
	return true;
}

