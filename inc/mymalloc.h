#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define MASK(x) x ## 000000000000

#define PREV_IN_USE MASK(0x01)
#define MMAPED MASK(0x02)
#define ALLOCATED_ARENA MASK(0x04)


void *mymalloc(size_t size);
void *mmap_call(void *addr, size_t length);

void toggle_mask(void *addr, uint64_t mask);
bool mask_is_set(void *addr, uint64_t mask); 
