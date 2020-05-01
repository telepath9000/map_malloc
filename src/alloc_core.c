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

#include "../include/map_malloc.h"

static void	insert_chunk(t_unit *chunk, size_t type)
{
	void	*tmp;

	tmp = find_slot(chunk, type);
	((t_unit *)tmp)->next = chunk;
	((t_unit *)chunk)->prev = tmp;
	if (((t_unit *)chunk)->next)
		((t_unit *)chunk)->next->prev = (t_unit *)chunk;
}

/*static int	set_initial(t_unit *chunk, size_t type)
{
	int check;

	check = 0;
	if (type == SMALL_BYTES && !g_mem->small && (check = 1))
		g_mem->small = chunk;
	else if (type == MED_BYTES && !g_mem->med && (check = 1))
		g_mem->med = chunk;
	else if (!g_mem->large && (check = 1))
		g_mem->large = chunk;
	if (!check && type == SMALL_BYTES && chunk > g_mem->small &&
			(chunk->next = g_mem->small) &&
			(g_mem->small->prev = chunk) && (check = 1))
		g_mem->small = chunk;
	else if (!check && type == MED_BYTES && chunk > g_mem->med &&
			(chunk->next = g_mem->med) &&
			(g_mem->med->prev = chunk) && (check = 1))
		g_mem->med = chunk;
	else if (!check && chunk > g_mem->large &&
			(chunk->next = g_mem->large) &&
			(g_mem->large->prev = chunk) && (check = 1))
		g_mem->large = chunk;
	return (check);
}*/

static int set_initial(t_unit *chunk, size_t type)
{
	t_unit	*to_mod;
	int 	check;

	check = 0;
	if (type == SMALL_BYTES)
		to_mod = g_mem->small;
	else if (type == MED_BYTES)
		to_mod = g_mem->med;
	else
		to_mod = g_mem->med;
	if (!to_mod)
	{
		check = 1;
		to_mod = chunk;
	}
	else if (chunk > to_mod)
	{
		check = 1;
		chunk->next = to_mod;
		to_mod->prev = chunk;
		to_mod = chunk;
	}
	return check;
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
