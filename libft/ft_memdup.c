#include "malloc.h"
#include "libft.h"

void	*memdup_ft(void *src, size_t ln)
{
	void	*dst;

	dst = malloc(ln);
	if (!dst)
		return (NULL);
	memcpy_ft(dst, src, ln);
	return (dst);
}
