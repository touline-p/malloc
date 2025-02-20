#ifndef MASKMANIPULATION_H
# define MASKMANIPULATION_H

# include <stdint.h>
# include <stdbool.h>

# include "type.h"

# define MASK(x) x ## 00000000000000

# define PREV_IN_USE MASK(0x01)
# define CHUNK_IN_USE MASK(0x02)
# define FIRST_IN_ZONE MASK(0x04)
# define LAST_IN_ZONE MASK(0x08)

# define NEXT_CHUNK_IN_ZONE(header) (void *)(header + GET_SIZE(header))

# define CHNK_CLR(x) *x = 0
# define SIZE_MASK 0xffffffffff
# define INV_SIZE_MASK ~0xffffffffff
# define ASSIGN_SIZE(to_assign, assignee) *to_assign &= INV_SIZE_MASK; *to_assign |= (*assignee & SIZE_MASK)
# define GET_SIZE(to_get) (*(chunk_info_t *)to_get & SIZE_MASK)
# define GET_USE_SIZE(to_get) (((chunk_info_t *)to_get)[1] & SIZE_MASK)
# define SET_USE_SIZE(to_set, size) (((chunk_info_t *)to_set)[1] = size)
# define NEXT_FREED_CHUNK(x) ((freed_chunk_t *)x)->next

void toggle_mask(void *addr, uint64_t mask);
bool mask_is_set(void *addr, uint64_t mask); 
uint64_t *get_header_from_addr(void *header);
uint64_t *get_addr_from_header(void *addr);

#endif
