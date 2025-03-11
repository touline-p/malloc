#include "libft.h"

#include <stddef.h>
#include <string.h>

void *memset_schema_ft(void *dst, void *schema, size_t size_dst, size_t size_src) {
	while (size_dst > size_src) {
		memcpy(dst, schema, size_src);
		dst += size_src;
		size_dst -= size_src;
	}
	memcpy(dst, schema, size_dst);
	return dst;
}
