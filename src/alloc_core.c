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
	g_mem->total_mem = getpagesize();
}

static void	init_chunk(void *memory, int type, size_t size)
{
	int	i;

	i = 0;
	if (!g_mem)
		init_mem();
	if (type == SMALL_BYTES && (((t_small *)memory)->table[0] = size) &&
			(((t_small *)memory)->filled = 1) && ++g_mem->ssize &&
			(((t_small *)memory)->prev = NULL))
		((t_small *)memory)->next = NULL;
	if (type == MED_BYTES && (((t_med *)memory)->table[0] = size) &&
			(((t_med *)memory)->filled = 1) && ++g_mem->msize &&
			(((t_med *)memory)->prev = NULL))
		((t_med *)memory)->next = NULL;
	if (type == LARGE && ++g_mem->lsize)
	{
		((t_large *)memory)->next = NULL;
		((t_large *)memory)->prev = NULL;
		((t_large *)memory)->size = size;
	}
	while (type == SMALL_BYTES && ++i < 100)
		((t_small *)memory)->table[i] = 0;
	while (type == MED_BYTES && ++i < 100)
		((t_med *)memory)->table[i] = 0;
}

static void	*insert_mem_ascending(void *ptr, int type)
{
	void	*tmp;

	if (type == SMALL_BYTES && (tmp = (void *)g_mem->small))
	{
		while (((t_small *)tmp)->next && ptr > tmp)
			tmp = ((t_small *)tmp)->next;
		((t_small *)ptr)->next = ((t_small *)tmp)->next;
		((t_small *)tmp)->next = (t_small *)ptr;
	}
	else if (type == MED_BYTES && (tmp = (void *)g_mem->med))
	{
		while (((t_med *)tmp)->next && ptr > tmp)
			tmp = ((t_med *)tmp)->next;
		((t_med *)ptr)->next = ((t_med *)tmp)->next;
		((t_med *)tmp)->next = (t_med *)ptr;
	}
	else if (type == LARGE && (tmp = (void *)g_mem->large))
	{
		while (((t_large *)tmp)->next && ptr > tmp)
			tmp = ((t_large *)tmp)->next;
		((t_large *)ptr)->next = ((t_large *)tmp)->next;
		((t_large *)tmp)->next = (t_large *)ptr;
	}
	return (tmp);
}

static void	*place_memory(void *memory, int type, size_t size)
{
	void	*ptr;
	void	*tmp;
	int		check;

	check = 0;
	tmp = NULL;
	init_chunk(memory, type, size);
	(type == LARGE) ? set_limit(size, 1) : set_limit(type, 1);
	if (type == LARGE && !g_mem->large && (check = 1))
		g_mem->large = (t_large *)memory;
	if (type == LARGE)
		ptr = (char *)memory + LARGE_ALLOC;
	if (type == MED_BYTES && !g_mem->med && (check = 1))
		g_mem->med = (t_med *)memory;
	if (type == MED_BYTES)
		ptr = (char *)memory + MED_ALLOC;
	if (type == SMALL_BYTES && !g_mem->small && (check = 1))
		g_mem->small = (t_small *)memory;
	if (type == SMALL_BYTES)
		ptr = (char *)memory + SMALL_ALLOC;
	if (!check)
		tmp = insert_mem_ascending(memory, type);
	if (type == LARGE)
		((t_large *)memory)->prev = (t_large *)tmp;
	else if (type == MED_BYTES)
		((t_med *)memory)->prev = (t_med *)tmp;
	else if (type == SMALL_BYTES)
		((t_small *)memory)->prev = (t_small *)tmp;
	return (ptr);
}

void		*alloc_core(size_t size)
{
	void	*memory;
	void	*ptr;
	int		type;
	int		total;

	if ((total = get_alloc_size(size)) && size > MED_BYTES && (type = LARGE))
		memory = mmap(0, total, PROT_READ | PROT_WRITE,
				MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	else if (size <= MED_BYTES && size > SMALL_BYTES &&
			(type = MED_BYTES))
		memory = mmap(0, total, PROT_READ | PROT_WRITE,
				MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	else if ((type = SMALL_BYTES))
		memory = mmap(0, total, PROT_READ | PROT_WRITE,
				MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if ((ptr = place_memory(memory, type, size)) && type == SMALL_BYTES &&
			((g_mem->stail && g_mem->stail->next == memory) || !g_mem->stail))
		g_mem->stail = (t_small *)memory;
	if (type == MED_BYTES && ((g_mem->mtail && g_mem->mtail->next == memory) ||
				!g_mem->mtail))
		g_mem->mtail = (t_med *)memory;
	if (type == LARGE && ((g_mem->ltail && g_mem->ltail->next == memory) ||
				!g_mem->ltail))
		g_mem->ltail = (t_large *)memory;
	return (ptr);
}
