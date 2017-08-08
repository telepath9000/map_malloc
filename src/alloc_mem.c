/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_mem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebs <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/05 17:14:44 by wdebs             #+#    #+#             */
/*   Updated: 2017/08/07 17:37:56 by wdebs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

t_mem	*alloc_mem(size_t size)
{
	int		type;
	t_mem	*new

	type = size > 128 ? LARGE : NOT_LARGE;
	if (type = LARGE)
		mmap(new, size + sizeof(t_mem),
				PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	else
	{
		//stuff for small
		
	}
	return (new);
}
