/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                        :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebs <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/05 16:32:37 by wdebs             #+#    #+#             */
/*   Updated: 2017/08/07 16:47:45 by wdebs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_malloc.h"

static size_t	check_large(void *ptr, size_t size)
{
	t_large	*cur;
	t_large *prev;
	size_t	tmp_size;

	prev = NULL;
	cur = g_mem->large;
	tmp_size = 0;
	while (cur)
	{
		if (ptr == (char *)cur + LARGE_ALLOC)
		{
			if (get_alloc_size(tmp_size) < get_alloc_size(size) &&
					(tmp_size = cur->size) && (cur->size = size))
				free_core((void *)prev, (void *)cur, tmp_size);
			cur = NULL;
			break ;
		}
		if (cur)
		{
			prev = cur;
			cur = cur->next;
		}
	}
	return (tmp_size);
}

static size_t	check_med(void *ptr, size_t size)
{
	t_med	*cur;
	t_med	*prev;
	size_t	ret;
	int		i;

	cur = g_mem->med;
	prev = NULL;
	ret = 0;
	while ((i = -1) && cur)
	{
		while (++i < 100)
			if (cur->table[i] && ptr == (char *)cur + MED_ALLOC +
				(i * MED_BYTES) && (ret = cur->table[i]) && (cur->table[i] = size) &&
				(size > MED_BYTES || size < SMALL_BYTES) && cur->filled--)
			{
				cur->table[i] = 0;
				if (!cur->filled)
					free_core((void *)prev, (void *)cur, MED_BYTES);
				return (ret);
			}
		prev = cur;
		if (cur)
			cur = cur->next;
	}
	return (ret);
}

static size_t	check_small(void *ptr, size_t size)
{
	t_small	*cur;
	t_small	*prev;
	size_t	ret;
	int		i;

	cur = g_mem->small;
	prev = NULL;
	ret = 0;
	while ((i = 0) && cur)
	{
		while (++i < 100)
			if (cur->table[i] && ptr == (char *)cur + SMALL_ALLOC + (i * SMALL_BYTES) &&
				(ret = cur->table[i]) && (cur->table[i] = size) &&
			   	size > SMALL_BYTES && cur->filled--)
			{
				cur->table[i] = 0;
				if (!cur->filled)
					free_core((void *)prev, (void *)cur, SMALL_BYTES);
				return (ret);
			}
		prev = cur;
		if (cur)
			cur = cur->next;
	}
	return (ret);
}

void		*ft_realloc(void *ptr, size_t size)
{
	void	*ret;
	size_t	ref_size;

	ret = ptr;
	if (((ref_size = check_small(ptr, size)) && size > SMALL_BYTES) ||
			((ref_size = check_med(ptr, size)) && size > SMALL_BYTES &&
			 size <= MED_BYTES) ||
			((ref_size = check_large(ptr, size)) && size > MED_BYTES))
	{
		ret = ft_malloc(size);
		malcpy(ret, ptr, ref_size, size);
	}
	return (ret);
}
