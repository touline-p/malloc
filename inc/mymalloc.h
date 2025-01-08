#ifndef MYMALLOC_H
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


#define MASK(x) x ## 00000000000000

#define OFFSET_HEADER 2

#define PREV_IN_USE MASK(0x01)
#define CHUNK_IN_USE MASK(0x02)

void *mymalloc(size_t size);
void *mmap_call(void *addr, size_t length);
void *allocate_page(void *proximity);

void toggle_mask(void *addr, uint64_t mask);
bool mask_is_set(void *addr, uint64_t mask); 

uint64_t *get_header_from_addr(uint64_t *header);
uint64_t *get_addr_from_header(uint64_t *header);

typedef struct chain_s chain_t;
struct chain_s {
	chain_t *chain;
	void *chunk;
};

typedef uint64_t chunk_info_t;

typedef struct arena_s {
	// mutex
	// bins
	size_t nb_alloc;

	chain_t *avalaible; //chain of disponible chunks
	chunk_info_t *top;
} arena_t;

#define CHNK_CLR(x) *x = 0
#define SIZE_MASK 0xffffffffff
#define INV_SIZE_MASK ~0xffffffffff
#define ASSIGN_SIZE(to_assign, assignee) *to_assign &= INV_SIZE_MASK; *to_assign |= (*assignee & SIZE_MASK)
#define GET_SIZE(to_get) *to_get & SIZE_MASK

extern arena_t arena_g;


#endif
