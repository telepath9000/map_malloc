#include "../include/map_malloc.h"

void *free_or_realloc_type(void *ptr, t_mem_type type, int is_realloc, size_t size)
{
	t_unit  *cur;
	t_unit  *next;
	void    *ret;
	size_t  end;

	end = type == large ? 1 : 100;
	cur = get_list_of_type(type);
	ret = NULL;
	while (cur)
	{
		next = cur->next;
		ret = initialize_and_free_partition(ptr, cur, type, is_realloc, size, end);
		if (ret)
			return ret;
		cur = next;
	}
	return NULL;
}
