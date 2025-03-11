#ifndef MASKMANIPULATION_H
# define MASKMANIPULATION_H

# include <stdint.h>
# include <stdbool.h>

# include "type.h"

void toggle_mask(chunk_info_t *addr, uint64_t mask);
bool mask_is_set(chunk_info_t *addr, uint64_t mask); 
void set_mask(chunk_info_t *addr, size_t mask, bool dst);
uint64_t *get_header_from_addr(void *header);
uint64_t *get_addr_from_header(void *addr);

size_t get_size(chunk_info_t *addr);
void set_size(chunk_info_t *addr, size_t size);
#endif
