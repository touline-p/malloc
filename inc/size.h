#ifndef SIZE_H
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

# define SIZE_H

# define LITTLE_TINY 16

# define SMALLEST_TINY 0
# define BIGGEST_TINY 16
# define SMALLEST_MEDIUM 17
# define BIGGEST_MEDIUM 4080

# define TINY_TEST_SIZE BIGGEST_TINY - 4
# define MEDIUM_TEST_SIZE BIGGEST_MEDIUM - 1000
# define LARGE_TEST_SIZE 16000

# define OFFSET_HEADER 2
# define SIZE_CHUNK_HEADER sizeof(uint64_t) * OFFSET_HEADER

# define IS_TINY(x) x <= BIGGEST_TINY
# define IS_MEDIUM(x) x <= BIGGEST_MEDIUM && x > BIGGEST_TINY
# define IS_BIG(x) x > BIGGEST_MEDIUM
# define TINIEST_TINY_SIZE LITTLE_TINY + SIZE_CHUNK_HEADER 

# define ALLOC_BY_ZONE 100

bool is_tiny(size_t size);
bool is_medium(size_t size);
bool is_big(size_t size);
bool ret_true(size_t size);


#endif
