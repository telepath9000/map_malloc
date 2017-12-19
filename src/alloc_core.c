/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebs <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/05 17:14:44 by wdebs             #+#    #+#             */
/*   Updated: 2017/08/07 17:37:56 by wdebs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_malloc.h"

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
	set_limit(GLOBAL, 1);
}

static void	init_chunk(void *memory, int type, size_t size)
{
	int	i;

	i = 0;
	if (!g_mem)
		init_mem();
	if (type == SMALL_BYTES && (((t_small *)memory)->table[0] = 1) &&
			(((t_small *)memory)->filled = 1) && ++g_mem->ssize &&
			(((t_small *)memory)->data = memory + SMALL_ALLOC))
		((t_small *)memory)->next = NULL;
	if (type == MED_BYTES && (((t_med *)memory)->table[0] = 1) &&
			(((t_med *)memory)->filled = 1) && ++g_mem->msize &&
			(((t_med *)memory)->data = memory + MED_ALLOC))
		((t_med *)memory)->next = NULL;
	if (type == LARGE && ++g_mem->lsize)
	{
		((t_large *)memory)->next = NULL;
		((t_large *)memory)->size = size;
		((t_large *)memory)->data = memory + LARGE_ALLOC;
	}
	while (type == SMALL_BYTES && ++i < 13)
		((t_small *)memory)->table[i] = 0;
	while (type == MED_BYTES && ++i < 13)
		((t_med *)memory)->table[i] = 0;
}

static void	*place_memory(void *memory, int type, size_t size)
{
	void	*ptr;

	init_chunk(memory, type, size);
	(type == LARGE) ? set_limit(size, 1) : set_limit(type, 1);
	if (type == LARGE && !g_mem->large)
		g_mem->large = (t_large *)memory;
	if (type == LARGE && g_mem->ltail)
		g_mem->ltail->next = (t_large *)memory;
	if (type == LARGE && (g_mem->ltail = (t_large *)memory))
		ptr = g_mem->ltail->data;
	if (type == MED_BYTES && !g_mem->med)
		g_mem->med = (t_med *)memory;
	if (type == MED_BYTES && g_mem->mtail)
		g_mem->mtail->next = (t_med *)memory;
	if (type == MED_BYTES && (g_mem->mtail = (t_med *)memory))
		ptr = g_mem->mtail->data;
	if (type == SMALL_BYTES && !g_mem->small)
		g_mem->small = (t_small *)memory;
	if (type == SMALL_BYTES && g_mem->stail)
		g_mem->stail->next = (t_small *)memory;
	if (type == SMALL_BYTES && (g_mem->stail = (t_small *)memory))
		ptr = g_mem->stail->data;
	return (ptr);
}

void		*alloc_core(size_t size)
{
	void	*memory;
	void	*ptr;
	int		type;
	int		total;

	memory = NULL;
	total = get_alloc_size(size);
	if (size > MED_BYTES && (type = LARGE))
		memory = mmap(0, total, PROT_READ | PROT_WRITE,
				MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	else if (size <= MED_BYTES && size > SMALL_BYTES &&
			(type = MED_BYTES))
		memory = mmap(0, total, PROT_READ | PROT_WRITE,
				MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	else if ((type = SMALL_BYTES))
		memory = mmap(0, total, PROT_READ | PROT_WRITE,
				MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (memory != (void *)-1)
		ptr = place_memory(memory, type, size);
	return (ptr);
}
