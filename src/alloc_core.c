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

static void	insert_chunk(void *chunk, size_t type)
{
	void	*tmp;

	tmp = find_slot(chunk, type);
	if (type == SMALL_BYTES)
	{
		((t_small *)tmp)->next = (t_small *)chunk;
		((t_small *)chunk)->prev = (t_small *)tmp;
		if (((t_small *)chunk)->next)
			((t_small *)chunk)->next->prev = (t_small *)chunk;
	}
	else if (type == MED_BYTES)
	{
		((t_med *)tmp)->next = (t_med *)chunk;
		((t_med *)chunk)->prev = (t_med *)tmp;
		if (((t_med *)chunk)->next)
			((t_med *)chunk)->next->prev = (t_med *)chunk;
	}
	else
	{
		((t_large *)tmp)->next = (t_large *)chunk;
		((t_large *)chunk)->prev = (t_large *)tmp;
		if (((t_large *)chunk)->next)
			((t_large *)chunk)->next->prev = (t_large *)chunk;
	}
}

static int	set_initial(void *chunk, size_t type)
{
	int check;

	check = 0;
	if (type == SMALL_BYTES && !g_mem->small && (check = 1))
		g_mem->small = (t_small *)chunk;
	else if (type == MED_BYTES && !g_mem->med && (check = 1))
		g_mem->med = (t_med *)chunk;
	else if (!g_mem->large && (check = 1))
		g_mem->large = (t_large *)chunk;
	if (!check && type == SMALL_BYTES && (t_small *)chunk > g_mem->small &&
			(((t_small *)chunk)->next = g_mem->small) &&
			(g_mem->small->prev = (t_small *)chunk) && (check = 1))
		g_mem->small = (t_small *)chunk;
	else if (!check && type == MED_BYTES && (t_med *)chunk > g_mem->med &&
			(((t_med *)chunk)->next = g_mem->med) &&
			(g_mem->med->prev = (t_med *)chunk) && (check = 1))
		g_mem->med = (t_med *)chunk;
	else if (!check && (t_large *)chunk > g_mem->large &&
			(((t_large *)chunk)->next = g_mem->large) &&
			(g_mem->large->prev = (t_large *)chunk) && (check = 1))
		g_mem->large = (t_large *)chunk;
	return (check);
}

void		*alloc_core(size_t size)
{
	void	*ret;
	void	*chunk;
	size_t	type;
	size_t	total;

	type = get_type(size);
	total = get_alloc_size(size);
	chunk = mmap(0, total, PROT_READ | PROT_WRITE,
			MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	ret = init_chunk(chunk, type);
	add_to_size(type, size);
	if (!set_initial(chunk, type))
		insert_chunk(chunk, type);
	set_tail(chunk, type);
	return (ret);
}
