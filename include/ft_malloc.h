/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebs <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/05 19:39:46 by wdebs             #+#    #+#             */
/*   Updated: 2017/08/05 19:47:57 by wdebs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <sys/mman.h>
# include <unistd.h>
# include <sys/resource.h>
# include <limits.h>

# define TINY		32768 
# define SMALL		131072
# define LARGE		1
# define NOT_LARGE	2
# define YES		1
# define NO			0
# define MAX_SMALLS	100

typedef struct		s_smalls
{
	struct s_smalls	*next;
	struct s_smalls	*head;
	void			*ptr;
}

// allocation for tiny and small needs to be done contiguously in memory for it to be worthwhile.
// large structs can be tacked onto the end, possibly save the start of the large list to jump to it in space?
// small structs can then be added between the end of the smalls and the start of the larges.
// t_smalls structs will now be used as a linked list to hold the pointers to each segment of memory that can
// be used.
// When a small struct is empty it will then be freed completely and the two adjacent structs will be linked
// together.

typedef struct		s_mem
{
	struct s_mem	*next;
	void			*large;
	size_t			tiny_count;
	size_t			small_count;
	int				type;
}					t_mem;

t_mem				*g_mem;

void				ft_free(void *ptr);
void				ft_malloc(size_t size);
void*				ft_realloc(void *ptr, size_t size);
t_mem*				alloc_mem(size_t size);
void				show_alloc_mem(void);

#endif
