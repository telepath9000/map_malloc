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

# define PAGE getpagesize()
# define SMALL_BYTES 256
# define MED_BYTES 512
# define SIZES (sizeof(t_small *) + sizeof(t_large *) + sizeof(t_med *)) * 2
# define GLOBAL SIZES + sizeof(size_t) * 3
# define SMALL (256 * 100) > PAGE ? (((256 * 100) / PAGE) * PAGE) + PAGE : PAGE
# define MED (512 * 100) > PAGE ? (((512 * 100) / PAGE) * PAGE) + PAGE : PAGE
# define LARGE 1
# define SMALL_ALLOC (sizeof(int) * 101) + sizeof(t_small *) + SMALL
# define MED_ALLOC (sizeof(int) * 101) + sizeof(t_med *) + MED
# define LARGE_ALLOC sizeof(int) + sizeof(t_large *)

/*
** Possibly store pointer list indeces in hash table contained in g_mem, one table for each size
** 
** Storing pointer positions in an array of long ints with binary values representing filled or not
**
** For medium size posibly use trie? binary tree with binary representation?
*/

typedef struct		s_small
{
	struct s_small	*next;
	unsigned char	table[13];
	void			*data;
}					t_small;

/*
** possibly turn med into a tree structure
*/

typedef struct		s_med
{
	struct s_med	*next;
	unsigned char	table[13];
	void			*data;
}					t_med;

typedef struct		s_large
{
	struct s_large	*next;
	size_t			size;
	void			*data;
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
}					t_mem;

extern t_mem		*g_mem;

void				ft_free(void *ptr);
void				*ft_malloc(size_t size);
void				*ft_realloc(void *ptr, size_t size);
void				show_alloc_mem(void);

#endif
