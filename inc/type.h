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
	TINY,
	MEDIUM,
	LARGE,
	ZONE_NB,
};

# define FONCTIONAL_NUMBER 18

# define COMP_CAST(x) ((bool (*) (uint64_t))(x))
# define ALLOC_CAST(x) ((int (*) (void **, size_t, size_t))(x))
# define POST_FREE_CAST(x) ((void (*) (void *))(x))

enum zone_function_e {
	COMPARAISON,
	TOP,
	FREED,
	FAST_ALLOC_PTR,
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
