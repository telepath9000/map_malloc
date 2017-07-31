#include "wdmalloc.h"

void	*alloc_large(size_t size)
{
	void	*ret;
	size_t	len;

	len = (size % getpagesize()) != 0 ? ((size / getpagesize()) + 1) * ;
	ret = mmap(ret, len, 

void	*wd_malloc(size_t size)
{
	void	*ret;

	ret = NULL;
	if (size == 0)
		return (NULL);
	else if (size < TINY)
		ret = alloc_tiny(size);
	else if (size < SMALL)
		ret = alloc_small(size);
	else
		ret = alloc_large(size);
	return (ret);
}
