#ifndef DISPLAY_H
# define DISPLAY_H

# include "type.h"

void show_alloc_mem(void);
void display_arena(void);
void display_free(freed_chunk_t *link);

#endif // !DISPLAY_H
