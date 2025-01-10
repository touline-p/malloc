#include <stdlib.h>
#include <stdbool.h>

bool is_memset_to(char *ptr, char payload, size_t size) { 
	size_t index = 0;

	while (index < size) {
		if (ptr[index] != payload)
			return false;
		++index;
	}
	return true;
}

