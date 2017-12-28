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
# include <sys/time.h>

#include <stdio.h>

# define SMALL_BYTES 1024
# define MED_BYTES 4096
# define SIZES (sizeof(t_small *) + sizeof(t_large *) + sizeof(t_med *)) * 2
# define GLOBAL SIZES + (sizeof(size_t) * 3) + sizeof(rlim_t)
# define LARGE 1
# define SMALL_ALLOC (sizeof(int) * 101) + (sizeof(t_small *) * 2)
# define MED_ALLOC (sizeof(int) * 101) + (sizeof(t_med *) * 2)
# define LARGE_ALLOC sizeof(size_t) + (sizeof(t_large *) * 2)

typedef struct		s_small
{
	struct s_small	*next;
	struct s_small	*prev;
	int				table[100];
	int				filled;
}					t_small;

typedef struct		s_med
{
	struct s_med	*next;
	struct s_med	*prev;
	int				table[100];
	int				filled;
}					t_med;

typedef struct		s_large
{
	struct s_large	*next;
	struct s_large	*prev;
	size_t			size;
}					t_large;

typedef struct		s_mem
{
	t_small			*small;
	t_small			*stail;
	t_med			*med;
	t_med			*mtail;
	t_large			*large;
	t_large			*ltail;
	size_t			ssize;
	size_t			msize;
	size_t			lsize;
	rlim_t			total_mem;
}					t_mem;

extern t_mem		*g_mem;

void				ft_free(void *ptr);
void				free_core(void *prev, void *target, int type);

void				*ft_malloc(size_t size);

void				*ft_realloc(void *ptr, size_t size);

void				show_alloc_mem(void);

void				*alloc_core(size_t size);

/*
** utilities
*/

size_t				get_alloc_size(size_t size);
int					check_limit(size_t size);
int					error_handle_munmap(void *target, size_t size);
void				set_limit(size_t type, size_t inc);
void				malcpy(void *dest, void *src, size_t ref_len, size_t len);

size_t				get_type(size_t size);
void				*init_chunk(void *mem, size_t type);
void				set_tail(void *chunk, size_t type);
void				*find_slot(void *chunk, size_t type);

void				add_to_size(size_t type, size_t size);
size_t				get_type(size_t size);

#endif
