#include <stddef.h>

#include "mymalloc.h"
#include "maskmanipulation.h"

void *myrealloc(void *addr, size_t size) {
	(void)size;
	myfree(addr);
	return mymalloc(size);
	//SET_USE_SIZE(get_header_from_addr(addr), size);
	//return addr;
}
