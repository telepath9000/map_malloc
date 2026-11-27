#include "../include/map_malloc.h"

void *map_calloc(size_t nmemb, size_t size)
{
	size_t total;
	void *ptr;

	total = nmemb * size;
	ptr = map_malloc(total);
	if (ptr)
	{
		memset(ptr, 0, total);
	}
	return ptr;
}
