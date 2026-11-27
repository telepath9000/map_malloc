#include "../include/map_malloc.h"

void *initialize_and_free_partition(void *ptr, t_unit *chunk, t_mem_type type, int is_realloc, size_t size, size_t end)
{
	void    *ret;
	size_t  i;
	size_t  chunk_size;

	ret = NULL;
	i = 0;
	while (i < end)
	{
		if (chunk->table[i] && ptr == (void *)get_address(chunk, i, type))
		{
			if (type == large)
				chunk_size = get_alloc_size(chunk->table[i]);
			else if (type == small)
				chunk_size = get_alloc_size(SMALL_BYTES);
			else
				chunk_size = get_alloc_size(MED_BYTES);

			if (is_realloc)
			{
				ret = map_malloc(size);
				if (ret)
					malcpy(ret, ptr, chunk->table[i], size);
				else
					return NULL;
			}
			else
				ret = ptr;

			chunk->table[i] = 0;
			chunk->filled--;

			if (!chunk->filled)
				free_core(chunk, type, chunk_size);
			return ret;
		}
		i++;
	}
	return ret;
}
