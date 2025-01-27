#ifndef GLOBAL_H
# define GLOBAL_H

# include "type.h"

# define MAX_PAGE 20

extern arena_t arena_g;
extern page_t pages[MAX_PAGE];
extern size_t allocated_page;
extern size_t fast_alloc;

#endif // !GLOBAL_H
