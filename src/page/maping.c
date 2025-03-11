#include "malloc.h"
#include "sys/mman.h"

void drop_page(void *page, size_t size) {
	munmap(page, size);
	desindex_page(page);
}
