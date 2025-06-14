#include "../include/map_malloc.h"

static void	init_mem(void)
{
	g_mem = mmap(0, GLOBAL,
			PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	g_mem->small = NULL;
	g_mem->stail = NULL;
	g_mem->med = NULL;
	g_mem->mtail = NULL;
	g_mem->large = NULL;
	g_mem->ltail = NULL;
	g_mem->ssize = 0;
	g_mem->msize = 0;
	g_mem->lsize = 0;
	g_mem->total_mem = 0;
}

t_unit	*find_slot(t_unit	*chunk, size_t type)
{
	t_unit	*ret;

	if (type == SMALL_BYTES)
		ret = g_mem->small;
	else if (type == MED_BYTES)
		ret = g_mem->med;
	else
		ret = g_mem->large;
	if (ret)
	{
		while (ret->next && chunk > ret)
			ret = ret->next;
		chunk->next = ret->next;
	}
	return (ret);
}

t_unit	*init_chunk(t_unit *mem, size_t type)
{
	t_unit	*ret;

	if (!g_mem)
		init_mem();
	mem->prev = NULL;
	mem->next = NULL;
	if (type == SMALL_BYTES)
  {
    ret = (t_unit *)((char *)mem + SMALL_ALLOC);
		mem->unit.small = (t_small *)((char *)mem + sizeof(t_unit));
    mem->unit.small->filled = 0;
    memset(mem->unit.small->table, 0, sizeof(mem->unit.small->table));
	}
	else if (type == MED_BYTES)
	{
    ret = (t_unit *)((char *)mem + MED_ALLOC);
		mem->unit.med = (t_med *)((char *)mem + sizeof(t_unit));
    mem->unit.med->filled = 0;
    memset(mem->unit.med->table, 0, sizeof(mem->unit.med->table));
	}
	else
	{
    ret = (t_unit *)((char *)mem + LARGE_ALLOC);
		mem->unit.large = (t_large *)((char *)mem + sizeof(t_unit));
		mem->unit.large->size = type;
	}
	return ret;
}

void	set_tail(t_unit *chunk, size_t type)
{
	t_unit	*tail;

	if (g_mem)
	{
		if (type == SMALL_BYTES)
			tail = g_mem->stail;
		else if (type == MED_BYTES)
			tail = g_mem->mtail;
		else
			tail = g_mem->ltail;
		if (!tail || (tail && tail->next == chunk))
			tail = chunk;
	}
}
