#include "../include/map_malloc.h"

int map_posix_memalign(void **memptr, size_t alignment, size_t size)
{
	void *ptr;

	if (alignment % sizeof(void *) != 0 || (alignment & (alignment - 1)) != 0)
		return 22; 

	ptr = map_malloc(size);
	if (!ptr)
		return 12; 

	*memptr = ptr;
	return 0;
}
