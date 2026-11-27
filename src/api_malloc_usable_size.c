#include "../include/map_malloc.h"

size_t map_malloc_usable_size(void *ptr)
{
	t_unit *cur;
	size_t end;

	if (!ptr || !g_mem)
		return 0;

	for (int i = 0; i < 3; i++)
	{
		cur = get_list_of_type(g_type_list[i]);
		end = (g_type_list[i] == large) ? 1 : 100;

		while (cur)
		{
			for (size_t j = 0; j < end; j++)
			{
				if (cur->table[j] && ptr == get_address(cur, j, g_type_list[i]))
					return cur->table[j];
			}
			cur = cur->next;
		}
	}
	return 0;
}
