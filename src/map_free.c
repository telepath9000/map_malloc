/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebs <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/05 17:37:04 by wdebs             #+#    #+#             */
/*   Updated: 2017/08/07 16:45:38 by wdebs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/map_malloc.h"

void		free_core(t_unit *target, int type)
{
	int	not_begin;

	not_begin = 0;
	if (target->prev && (not_begin = 1))
		target->prev->next = target->next;
	if (type == SMALL_BYTES)
	{
		g_mem->small = !not_begin ? target->next : g_mem->small;
		g_mem->ssize--;
	}
	else if (type == MED_BYTES)
	{
		g_mem->med = !not_begin ? target->next : g_mem->med;
		g_mem->msize--;
	}
	else if (type > MED_BYTES)
	{
		g_mem->large = !not_begin ? target->next : g_mem->large;
		g_mem->lsize--;
	}
	error_handle_munmap(target, get_alloc_size(type));
}

static void	free_large(void *ptr)
{
	t_unit	*cur;

	cur = g_mem->large;
	while (cur)
	{
		if (ptr == (void *)get_address(cur, 0, large))
		{
			free_core((void *)cur, cur->unit.large->size);
			cur = NULL;
			break ;
		}
		if (cur)
			cur = cur->next;
	}
}

static int	free_med(void *ptr)
{
	t_unit	*cur;
	t_med	*cur_ref;
	int		ret;
	int		i;

	cur = g_mem->med;
	ret = 0;
	while ((i = -1) && cur)
	{
		cur_ref = cur->unit.med;
		while (++i < 100)
			if (cur_ref->table[i] && ptr == (void *)get_address(cur, i, med))
			{
				cur_ref->table[i] = 0;
				if (++ret && !(--cur_ref->filled))
					free_core((void *)cur, MED_BYTES);
				cur = NULL;
				break ;
			}
		if (cur)
			cur = cur->next;
	}
	return (ret);
}

static int	free_small(void *ptr)
{
	t_unit	*cur;
	t_small	*cur_ref;
	int		ret;
	int		i;

	cur = g_mem->small;
	ret = 0;
	while (cur)
	{
		i = -1;
		cur_ref = cur->unit.small;
		while (++i < 100)
			if (cur_ref->table[i] && ptr == (void *)get_address(cur, i, small))
			{
				cur_ref->table[i] = 0;
				if (++ret && !(--cur_ref->filled))
					free_core((void *)cur, SMALL_BYTES);
				cur = NULL;
				break ;
			}
		if (cur)
			cur = cur->next;
	}
	return (ret);
}

void		map_free(void *ptr)
{
	if (ptr && g_mem)
	{
		if (!free_small(ptr) && !free_med(ptr))
			free_large(ptr);
		if (!(g_mem->ssize + g_mem->msize + g_mem->lsize))
		{
			error_handle_munmap(g_mem, GLOBAL);
			g_mem = NULL;
		}
	}
}
