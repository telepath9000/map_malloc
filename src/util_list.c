/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_list.c                                        :+:      :+:    :+:   */
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

void	*find_slot(void *chunk, size_t type)
{
	void	*ret;

	if (type == SMALL_BYTES && ((t_small *)ret = g_mem->small))
	{
		while (((t_small *)ret)->next && chunk > ret)
			(t_small *)ret = ((t_small *)ret)->next;
		((t_small *)chunk)->next = ((t_small *)ret)->next;
	}
	else if (type == MED_BYTES && ((t_med *)ret = g_mem->med))
	{
		while (((t_med *)ret)->next && chunk > ret)
			(t_med *)ret = ((t_med *)ret)->next;
		((t_med *)chunk)->next = ((t_med *)ret)->next;
	}
	else if (((t_large *)ret = g_mem->large))
	{
		while (((t_large *)ret)->next && chunk > ret)
			(t_large *)ret = ((t_large *)ret)->next;
		((t_large *)chunk)->next = ((t_large *)ret)->next;
	}
	return (ret);
}

void	*init_chunk(void *mem, size_t type)
{
	void	*ret;
	int		i;

	i = -1;
	if (!g_mem)
		init_mem();
	if (type == SMALL_BYTES && !(((t_small *)mem)->next = NULL) &&
			!(((t_small *)mem)->prev = NULL) && !(((t_small *)mem)->filled = 0))
		while (++i < 100)
			((t_small *)mem)->table[i] = 0;
	else if (type == MED_BYTES && !(((t_med *)mem)->next = NULL) &&
			!(((t_med *)mem)->prev = NULL) && !(((t_med *)mem)->filled = 0))
		while (++i < 100)
			((t_small *)mem)->table[i] = 0;
	else if (!(((t_large *)mem)->next = NULL) &&
			!(((t_large *)mem)->prev = NULL))
		((t_large *)mem)->size = type;
	if (type == SMALL_BYTES)
		ret = (char *)mem + SMALL_ALLOC;
	else if (type == MED_BYTES)
		ret = (char *)mem + MED_ALLOC;
	else
		ret = (char *)mem + LARGE_ALLOC;
	return (ret);
}

void	set_tail(void *chunk, size_t type)
{
	if (g_mem)
	{
		if (type == SMALL_BYTES &&
				(!g_mem->stail ||
				 (g_mem->stail && g_mem->stail->next == chunk)))
			g_mem->stail = chunk;
		else if (type == MED_BYTES &&
				(!g_mem->mtail ||
				 (g_mem->mtail && g_mem->mtail->next == chunk)))
			g_mem->mtail = chunk;
		else if (!g_mem->ltail ||
				(g_mem->ltail && g_mem->ltail->next == chunk))
			g_mem->ltail = chunk;
	}
}
