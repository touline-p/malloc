#ifndef MYMALLOC_H
# define MYMALLOC_H
# include <stddef.h>
# include <stdbool.h>
# include <stdint.h>
# include "type.h"

void *mymalloc(size_t size);
void *mmap_call(void *proximity, size_t length);
int index_page(void *addr, size_t length);
void myfree(void *addr);
void *myrealloc(void *addr, size_t size);
int try_init_page(void **to_init, size_t size);
void *allocate_page(void *proximity);
void stock_and_reinit(void **to_stock, void **to_init, size_t size);
size_t size_allocation(size_t size);

void init_malloc(void);
void clean_malloc(void);

void display_arena(void);
void display_free(freed_chunk_t *link);

void show_alloc_mem(void);


# define IGNORE_ARGUMENTS 0

#endif
