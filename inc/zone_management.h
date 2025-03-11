#ifndef ZONE_MANAGEMENT_H
# define ZONE_MANAGEMENT_H

# include "stddef.h"
#include "type.h"

size_t zone_size(size_t chunk_size);
void setup_zone(void *dst, size_t chunk, size_t nb_alloc);
int index_page(void *addr, size_t length);
void desindex_page(void *header, size_t size);
void *zone_allocator(freed_chunk_t **zone_ptr, size_t size_chunk);


#endif
