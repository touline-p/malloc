#include "sys/mman.h"
#include "page.h"

void drop_page(void *page, size_t size) {
	munmap(page, size);
	desindex_page(page);
}
