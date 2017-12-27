/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebs <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/05 17:37:04 by wdebs             #+#    #+#             */
/*   Updated: 2017/08/07 16:45:38 by wdebs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_malloc.h"

void	add_to_size(size_t type, size_t size)
{

	g_mem->total_mem += size;
	if (type == SMALL_BYTES)
		g_mem->ssize++;
	else if (type == MED_BYTES)
		g_mem->msize++;
	else
		g_mem->lsize++;
}

size_t	get_type(size_t size)
{
	if (size <= SMALL_BYTES)
		return (SMALL_BYTES);
	else if (size > SMALL_BYTES && size <= MED_BYTES)
		return (MED_BYTES);
	else
		return (size);
}
