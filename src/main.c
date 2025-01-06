#include <stdlib.h>
#include "mymalloc.h"

#include <sys/mman.h>

void *mymalloc(size_t size) {
	return mmap_call(NULL, 8);
}

// On utilise PROT_WRITE et PROT_READ meme si pour certaines archi le WRITE 
// implique le READ
//
void *mmap_call(void *addr, size_t length) {
	return mmap(addr, length, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}
