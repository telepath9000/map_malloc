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

int		free_small(void *ptr)
{

}

void	ft_free(void *ptr)
{
	if (!free_small(ptr) && !free_med(ptr))
		free_large(ptr);
	if (!(g_mem->ssize + g_mem->msize + g_mem->lsize))
		munmap(g_mem, GLOBAL);
}
