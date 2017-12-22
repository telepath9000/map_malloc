/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebs <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/05 17:37:04 by wdebs             #+#    #+#             */
/*   Updated: 2017/08/07 16:45:38 by wdebs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_malloc.h"

void		free_core(void *prev, void *target, int type)
{
	if (type == SMALL_BYTES)
	{
		if (prev)
			((t_small *)prev)->next = ((t_small *)target)->next;
		else
			g_mem->small = ((t_small *)target)->next;
		g_mem->ssize--;
	}
	else if (type == MED_BYTES)
	{
		if (prev)
			((t_med *)prev)->next = ((t_med *)target)->next;
		else
			g_mem->med = ((t_med *)target)->next;
		g_mem->msize--;
	}
	else if (type > MED_BYTES)
	{
		if (prev)
			((t_large *)prev)->next = ((t_large *)target)->next;
		else
			g_mem->large = ((t_large*)target)->next;
		g_mem->lsize--;
	}
	error_handle_munmap(target, get_alloc_size(type));
}

static void	free_large(void *ptr)
{
	t_large	*cur;
	t_large *prev;

	prev = NULL;
	cur = g_mem->large;
	while (cur)
	{
		if (ptr == (char *)cur + LARGE_ALLOC)
		{
			free_core((void *)prev, (void *)cur, cur->size);
			cur = NULL;
			break ;
		}
		if (cur)
		{
			prev = cur;
			cur = cur->next;
		}
	}
}

static int	free_med(void *ptr)
{
	t_med	*cur;
	t_med	*prev;
	int		ret;
	int		i;

	cur = g_mem->med;
	prev = NULL;
	ret = 0;
	while ((i = -1) && cur)
	{
		while (++i < 100)
			if (cur->table[i] && ptr == (char *)cur + MED_ALLOC +
					(i * MED_BYTES) && cur->filled--)
			{
				cur->table[i] = 0;
				if (++ret && !cur->filled)
					free_core((void *)prev, (void *)cur, MED_BYTES);
				cur = NULL;
				break ;
			}
		prev = cur;
		if (cur)
			cur = cur->next;
	}
	return (ret);
}

static int	free_small(void *ptr)
{
	t_small	*cur;
	t_small	*prev;
	int		ret;
	int		i;

	cur = g_mem->small;
	prev = NULL;
	ret = 0;
	while ((i = -1) && cur)
	{
		while (++i < 100)
			if (cur->table[i] && ptr == (char *)cur +
					SMALL_ALLOC + (i * SMALL_BYTES) && cur->filled--)
			{
				cur->table[i] = 0;
				if (++ret && !cur->filled)
					free_core((void *)prev, (void *)cur, SMALL_BYTES);
				cur = NULL;
				break ;
			}
		prev = cur;
		if (cur)
			cur = cur->next;
	}
	return (ret);
}

void		ft_free(void *ptr)
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
