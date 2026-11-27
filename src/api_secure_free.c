#include "../include/map_malloc.h"

void map_secure_free(void *ptr)
{
	size_t block_size;

	if (ptr)
	{
		block_size = map_malloc_usable_size(ptr);
		if (block_size > 0)
		{
			memset(ptr, 0, block_size);
		}
		map_free(ptr);
	}
}
