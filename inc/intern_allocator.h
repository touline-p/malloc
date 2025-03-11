#ifndef INTERN_ALLOCATOR_H
# define INTERN_ALLOCATOR_H

# include "stddef.h"
#include "type.h"

void *tiny_allocator(size_t size);
void *medium_allocator(size_t size);
void *large_allocator(size_t size);
void tiny_unallocator(freed_chunk_t *header, size_t size);
void medium_unallocator(freed_chunk_t *header, size_t size);
void large_unallocator(freed_chunk_t *header, size_t size);

#endif
