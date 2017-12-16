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

#include "ft_malloc.h"

t_mem		*g_mem = NULL;

static void	init_mem(void)
{
	mmap((void *)g_mem, GLOBAL_ALLOC,
				PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	g_mem->small = NULL;
	g_mem->stail = NULL;
	g_mem->med = NULL;
	g_mem->mtail = NULL;
	g_mem->large = NULL;
	g_mem->ltail = NULL;
	g_mem->ssize = 0;
	g_mem->msize = 0;
	g_mem->lsize = 0;
}

static void	init_chunk(void *memory, char type, size_t size)
{
	int	i;

	i = 0;
	if (!g_mem)
		init_mem();
	while (type == SMALL && ++i < 14)
	{
		((t_small *)memory)->next = NULL;
		((t_small *)memory)->table[i] = 0;
		g_mem->ssize++;
	}
	while (typ == MED && ++i < 14)
	{
		((t_med *)memory)->next = NULL;
		((t_med *)memory)->table[i] = 0;
		g_mem->msize++;
	}
	if (type == LARGE)
	{
		((t_large *)memory)->next = NULL;
		((t_large *)memory)->size = size;
		g_mem->lsize++;
	}
}


static void	*place_memory(void *memory, char type, size_t size)
{
	void	*ptr;
	void	*mem;

	init_chunk(memory, type, size);
	if (type == LARGE)
	{
		if (!g_mem->large)
		{
			g_mem->large = (t_large *)memory
}

void		*alloc_core(size_t size)
{
	char	type;
	void	*memory;
	void	*ptr;

	if (size > MED_BYTES && (type = LARGE))
		mmap(memory, LARGE_ALLOC + size,
				PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	else if (size <= MED_BYTES && size > SMALL_BYTES && (type = MED))
		mmap(memory, MED_ALLOC,
				PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	else if ((type = SMALL))
		mmap(memory, SMALL_ALLOC,
				PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	ptr = place_memory(memory, type, size);
	return (ptr);
}
