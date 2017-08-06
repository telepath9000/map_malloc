/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebs <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/05 17:37:04 by wdebs             #+#    #+#             */
/*   Updated: 2017/08/05 19:48:00 by wdebs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.c"

static int	small_free(void	*ptr)
{
	t_smalls	*tmp;
	int			i;
	int			ret;

	ret = -1;
	i = -1;
	tmp = NULL;
	while (++i < MAX_SMALLS)
	{
		if (g_mem->tiny[i]->ptr == ptr && (tmp = g_mem->tiny[i]))
			g_mem->tiny_size--;
		if (g_mem->small[i]->ptr == ptr && (tmp = g_mem->small[i]))
			g_mem->small_size--;
		if (tmp && (ret = NO) == 0)
		{
			munmap(tmp->ptr, tmp->size);
			tmp->size = 0;
			tmp->ptr = NULL;
		}
	}
	if (!g_mem->tiny_size && !g_mem->small_size)
		ret = YES;
	return (ret);
}

void		free_node(t_mem *start)
{

}

void		ft_free(void *ptr)
{
	t_mem	*tmp;
	t_mem	*start;
	int		free;

	free = 0;
	start = g_mem;
	if (ptr == NULL)
		return ;
	while (g_mem)
	{
		free = small_free(ptr);
		if (free == NO)
			break ;
		if (free == YES || g_mem->large == ptr)
		{
			if (g_mem == start)
				start = g_mem->next;
			free_node(start);
		}
		tmp = g_mem;
		g_mem = g_mem->next;
	}
	g_mem = start;
}
