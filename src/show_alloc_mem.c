/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebs <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/05 17:36:55 by wdebs             #+#    #+#             */
/*   Updated: 2017/08/05 17:48:25 by wdebs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/map_malloc.h"

static void	print_base(size_t val, int base)
{
	size_t	size;
	size_t	tmp;
	char	*radix;
	char	print[21];

	radix = "0123456789abcdef";
	tmp = val;
	size = 0;
	while (tmp && ++size)
		tmp /= base;
	if (base == 16)
		write(1, "0x", 2);
	tmp = size - 1;
	while (val)
	{
		print[tmp] = radix[(val % base)];
		val = val / base;
		tmp--;
	}
	write(1, print, size);
}

static void	print_large_alloc(t_large *node)
{
	t_large	*n;

	while (node)
	{
		write(1, "\n", 1);
		n = (t_large *)((char *)node + LARGE_ALLOC);
		print_base((size_t)&(*n), 16);
		n = (t_large *)((char *)node + (node->size - 1));
		write(1, " - ", 3);
		print_base((size_t)&(*n), 16);
		write(1, " : ", 3);
		print_base(node->size, 10);
		write(1, " bytes", 7);
		node = node->prev;
	}
}

static void	print_small_alloc(t_small *node)
{
	t_small	*n;
	int		checked;
	int		i;

	while (node && (i = -1) && !(checked = 0))
	{
		while (++i < 100 && checked < node->filled)
			if (node->table[i] && (n = ((t_small *)(char *)node + SMALL_ALLOC +
					(i * SMALL_BYTES))) && ++checked)
			{
				write(1, "\n", 1);
				print_base((size_t)&(*n), 16);
				write(1, " - ", 3);
				n = (t_small *)((char *)node + SMALL_ALLOC +
						((i + 1) * SMALL_BYTES) - 1);
				print_base((size_t)&(*n), 16);
				write(1, " : ", 3);
				print_base(node->table[i], 10);
				write(1, " bytes", 6);
			}
		node = node->prev;
	}
	write(1, "\n", 1);
}


static void	print_med_alloc(t_med *node)
{
	t_med	*n;
	int		checked;
	int		i;

	while (node && (i = -1) && !(checked = 0))
	{
		while (++i < 100 && checked < node->filled)
			if (node->table[i] && (n = ((t_med *)(char *)node + MED_ALLOC +
					(i * MED_BYTES))) && ++checked)
			{
				write(1, "\n", 1);
				print_base((size_t)&(*n), 16);
				write(1, " - ", 3);
				n = (t_med *)((char *)node + MED_ALLOC +
						((i + 1) * MED_BYTES) + 1);
				print_base((size_t)&(*n), 16);
				write(1, " : ", 3);
				print_base(node->table[i], 10);
				write(1, " bytes", 6);
			}
		node = node->prev;
	}
	write(1, "\n", 1);
}


void	show_alloc_mem(void)
{
	t_small	*scur;
	t_med	*mcur;
	t_large	*lcur;

	if (g_mem)
	{
		scur = g_mem->stail;
		mcur = g_mem->mtail;
		lcur = g_mem->ltail;
		scur ? write(1, "TINY : ", 7) : 0;
		scur ? print_base((size_t)(&(*scur)), 16) : 0;
		scur ? print_small_alloc(scur) : 0;
		mcur ? write(1, "SMALL : ", 8) : 0;
		mcur ? print_base((size_t)(&(*mcur)), 16) : 0;
		mcur ? print_med_alloc(mcur) : 0;
		lcur ? write(1, "LARGE : ", 8) : 0;
		lcur ? print_base((size_t)(&(*lcur)), 16) : 0;
		lcur ? print_large_alloc(lcur) : 0;
		write(1, "\nTotal : ", 8);
		print_base(g_mem->total_mem, 10);
		write(1, " bytes\n\n", 7);
	}
	else
		write(1, "No memory has been allocated.\n", 30);
}
