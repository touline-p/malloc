#ifndef MYMALLOC_H
# define MYMALLOC_H
# include <stddef.h>
# include <stdbool.h>
# include <stdint.h>




void *mymalloc(size_t size);
void myfree(void *addr);
int try_init_page(void **to_init, size_t size);
void *mmap_call(void *addr, size_t length);
void *allocate_page(void *proximity);
void stock_and_reinit(void **to_stock, void **to_init, size_t size);
size_t size_allocation(size_t size);

// size_allocation


void init_malloc(void);
void unmap_all_page(void);



#endif
