#include "../include/map_malloc.h"

t_mem		*g_mem = NULL;

static void	*search_memory_med(int size)
{
	void	*ret;
	t_unit	*cur;
	t_med	*cur_med;
	int		i;

	ret = NULL;
	cur = g_mem->med;
	while (cur)
	{
		cur_med = cur->unit.med;
		i = (cur_med->filled < 50) ? 0 : 99;
		if (cur_med->filled < 100)
		{
			while (cur_med->filled < 50 && cur_med->table[i])
				i++;
			while (cur_med->filled >= 50 && cur_med->table[i])
				i--;
			cur_med->table[i] = size;
			cur_med->filled++;
			ret = (void *)get_address(cur, i, med);
			break ;
		}
		cur = cur->next;
	}
	return (ret);
}

static void	*search_memory_small(int size)
{
	void	*ret;
	t_unit	*cur;
	t_small	*cur_sml;
	int		i;

	ret = NULL;
	cur = g_mem->small;
	while (cur)
	{
		cur_sml = cur->unit.small;
		i = (cur_sml->filled < 50) ? 0 : 99;
		if (cur_sml->filled < 100)
		{
			while (cur_sml->filled < 50 && cur_sml->table[i])
				i++;
			while (cur_sml->filled >= 50 && cur_sml->table[i])
				i--;
			cur_sml->table[i] = size;
			cur_sml->filled++;
			ret = (void *)get_address(cur, i, small);
			break ;
		}
		cur = cur->next;
	}
	return (ret);
}

void		*map_malloc(size_t size)
{
	void	*ptr;

	ptr = NULL;
	if (check_limit(size))
		write(2, "ERROR: virtual memory space limit has been reached.\n", 53);
	else if (size > 0)
	{
		if (g_mem)
		{
			if (size <= SMALL_BYTES && g_mem->small)
				ptr = search_memory_small(size);
			else if (size <= MED_BYTES && g_mem->med)
				ptr = search_memory_med(size);
		}
		if (!ptr)
			ptr = alloc_core(size);
	}
	return (ptr);
}
