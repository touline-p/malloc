#ifndef MYMALLOC_H
# define MYMALLOC_H
# include <stddef.h>
# include <stdbool.h>
# include <stdint.h>
# include "type.h"

# define OUTPUT_ERR_FD 2
# define DEBUG(x) write(OUTPUT_ERR_FD, x, ft_strlen(x));
# define DEBUG_char(x) { char c = '0' + x; write(OUTPUT_ERR_FD, &c, 1) ; write(OUTPUT_ERR_FD, "\n", 1); }
# define LINE write(OUTPUT_ERR_FD, "\n", 1);
void *malloc(size_t size);
void *mmap_call(void *proximity, size_t length);
int index_page(void *addr, size_t length);
void free(void *addr);
void *realloc(void *addr, size_t size);
int try_init_page(void **to_init, size_t size);
void *allocate_page(void *proximity);
void stock_and_reinit(void **to_stock, void **to_init, size_t size);
size_t size_allocation(size_t size);

size_t ft_strlen(char *str);
void init_malloc(void);
void clean_malloc(void);

void display_arena(void);
void display_free(freed_chunk_t *link);

void show_alloc_mem(void);
void print_chunk_info_t(chunk_info_t *chunk);
void print_info_from_addr(chunk_info_t *addr);
size_t cached_sysconf();
void display_indexed_page();


# define IGNORE_ARGUMENTS 0

#endif
