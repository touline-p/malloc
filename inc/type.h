#ifndef TYPE_H
# define TYPE_H

# include "stddef.h"
# include "stdint.h"

typedef struct chain_s chain_t;
struct chain_s {
	chain_t *chain;
	void *chunk;
};

typedef uint64_t chunk_info_t;

typedef struct freed_chunk_s freed_chunk_t;

struct freed_chunk_s {
	chunk_info_t chunk_info_first;
	chunk_info_t chunk_info_second;
	freed_chunk_t *next;
	freed_chunk_t *prev;
};

enum zone_e {
	TINY,
	MEDIUM,
	LARGE,
	ZONE_NB,
};

typedef struct page_info_s page_info_t;

struct page_info_s {
	void *ptr;
	size_t length;
	enum zone_e zone;
};

typedef struct page_chain_s page_chain_t;

typedef struct arena_s {
	void *tiny;
	void *free_tiny;
	void *medium;
	void *free_medium;
	void *big;
	void *free_big;
	page_info_t *pages_arr;
	size_t page_nb;
} arena_t;

typedef struct page_s {
	void *addr;
	size_t size;
} page_t;




# define FIRST_POOL TINY
# define POOL_NB MEDIUM + 1

# define FONCTIONAL_NUMBER 18

# define COMP_CAST(x) ((bool (*) (uint64_t))(x))
# define ALLOC_CAST(x) ((int (*) (void **, size_t, size_t))(x))
# define POST_FREE_CAST(x) ((void (*) (void *))(x))

enum zone_function_e {
	COMPARAISON,
	TOP,
	FREED,
	FAST_ALLOC_PTR,
	SIZE_MIN_ALLOC,
	SIZE_MAX_ALLOC,
	MESSAGE,
	ALLOCATION_FN,
	ZONE_FUNCTION_NB
};

enum free_function_e {
	COMPARAISON_F,
	MESSAGE_F,
	FREED_F,
	POST_FREE_FN_F,
	FUNCTION_NB_F
};

#endif
