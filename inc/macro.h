#ifndef MACRO_H
# define MACRO_H

# define MIN(x, y) x < y ? x : y
# define MAX(x, y) x > y ? x : y
# define ROUND(x, y) (x / y) * y + ((x % y) != 0) * y

#endif // !MACRO_H
