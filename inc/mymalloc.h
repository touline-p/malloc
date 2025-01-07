#ifndef MYMALLOC_H
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


#define MASK(x) x ## 00000000000000


#define PREV_IN_USE MASK(0x01)

void *mymalloc(size_t size);
void *mmap_call(void *addr, size_t length);
void *allocate_page(void *proximity);

void toggle_mask(void *addr, uint64_t mask);
bool mask_is_set(void *addr, uint64_t mask); 


typedef struct chain_s chain_t;
struct chain_s {
	chain_t *chain;
	void *chunk;
};

typedef struct arena_s {
	// mutex
	// bins

	chain_t *avalaible; //chain of disponible chunks
	void *top;
} arena_t;

typedef uint64_t chunk_info_t;
#define CHNK_CLR(x) *x = 0
#define SIZE_MASK 0xffffffffff
#define INV_SIZE_MASK ~0xffffffffff
#define ASSIGN_SIZE(to_assign, assignee) *to_assign &= INV_SIZE_MASK; *to_assign |= (*assignee & SIZE_MASK)
#define GET_SIZE(to_get) *to_get & SIZE_MASK

extern arena_t arena_g;


#endif
