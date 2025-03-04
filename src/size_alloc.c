#include "size.h"
#include "type.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

bool is_tiny(size_t size) {
	return size  <= (BIGGEST_TINY + sizeof(chunk_info_t));
}

bool is_medium(size_t size) {
	return size <= (BIGGEST_MEDIUM + sizeof(chunk_info_t));
}

bool is_big(size_t size) {
	return true;
}

bool ret_true(size_t size) {
	(void)size;
	return true;
}

