#include "libft.h"
#include "malloc.h"
#include "printf_ft.h"

int main(int ac, char **av) {
	int *addr = malloc(56);
	printf_ft("bonjour a vous tous\n");
	bzero_ft(addr, 56);
	return 3;
}
