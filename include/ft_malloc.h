/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebs <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/05 19:39:46 by wdebs             #+#    #+#             */
/*   Updated: 2017/08/07 17:34:35 by wdebs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <sys/mman.h>
# include <unistd.h>
# include <sys/resource.h>
# include <limits.h>

# define TOTAL_STRUCT_SIZE_LARGE
# define TOTAL_STRUCT_SIZE_SMALL	
# define SMALL_ALLOC_TOTAL	19200
# define TINY				32768
# define TINY_SIZE			32
# define TINY_INC			4
# define SMALL				131072
# define SMALL_SIZE			128
# define SMALL_INC			16
# define LARGE				1
# define NOT_LARGE			2
# define YES				1
# define NO					0
# define MAX_SMALLS			200

// figure out how mmap maps a struct and in what order for pointer arithmetic.

typedef struct		s_mem
{
	void			*next; // cast as t_mem
	void			*prev; // cast as t_mem
	void			*large;
	void			*current_small;
	void			*start;
	size_t			large_size;
	size_t			tiny_count;
	size_t			small_count;
	int				type;
}					t_mem;

t_mem				*g_mem;

void				ft_free(void *ptr);
void				*ft_malloc(size_t size);
void*				ft_realloc(void *ptr, size_t size);
t_mem*				alloc_mem(size_t size);
void				show_alloc_mem(void);

#endif
