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

#include "ft_malloc.c"

static int		small_free(void	*ptr)
{
	void	*start;
	int		i;
	int		ret;

	ret = -1;
	start = g_mem->current_small;
	i = 0;
	while (i < MAX_SMALLS && ret == -1)
	{
		if (i < 100)
		{
			if (g_mem->current_small == ptr)
			{
				g_mem->current_small = NULL;
				ret = NO;
				g_mem->tiny_count--;
			}
			g_mem->current_small += TINY_INC;
		}
		else if (i < 200)
		{
			if (g_mem->current_small == ptr)
			{
				g_mem->current_small = NULL;
				ret = NO;
				g_mem->small_count--;
			}
			g_mem->current_small += SMALL_INC;
		}
		i++;
	}
	if (!g_mem->tiny_count && !g_mem->small_count)
		ret = YES;
	g_mem->current_small = start;
	return (ret);
}

static t_mem	*free_node(t_mem *start)
{
	t_mem	*tmp;
	size_t	size;

	tmp = NULL;
	size = (g_mem->type == LARGE) ? g_mem->large_size : SMALL_ALLOC_SIZE;
	if (g_mem->prev == NULL)
		start = start->next;
	if (g_mem->prev)
	{
		tmp = g_mem->prev;
		tmp->next = g_mem->next;
	}
	if (g_mem->next)
		g_mem->next->prev = tmp;
	(type == LARGE) ?
		munmap(g_mem->large, size) : munmap(g_mem->current_small, SMALL_ALLOC_SIZE);
	munmap(g_mem, sizeof(t_mem));
	return (start);
}

void			ft_free(void *ptr)
{
	t_mem	*start;
	int		ret;

	start = g_mem;
	ret = NO;
	if (ptr == NULL)
		return ;
	while (g_mem)
	{
		if (g_mem->type == NOT_LARGE)
			ret = small_free(ptr);
		if (ret == YES || ptr == g_mem->large)
		{
			start = free_node(start);
			break ;
		}
		g_mem = g_mem->next;
	}
	g_mem = start;
}
