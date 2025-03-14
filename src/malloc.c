#include "malloc.h"
#include "intern_allocator.h"
#include "define.h"
#include "macro.h"

#include "maskmanipulation.h"
#include "printf_ft.h"
#include "libft.h"
#include "type.h"
#include "pthread.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

arena_t arena_g = {.pages_arr = NULL, .page_nb = 0, .tiny_list = NULL, .medium_list = NULL};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
bool ava = true;

void wait_for_mutex() {
	bool mine = false;

	

	while (true) {
		pthread_mutex_lock(&mutex);
		if (ava == true) {
			ava = false;
			break;
		}
		usleep(5);
		pthread_mutex_unlock(&mutex);
	}
	pthread_mutex_unlock(&mutex);
}

void release_mutex() {
	pthread_mutex_lock(&mutex);
	ava = true;
	pthread_mutex_unlock(&mutex);
}

void print_zone(int *addr, int *end) {
	while (addr < end) {
		printf_ft("%10d ", *addr);
		addr++;
	}
}

void *malloc(size_t len) {
	static int nb_alloc = 0;
	void *ret_val;

	printf_ft("-- MALLOC %d\n", len);
	wait_for_mutex();
	if (len <= TINY_MAX_LEN)
		ret_val = tiny_allocator(len + HEADER_LEN);
	else if (len <= MEDIUM_MAX_LEN)
		ret_val = medium_allocator(len + HEADER_LEN);
	else 
		ret_val = large_allocator(len + HEADER_LEN);
	release_mutex();
	set_mask(ret_val, CHUNK_IN_USE, true);
	ret_val = get_addr_from_header(ret_val);
	release_mutex();
	//show_alloc_mem();
	return ret_val;
}

void free(void *addr) {
	void *header;
	size_t size;


	if(addr == NULL) {
		return ;
	}

	printf_ft("--FREE %p\n", addr);
	header = get_header_from_addr(addr);
	set_mask(header, CHUNK_IN_USE, false);
	size = get_size(header);
	wait_for_mutex();
	if (size <= TINY_MAX_SIZE)
		tiny_unallocator(header, size);
	else if (size <= MEDIUM_MAX_SIZE)
		medium_unallocator(header, size);
	else
		large_unallocator(header, size);
	release_mutex();
	//show_alloc_mem();
}

void *realloc(void *addr, size_t new_size) {
	void *header;
	size_t size;
	void *dst;

	printf_ft("--REALLOC -- %p ", addr);
	if (addr == NULL)
		return malloc(new_size);
	if (new_size == 0)
		return (free(addr), NULL);
	header = get_header_from_addr(addr);
	size = get_size(header);
	dst = malloc(new_size);
	memcpy_ft(dst, addr, MIN(size - HEADER_LEN, new_size));
	free(addr);
	return dst;
}

void *calloc(size_t nb, size_t size_of) {
	void *addr;

	addr = malloc(size_of * nb);
	bzero_ft(addr, size_of * nb);
	return addr;
}

void *reallocarray(void *ptr, int nb, int size) {
	return realloc(ptr, nb * size);
}
