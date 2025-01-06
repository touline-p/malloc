#include "mymalloc.h"

bool chunk_is_allocated_arena(void *chunk) {
	return false;
}

bool chunk_is_mmaped(void *chunk) {
	return false;
}

bool chunk_is_previously_used(void *chunk) {
	return false;
}
