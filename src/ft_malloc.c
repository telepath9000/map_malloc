/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebs <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/05 16:32:37 by wdebs             #+#    #+#             */
/*   Updated: 2017/08/07 16:47:45 by wdebs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_malloc.h"

t_mem		*g_mem = NULL;

/*
**	Will implement different data structures for small and medium sizes later, this is why they
**	are seperate typedef's.
*/

static void	*search_memory_med(void)
{
	void	*ret;
	t_med	*cur;
	int		i;

	i = 8;
	ret = NULL;
	cur = g_mem->med;
	while (cur)
	{
		if (cur->filled < 100)
		{
			while ((cur->table[(i / 8) - 1] << (i % 8)) & 1)
				i++;
			cur->table[(i / 8) - 1] |= (1 << (i % 8));
			cur->filled++;
			ret = cur->data + (i * MED_BYTES);
			break ;
		}
		cur = cur->next;
	}
	return (ret);
}

static void	*search_memory_small(void)
{
	void	*ret;
	t_small	*cur;
	int		i;

	i = 8;
	ret = NULL;
	cur = g_mem->small;
	while (cur)
	{
		if (cur->filled < 100)
		{
			while ((cur->table[(i / 8) - 1] << (i % 8)) & 1)
				i++;
			cur->table[(i / 8) - 1] |= (1 << (i % 8));
			cur->filled++;
			ret = cur->data + (i * SMALL_BYTES);
			break ;
		}
		cur = cur->next;
	}
	return (ret);
}

void		*ft_malloc(size_t size)
{
	void	*ptr;

	ptr = NULL;
	if (size > 0)
	{
		if (g_mem)
		{
			if (size <= SMALL_BYTES)
				ptr = search_memory_small();
			else if (size <= MED_BYTES)
				ptr = search_memory_med();
			if (!ptr)
				ptr = alloc_core(size);
		}
	}
	return (ptr);
}
