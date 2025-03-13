#include "malloc.h"
#include "intern_allocator.h"
#include "define.h"
#include "macro.h"

#include "maskmanipulation.h"
#include "printf_ft.h"
#include "libft.h"
#include "type.h"

arena_t arena_g = {.pages_arr = NULL, .page_nb = 0, .tiny_list = NULL, .medium_list = NULL};

void print_zone(int *addr, int *end) {
	while (addr < end) {
		printf_ft("%10d ", *addr);
		addr++;
	}

}

void *malloc(size_t len) {
	printf_ft("MALLOC ---> %8d   ", len);
	static int nb_alloc = 0;
	void *ret_val;
	bool flag = false;
	if (len <= TINY_MAX_LEN)
		ret_val = tiny_allocator(len + HEADER_LEN);
	else if (len <= MEDIUM_MAX_LEN) {
		flag = true;
		ret_val = medium_allocator(len + HEADER_LEN);
	} else 
		ret_val = large_allocator(len + HEADER_LEN);
	ret_val = get_addr_from_header(ret_val);
	printf_ft("end ret_val %p %d\n", ret_val, ++nb_alloc);
	printf_ft("\n");
	display_free(arena_g.medium_list);
	printf_ft("\n");

	return ret_val;
}

void free(void *addr) {
	printf_ft("FREE ---> %p", addr);
	void *header;
	size_t size;

	if(addr == NULL)
		return ;

	header = get_header_from_addr(addr);
	size = get_size(header);
	if (size <= TINY_MAX_SIZE) {
		tiny_unallocator(header, size); printf_ft("tiny");
	} else if (size <= MEDIUM_MAX_SIZE) {
		medium_unallocator(header, size); printf_ft("mid");
	} else {
		large_unallocator(header, size); printf_ft("large");
	}
	printf_ft(" -- end\n");
	printf_ft("\n");
	display_free(arena_g.medium_list);
	printf_ft("\n");
	
}

void *realloc(void *addr, size_t new_size) {
	printf_ft("REALLOC ---> %p to size %d", addr, new_size);
	void *header;
	size_t size;


	if (addr == NULL)
		return malloc(new_size);
	if (new_size == 0)
		return (free(addr), NULL);
	header = get_header_from_addr(addr);
	size = get_size(addr);
	void *dst = malloc(new_size);
	memcpy_ft(dst, addr, MIN(size - HEADER_LEN, new_size));
	free(addr);
	printf_ft("end realloc\n");
	return dst;
}

void *calloc(size_t size) {
	printf_ft("calloc %d\n", size);
	void *addr;

	addr = malloc(size);
	bzero_ft(addr, size);
	return addr;
}
