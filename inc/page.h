#ifndef PAGE_H
# define PAGE_H

#include <stddef.h>

void drop_page(void *page, size_t size);
void desindex_page(void *page);
int index_page(void *addr, size_t length);

#endif // !PAGE_H
