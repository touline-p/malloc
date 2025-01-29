#ifndef ALLOCATION_H 
# define ALLOCATION_H

# include <stddef.h>

int allocate_memory_pool(void **to_init, size_t size);
int allocate_unique_zone(void **to_init, size_t max_size, size_t size);

#endif 

