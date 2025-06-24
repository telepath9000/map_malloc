#include "../include/map_malloc.h"

t_mem *g_mem = NULL;
t_mem_type g_type_list[3] = {small, med, large};

static void *find_available_partition(t_unit *chunk, int size, t_mem_type type)
{
	int i;

	i = 0;
	while (chunk->table[i])
		i++;
	chunk->table[i] = size;
	chunk->filled++;
	return (void *)get_address(chunk, i, type);
}

static void *search_memory(int size, t_mem_type type)
{
	t_unit *cur;

	if (type == large)
		write(2, "ERROR: large allocations do not require partitioning.\n", 55);
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
	if (check_limit(size))
		write(2, "ERROR: virtual memory space limit has been reached.\n", 53);
	if (g_mem && size <= SMALL_BYTES && g_mem->small)
		ptr = search_memory(size, small);
	else if (g_mem && size <= MED_BYTES && g_mem->med)
		ptr = search_memory(size, med);
	if (!ptr)
		ptr = alloc_core(size);
	return ptr;
}
