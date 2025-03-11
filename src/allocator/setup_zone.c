#include "define.h"
#include "printf_ft.h"
#include "type.h"
#include "maskmanipulation.h"
#include "zone_management.h"
#include "display.h"

#include <unistd.h>
#include <sys/mman.h>


size_t zone_size(size_t chunk_size) {
	size_t size;
	size_t page_size;

	size = chunk_size * 100;
	page_size = sysconf(_SC_PAGESIZE);
	size = (size / page_size) * page_size + ((size % page_size) != 0) * page_size;
	return  size;
}

void *zone_allocator(freed_chunk_t **zone_ptr, size_t size_chunk)
{
	size_t size;
	int flags;
	int protections;

	size = zone_size(size_chunk);
	printf_ft("Allocating zone : zone size is %d\n", size);
	protections = PROT_WRITE | PROT_READ;
	flags = MAP_PRIVATE | MAP_ANONYMOUS;
	*zone_ptr = mmap(NULL, size, protections, flags, -1, 0);
	if (*zone_ptr == MAP_FAILED) {
		printf_ft("zone allocator failed\n");
		return MAP_FAILED;
	}
	setup_zone(*zone_ptr, size_chunk, size / size_chunk);
	printf_ft("indexation\n");
	index_page(*zone_ptr, size);
	return SUCCESS;
}

void setup_zone(void *dst, size_t chunk_size, size_t nb_alloc) {
	freed_chunk_t *pin;
	int nb = 0;

	pin = dst;
	pin->prev = NULL;
	set_mask((chunk_info_t*)pin, FIRST_IN_ZONE, true);
	set_mask((chunk_info_t*)pin, FIRST_IN_CHUNK, true);
	set_mask((chunk_info_t*)pin, LAST_IN_CHUNK, true);
	set_size((chunk_info_t*)pin, chunk_size);
	nb++;
	while (--nb_alloc)
	{
		nb++;
		pin->next = dst + chunk_size;
		set_mask((chunk_info_t*)pin, FIRST_IN_CHUNK, true);
		set_mask((chunk_info_t*)pin, LAST_IN_CHUNK, true);
		set_size((chunk_info_t*)pin, chunk_size);
		pin = pin->next;
		pin->prev = dst;
		dst += chunk_size;
	}
	pin->next = NULL;
	set_mask((chunk_info_t*)pin, LAST_IN_ZONE, true);
	set_mask((chunk_info_t*)pin, FIRST_IN_CHUNK, true);
	set_mask((chunk_info_t*)pin, LAST_IN_CHUNK, true);
	set_size((chunk_info_t*)pin, chunk_size);
	show_alloc_mem();
}

