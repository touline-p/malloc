#include "define.h"
#include "malloc.h"
#include "stddef.h"
#include "sys/mman.h"
#include "zone_management.h"
#include "maskmanipulation.h"
#include "page.h"
#include <unistd.h>
#include "macro.h"


void *large_allocator(size_t size)
{
	size_t page_size;
	int protections;
	int flags;
	void *zone_ptr;

	page_size = sysconf(_SC_PAGESIZE);
	size = ROUND(size, page_size);
	size = (size / page_size) * page_size + ((size % page_size) != 0) * page_size;

	protections = PROT_WRITE | PROT_READ;
	flags = MAP_PRIVATE | MAP_ANONYMOUS;
	zone_ptr = mmap(NULL, size, protections, flags, -1, 0);
	if (MAP_FAILED == zone_ptr) {
		return NULL;
	}
	setup_zone(zone_ptr, size, 1);
	index_page(zone_ptr, size);
	set_mask(zone_ptr, CHUNK_IN_USE, true);
	return zone_ptr;
}

void large_unallocator(void *header, size_t size)
{
	drop_page(header, size);
}
