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

typedef struct freed_chunk_s {
	chunk_info_t chunk_info_first;
	chunk_info_t chunk_info_second;
	void *next;
	void *prev;
} freed_chunk_t;

typedef struct arena_s {
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

enum zone_e {
	TINY = 0,
	MEDIUM = 1,
	LARGE = 2,
	ZONE_NB = 3
};

# define FONCTIONAL_NUMBER 18

# define COMP_CAST(x) ((bool (*) (uint64_t))(x))
# define ALLOC_CAST(x) ((int (*) (void **, size_t))(x))

enum zone_function_e {
	COMPARAISON = 0,
	TOP = 1,
	FREED = 2,
	FAST_ALLOC_PTR = 3,
	SIZE_MAX_ALLOC = 4,
	MESSAGE = 5,
	ALLOCATION_FN = 6,
};

#endif
