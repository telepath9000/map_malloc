#include "../include/map_malloc.h"
#include <stdio.h>

t_mem *g_mem = NULL;
t_mem_type g_type_list[3] = {small, med, large};

static void *find_available_partition(t_unit *chunk, size_t size, t_mem_type type)
{
	size_t	i;

	i = 0;
	while (chunk->table[i])
		i++;
	chunk->table[i] = size;
	chunk->filled++;
	printf("%zu", (size_t)get_address(chunk, i, type));
	return (void *)get_address(chunk, i, type);
}

static void *search_memory(size_t size, t_mem_type type)
{
	t_unit *cur;

	if (type == large &&
		write(2, "ERROR: large allocations do not require partitioning.\n", 55) < 0)
		return NULL;
	cur = get_list_of_type(type);
	while (cur)
	{
		if (cur->filled < 100)
			return find_available_partition(cur, size, type); /* side effects on cur */
		cur = cur->next;
	}
	return NULL;
}

void *map_malloc(size_t size)
{
	void *ptr;

	ptr = NULL;
	if (check_limit(size) &&
		write(2, "ERROR: virtual memory space limit has been reached.\n", 53) < 0)
		return NULL;
	if (g_mem && size <= SMALL_BYTES && g_mem->small)
		ptr = search_memory(size, small);
	else if (g_mem && size <= MED_BYTES && g_mem->med)
		ptr = search_memory(size, med);
	if (!ptr)
		ptr = alloc_core(size);
	return ptr;
}
