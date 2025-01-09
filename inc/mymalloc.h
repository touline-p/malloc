
#ifndef MYMALLOC_H
# define MYMALLOC_H
# include <stddef.h>
# include <stdbool.h>
# include <stdint.h>

# define ZONE_NUMBER 3

enum zone_e {
	TINY = 0,
	MEDIUM = 1,
	LARGE = 2
};

# define FONCTIONAL_NUMBER 18

enum zone_function_e {
	COMPARAISON = 0,
	TOP = 1,
	FREED = 2,
	FAST_ALLOC_PTR = 3,
	SIZE_MAX_ALLOC = 4,
	MESSAGE = 5,
};

# define COMP_CAST(x) ((bool (*) (uint64_t))(x))

# define MASK(x) x ## 00000000000000

# define OFFSET_HEADER 2
# define SIZE_CHUNK_HEADER sizeof(uint64_t) * OFFSET_HEADER

# define PREV_IN_USE MASK(0x01)
# define CHUNK_IN_USE MASK(0x02)
# define FIRST_IN_ZONE MASK(0x04)

# define LITTLE_TINY 16
# define BIGGEST_MEDIUM 4080

# define IS_TINY(x) x <= BIGGEST_TINY
# define IS_MEDIUM(x) x <= BIGGEST_MEDIUM
# define BIGGEST_TINY 16
# define TINY_TEST_SIZE BIGGEST_TINY - 4
# define TINIEST_TINY_SIZE LITTLE_TINY + SIZE_CHUNK_HEADER

# define MAX_PAGE 20

# define DEBUG_L

# ifdef DEBUG_L
#  define DEBUG(x) dprintf(2, x)
# else
#  define DEBUG(x)  ;
# endif // DEBUG

# ifdef INFO_L 
#  define INFO(x) dprintf(2, x)
# else
#  define INFO(x)  ;
# endif // DEBUG

void *mymalloc(size_t size);
void myfree(void *addr);
int try_init_page(void **to_init, size_t size);
void *mmap_call(void *addr, size_t length);
void *allocate_page(void *proximity);
void stock_and_reinit(void **to_stock, void **to_init, size_t size);
size_t size_allocation(size_t size);


void init_malloc(void);
void unmap_all_page(void);

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

typedef struct freed_chunk_s {
	chunk_info_t chunk_info_first;
	chunk_info_t chunk_info_second;
	void *next;
	void *prev;
} freed_chunk_t;


typedef struct arena_s {
	// mutex
	// bins

	void *top;
	void *tiny;
	void *free_tiny;
	void *medium;
	void *free_medium;
	void *big;
	void *free_big;
} arena_t;

typedef struct page_s {
	void *addr;
	size_t size;
} page_t;


#define CHNK_CLR(x) *x = 0
#define SIZE_MASK 0xffffffffff
#define INV_SIZE_MASK ~0xffffffffff
#define ASSIGN_SIZE(to_assign, assignee) *to_assign &= INV_SIZE_MASK; *to_assign |= (*assignee & SIZE_MASK)
#define GET_SIZE(to_get) (*(chunk_info_t *)to_get & SIZE_MASK)

#define NEXT_FREED_CHUNK(x) ((freed_chunk_t *)x)->next

extern arena_t arena_g;
extern page_t pages[MAX_PAGE];
extern size_t allocated_page;
extern size_t fast_alloc;


#endif
