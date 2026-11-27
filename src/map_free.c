#include "../include/map_malloc.h"

static void reduce_size(t_mem_type type)
{
	switch (type)
	{
		case small:
			g_mem->ssize--;
			break ;
		case med:
			g_mem->msize--;
			break ;
		case large:
			g_mem->lsize--;
	}
}

void free_core(t_unit *target, t_mem_type type, size_t size)
{
	t_unit **list_of_type_head;

	list_of_type_head = get_modifiable_list_of_type(type);
	if (!list_of_type_head)
		return;
	reduce_size(type);
	if (target->prev)
		target->prev->next = target->next;
	else
		*list_of_type_head = target->next;
	if (target->next)
		target->next->prev = target->prev;
	error_handle_munmap(target, size);
}

void map_free(void *ptr)
{
	if (ptr && g_mem)
	{
		for (int i = 0; i < 3; i++)
			if (free_or_realloc_type(ptr, g_type_list[i], 0, 0))
				break;
		if (!(g_mem->ssize + g_mem->msize + g_mem->lsize))
		{
			error_handle_munmap(g_mem, GLOBAL);
			g_mem = NULL;
		}
	}
}
