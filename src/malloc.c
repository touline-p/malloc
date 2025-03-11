#include "malloc.h"
#include "intern_allocator.h"
#include "define.h"
#include "macro.h"

#include "maskmanipulation.h"
#include "printf_ft.h"
#include "libft.h"
#include "type.h"
#include <string.h>

arena_t arena_g = {};

void check_page(page_info_t page) {
	void *tmp = page.ptr;
	while (mask_is_set(tmp, LAST_IN_ZONE) == false)
	{
		size_t size = get_size(tmp);
		if (size == 0) {
			printf_ft("%p ptr\n", tmp);
			printf_ft("this is not normal we have to go check it\n");
			void **ptr = NULL;
			*ptr = "youpi";
		}
		tmp += get_size(tmp);
	}
}

void panic_check() {
	for (int i = 0 ; i < arena_g.page_nb ; i++) {
		check_page(arena_g.pages_arr[i]);
	}
}

void *malloc(size_t len) {
	void *ret_val;
	printf_ft("\n\n--ALLOC--\n");

	printf_ft("allocating, %d\n", len);
	if (len <= TINY_MAX_LEN)
		ret_val = tiny_allocator(len + HEADER_LEN);	
	else if (len <= MEDIUM_MAX_LEN) {
		ret_val = medium_allocator(len + HEADER_LEN);
	} else
	 	ret_val = large_allocator(len + HEADER_LEN);

	panic_check();
	return get_addr_from_header(ret_val);
}

void free(void *addr) {
	printf_ft("\n\n--FREE--\n");
	void *header;
	size_t size;

	if(addr == NULL)
		return ;

	header = get_header_from_addr(addr);
	printf_ft("unallocating, %p --", addr);
	size = get_size(header);
	printf_ft("size : %d\n", size);
	if (size <= TINY_MAX_SIZE)
		tiny_unallocator(header, size);
	else if (size <= MEDIUM_MAX_SIZE)
		medium_unallocator(header, size);
	else
		large_unallocator(header, size);
	panic_check();
}

void *realloc(void *addr, size_t new_size) {
	printf_ft("\n\n--REALLOC\n");
	printf_ft("reallocating, %p %d\n", addr, new_size);


	if (addr == NULL)
		return malloc(new_size);

	void *dst = malloc(new_size);
	size_t size = get_size(get_header_from_addr(addr));

	printf_ft("recopying %d \n", MIN(size, new_size));
	memcpy_ft(dst, addr, MIN(size, new_size));
	free(addr);
	printf_ft("return %p\n", dst);
	return dst;
}
