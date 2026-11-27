#include "../include/map_malloc.h"

void *map_reallocarray(void *ptr, size_t nmemb, size_t size)
{
	size_t total;
	size_t current_size;

	if (size && nmemb > ~(size_t)0 / size)
		return NULL;

	total = nmemb * size;

	if (ptr)
	{
		current_size = map_malloc_usable_size(ptr);
		if (current_size >= total)
			return ptr;
	}

	return map_realloc(ptr, total);
}
