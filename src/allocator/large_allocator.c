
#include "printf_ft.h"
#include "stddef.h"
#include "sys/mman.h"
#include "zone_management.h"
#include "page.h"


void *large_allocator(size_t size)
{
	int protections;
	int flags;
	void *zone_ptr;

	protections = PROT_WRITE | PROT_READ;
	flags = MAP_PRIVATE | MAP_ANONYMOUS;
	zone_ptr = mmap(NULL, size, protections, flags, -1, 0);
	if (MAP_FAILED == zone_ptr) {
		printf_ft("this failed\n");
		return NULL;
	}
	setup_zone(zone_ptr, size, 1);
	index_page(zone_ptr, size);
	return zone_ptr;
}

void large_unallocator(void *header, size_t size)
{
	drop_page(header, size);
}
